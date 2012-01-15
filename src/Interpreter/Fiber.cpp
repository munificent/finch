#include <iostream>

#include "ArrayObject.h"
#include "BlockObject.h"
#include "Block.h"
#include "DynamicObject.h"
#include "FiberObject.h"
#include "IInterpreterHost.h"
#include "Interpreter.h"
#include "Fiber.h"

#ifdef TRACE_INSTRUCTIONS

#include <iomanip>
#include <sstream>

#define TRACE_INSTRUCTION(instruction) TraceInstruction(instruction)
#define TRACE_STACK() TraceStack()
#else
#define TRACE_INSTRUCTION(instruction) ;
#define TRACE_STACK() ;
#endif

namespace Finch
{
    using std::cout;
    using std::endl;

    Fiber::Fiber(Interpreter & interpreter, Ref<Object> block)
    :   mIsRunning(false),
        mInterpreter(interpreter),
        mStack(),
        mCallFrames()
    {
        ArgReader args(mStack, 0, 0);

        // Top-level blocks outside of any method bind self to nil.
        CallBlock(interpreter.Nil().Obj(), block, args);
    }

    bool Fiber::IsDone() const
    {
        return mCallFrames.Count() == 0;
    }

    Ref<Object> Fiber::Execute()
    {
        mIsRunning = true;

        // Continue processing bytecode until the entire callstack has returned
        // or we pause and switch to another fiber.
        while (mIsRunning)
        {
            CallFrame & frame = mCallFrames.Peek();

            // Read and decode the next instruction.
            Instruction instruction = frame.Block().Code()[frame.ip++];
            OpCode op = DECODE_OP(instruction);
            int a = DECODE_A(instruction);
            int b = DECODE_B(instruction);
            int c = DECODE_C(instruction);

            TRACE_INSTRUCTION(instruction);

            switch (op)
            {
                case OP_CONSTANT:
                    Store(frame, b, frame.Block().GetConstant(a));
                    break;

                case OP_OBJECT:
                {
                    // The parent is already in the register that the child
                    // will be placed into.
                    Ref<Object> parent = Load(frame, a);
                    Value object = mInterpreter.NewObject(Value::HackWrapRef(parent));
                    Store(frame, a, object.Obj());
                    break;
                }

                case OP_BLOCK:
                {
                    // Create a new block object from the block.
                    Ref<Block> block = frame.Block().GetBlock(a);
                    Value blockObj = mInterpreter.NewBlock(block, Value::HackWrapRef(Self()));
                    BlockObject * blockPtr = blockObj.AsBlock();

                    // Capture upvalues.
                    for (int i = 0; i < block->NumUpvalues(); i++)
                    {
                        Instruction capture = frame.Block().Code()[frame.ip++];
                        OpCode captureOp = DECODE_OP(capture);
                        int captureIndex = DECODE_A(capture);

                        switch (captureOp)
                        {
                            case OP_CAPTURE_LOCAL:
                                blockPtr->AddUpvalue(CaptureUpvalue(
                                    frame.stackStart + captureIndex));
                                break;

                            case OP_CAPTURE_UPVALUE:
                                blockPtr->AddUpvalue(frame.Block().GetUpvalue(captureIndex));
                                break;

                            default:
                                ASSERT(false, "Unexpected capture pseudo-op.");
                        }
                    }

                    Store(frame, b, blockObj);
                    break;
                }

                case OP_ARRAY:
                {
                    // Create the empty array with enough capacity. Subsequent
                    // OP_ARRAY_ELEMENT instructions will fill it.
                    Value array = mInterpreter.NewArray(a);
                    Store(frame, b, array.Obj());
                    break;
                }

                case OP_ARRAY_ELEMENT:
                {
                    // Add the item to the array.
                    Ref<Object> element = Load(frame, a);
                    Ref<Object> array = Load(frame, b);
                    array->AsArray()->Elements().Add(Value::HackWrapRef(element));
                    break;
                }

                case OP_MOVE:
                    //cout << "MOVE     " << a << " -> " << b << endl;
                    Store(frame, b, Load(frame, a));
                    break;

                case OP_SELF:
                    Store(frame, a, Self());
                    break;

                case OP_MESSAGE_0:
                case OP_MESSAGE_1:
                case OP_MESSAGE_2:
                case OP_MESSAGE_3:
                case OP_MESSAGE_4:
                case OP_MESSAGE_5:
                case OP_MESSAGE_6:
                case OP_MESSAGE_7:
                case OP_MESSAGE_8:
                case OP_MESSAGE_9:
                case OP_MESSAGE_10:
                {
                    //String name = GetEnvironment().Strings().Find(a);
                    //cout << "MESSAGE  " << name << " " << b << " -> " << c << endl;
                    int numArgs = op - OP_MESSAGE_0;

                    Ref<Object> result = SendMessage(a, b, numArgs);

                    // A non-null result means the message was handled by a
                    // primitive that immediately calculated the result.
                    // Otherwise it's a normal method which will push a new
                    // callframe. When that method returns, it will handle
                    // setting the result on the caller.
                    if (!result.IsNull())
                    {
                        Store(frame, c, result);
                    }
                    break;
                }

                case OP_GET_UPVALUE:
                {
                    Ref<Upvalue> upvalue = frame.Block().GetUpvalue(a);
                    Store(frame, b, upvalue->Get(mStack));
                    break;
                }

                case OP_SET_UPVALUE:
                {
                    Ref<Upvalue> upvalue = frame.Block().GetUpvalue(a);
                    upvalue->Set(mStack, Value::HackWrapRef(Load(frame, b)));
                    break;
                }

                case OP_GET_FIELD:
                {
                    Value field = Self()->GetField(a);
                    // TODO(bob): Just make a null Value equivalent to nil.
                    if (!field.IsNull())
                    {
                        Store(frame, b, field);
                    }
                    else
                    {
                        // TODO(bob): Should this be an error instead?
                        Store(frame, b, Nil());
                    }
                    break;
                }

                case OP_SET_FIELD:
                {
                    Self()->SetField(a, Value::HackWrapRef(Load(frame, b)));
                    break;
                }

                case OP_GET_GLOBAL:
                {
                    const Value & value = mInterpreter.GetGlobal(a);

                    if (!value.IsNull())
                    {
                        Store(frame, b, value);
                    }
                    else
                    {
                        String name = mInterpreter.FindGlobalName(a);
                        Error(String::Format(
                                             "Trying to access undefined global '%s'.",
                                             name.CString()));
                        Store(frame, b, mInterpreter.Nil());
                    }
                    break;
                }

                case OP_SET_GLOBAL:
                {
                    Value value = Value::HackWrapRef(Load(frame, b));
                    mInterpreter.SetGlobal(a, value);
                    break;
                }

                case OP_DEF_METHOD:
                {
                    // Get the object we're attaching the method to.
                    DynamicObject * object = Load(frame, c)->AsDynamic();
                    // TODO(bob): What should this do if you try to bind a
                    // method to something non-dynamic?
                    ASSERT_NOT_NULL(object);

                    object->AddMethod(a, Value::HackWrapRef(Load(frame, b)));
                    break;
                }

                case OP_DEF_FIELD:
                {
                    // Get the object we're attaching the field to.
                    DynamicObject * object = Load(frame, c)->AsDynamic();
                    // TODO(bob): What should this do if you try to bind a
                    // field to something non-dynamic?
                    ASSERT_NOT_NULL(object);

                    object->SetField(a, Value::HackWrapRef(Load(frame, b)));
                    break;
                }

                case OP_END:
                {
                    Ref<Object> result = Load(frame, a);
                    PopCallFrame();

                    if (mCallFrames.Count() > 0)
                    {
                        StoreMessageResult(result);
                    }
                    else
                    {
                        // The fiber has completely unwound, so return the
                        // final result value.
                        TRACE_STACK();
                        return result;
                    }
                    break;
                }

                case OP_RETURN:
                {
                    int methodId = a;

                    Ref<Object> result = Load(frame, b);

                    // Find the enclosing method on the callstack.
                    int methodFrame;
                    for (methodFrame = 0; methodFrame < mCallFrames.Count(); methodFrame++)
                    {
                        if (mCallFrames[methodFrame].Block().MethodId() == methodId)
                        {
                            // Found it.
                            break;
                        }
                    }

                    if (methodFrame == mCallFrames.Count())
                    {
                        Error("Cannot return from a block whose enclosing method has already returned.");
                        // Unwind the whole stack.
                        methodFrame = mCallFrames.Count() - 1;
                    }

                    // Unwind until we reach the method.
                    while (methodFrame >= 0)
                    {
                        PopCallFrame();
                        methodFrame--;
                    }

                    if (mCallFrames.Count() > 0)
                    {
                        StoreMessageResult(result);
                    }
                    else
                    {
                        // If we unwound everything, end the fiber.
                        TRACE_STACK();
                        return result;
                    }
                    break;
                }

                default:
                    std::cout << op << std::endl;
                    ASSERT(false, "Unknown opcode.");
            }

            TRACE_STACK();
        }

        return Ref<Object>();
    }

    Ref<Object> Fiber::Load(const CallFrame & frame, int reg)
    {
        return mStack[frame.stackStart + reg];
    }

    void Fiber::Store(const CallFrame & frame, int reg, Ref<Object> value)
    {
        mStack[frame.stackStart + reg] = value;
    }
    
    void Fiber::Store(const CallFrame & frame, int reg, const Value & value)
    {
        // TODO(bob): Hackish. Need to copy value to have non-const one that
        // we can call Obj() on.
        Value copy = value;
        Store(frame, reg, copy.Obj());
    }


    void Fiber::PopCallFrame()
    {
        CallFrame & frame = mCallFrames.Peek();
        int oldStackSize = frame.stackStart + frame.Block().NumRegisters();
        mCallFrames.Pop();

        // Discard the callee frame's registers.
        int newStackSize = 0;
        if (mCallFrames.Count() > 0)
        {
            CallFrame & caller = mCallFrames.Peek();
            newStackSize = caller.stackStart + caller.Block().NumRegisters();
        }

        // Close any open upvalues that are being popped off
        // the stack.
        while (!mOpenUpvalues.IsNull())
        {
            if (mOpenUpvalues->Index() < newStackSize) break;

            mOpenUpvalues->Close(mStack);
            mOpenUpvalues = mOpenUpvalues->Next();
        }

        // Clear any discarded registers on the stack. Note that we don't
        // actually truncate the stack here. This is important because we may
        // still need those registers. Consider:
        //
        // 0               0
        // [ .  .  .  .  . ] We push a frame with a lot of temp registers.
        // 0   1    1      0
        // [ . [.  .] .  . ] Then we push a frame with just a few registers that
        // 0   1  2 1   2  0 overlaps the previous one a lot.
        // [ . [. [.] . ]. ] Then we push another.
        // 0   1    1
        // [ . [.  .] ?????? Then we return from that one and truncate to the
        //                   caller's (1's) num registers.
        // Oops! We've trashed registers that 0 may actually need later.
        // Overlapping register windows are hard, let's go shopping!
        //
        // Instead, we'll just *clear* the popped registers (because we know
        // they aren't in use after the call even though they may get used
        // again) but keep them around in case later callers need them.
        for (int i = newStackSize; i < oldStackSize; i++)
        {
            mStack[i] = Ref<Object>();
        }
    }

    void Fiber::StoreMessageResult(Ref<Object> result)
    {
        // Store the result back in the caller's dest register.
        CallFrame & caller = mCallFrames.Peek();
        Instruction instruction = caller.Block().Code()[caller.ip - 1];

        ASSERT((DECODE_OP(instruction) >= OP_MESSAGE_0) &&
               (DECODE_OP(instruction) <= OP_MESSAGE_10),
               "Should be returning to a message instruction.");

        int dest = instruction & 0x000000ff; // c
        Store(caller, dest, result);
    }

    // TODO(bob): Move this into Object?
    Ref<Object> Fiber::SendMessage(StringId messageId, int receiverReg, int numArgs)
    {
        Ref<Object> self = Load(mCallFrames.Peek(), receiverReg);
        Ref<Object> receiver = self;

        ASSERT(!receiver.IsNull(), "Should have receiver.");

        ArgReader args(mStack, mCallFrames.Peek().stackStart + receiverReg + 1,
                       numArgs);

        // Walk the parent chain looking for a method that matches the message.
        while (true)
        {
            // See if the object has a method bound to that name.
            Value method = receiver->FindMethod(messageId);
            if (!method.IsNull())
            {
                CallBlock(self, method.Obj(), args);
                return Ref<Object>();
            }

            // See if the object has a primitive bound to that name.
            PrimitiveMethod primitive = receiver->FindPrimitive(messageId);
            if (primitive != NULL)
            {
                return primitive(*this, self, args);
            }

            // If we're at the root of the inheritance chain, then stop.
            if (receiver->Parent().Obj() == receiver) break;
            receiver = receiver->Parent().Obj();
        }

        // If we got here, the object didn't handle the message.
        String messageName = mInterpreter.FindString(messageId);
        String error = String::Format("Object '%s' did not handle message '%s'",
            receiver->AsString().CString(), messageName.CString());
        Error(error);

        // Unhandled messages just return nil.
        return mInterpreter.Nil().Obj();
    }

    Ref<Object> Fiber::Self()
    {
        return mCallFrames.Peek().receiver;
    }

    Ref<Object> Fiber::Nil()
    {
        return mInterpreter.Nil().Obj();
    }

    Ref<Object> Fiber::CreateBool(bool value)
    {
        return value ? mInterpreter.True().Obj() : mInterpreter.False().Obj();
    }

    Ref<Object> Fiber::CreateNumber(double value)
    {
        return mInterpreter.NewNumber(value).Obj();
    }

    Ref<Object> Fiber::CreateString(const String & value)
    {
        return mInterpreter.NewString(value).Obj();
    }

    void Fiber::CallBlock(Ref<Object> receiver, Ref<Object> blockObj, ArgReader & args)
    {
        BlockObject & block = *(blockObj->AsBlock());

        // Allocate this frame's registers.
        // TODO(bob): Make this a single operation on Array.
        while (mStack.Count() < args.StackStart() + block.NumRegisters())
        {
            mStack.Add(Ref<Object>());
        }

        // If there aren't enough arguments, nil out the remaining parameters.
        for (int i = args.NumArgs(); i < block.NumParams(); i++)
        {
            mStack[args.StackStart() + i] = Nil();
        }

        mCallFrames.Push(CallFrame(args.StackStart(), receiver, blockObj));
    }

    void Fiber::Error(const String & message)
    {
        mInterpreter.GetHost().Error(message);
    }

    int Fiber::GetCallstackDepth() const
    {
        return mCallFrames.Count();
    }

    Ref<Upvalue> Fiber::CaptureUpvalue(int stackIndex)
    {
        // If there are no open upvalues at all, we must need a new one.
        if (mOpenUpvalues.IsNull())
        {
            mOpenUpvalues = Ref<Upvalue>(new Upvalue(stackIndex));
            return mOpenUpvalues;
        }

        Ref<Upvalue> prevUpvalue;
        Ref<Upvalue> upvalue = mOpenUpvalues;
        while (true)
        {
            if (upvalue.IsNull() || (upvalue->Index() < stackIndex))
            {
                // We've gone past this item on the stack, so there must not be
                // an open upvalue for it. Make a new one and link it in in the
                // right place to keep the list sorted.
                Ref<Upvalue> newUpvalue = Ref<Upvalue>(new Upvalue(stackIndex));

                if (prevUpvalue.IsNull())
                {
                    // Our new one is the first one in the list.
                    mOpenUpvalues = newUpvalue;
                }
                else
                {
                    prevUpvalue->SetNext(newUpvalue);
                }
                newUpvalue->SetNext(upvalue);

                return newUpvalue;
            }
            else if (upvalue->Index() == stackIndex)
            {
                // Already have an open upvalue, so use it.
                return upvalue;
            }

            // Walk towards the bottom of the stack.
            prevUpvalue = upvalue;
            upvalue = upvalue->Next();
        }
    }

#ifdef TRACE_INSTRUCTIONS
    void Fiber::TraceInstruction(Instruction instruction)
    {
        using namespace std;

        OpCode op = DECODE_OP(instruction);
        int a = DECODE_A(instruction);
        int b = DECODE_B(instruction);
        int c = DECODE_C(instruction);

        String opName;
        String action;
        switch (op)
        {
            case OP_CONSTANT:
                opName = "CONSTANT";
                action = String::Format("%d -> %d", a, b);
                break;

            case OP_OBJECT:
                opName = "OBJECT";
                action = String::Format("-> %d", a);
                break;

            case OP_BLOCK:
                opName = "BLOCK";
                action = String::Format("b%d -> %d", a, b);
                break;

            case OP_ARRAY:
                opName = "ARRAY";
                action = String::Format("[%d] -> %d", a, b);
                break;

            case OP_ARRAY_ELEMENT:
                opName = "ARRAY_ELEMENT";
                action = String::Format("%d -> %d", a, b);
                break;

            case OP_MOVE:
                opName = "MOVE";
                action = String::Format("%d -> %d", a, b);
                break;

            case OP_SELF:
                opName = "SELF";
                action = String::Format("self -> %d", a);
                break;

            case OP_MESSAGE_0:
            case OP_MESSAGE_1:
            case OP_MESSAGE_2:
            case OP_MESSAGE_3:
            case OP_MESSAGE_4:
            case OP_MESSAGE_5:
            case OP_MESSAGE_6:
            case OP_MESSAGE_7:
            case OP_MESSAGE_8:
            case OP_MESSAGE_9:
            case OP_MESSAGE_10:
            {
                opName = String::Format("MESSAGE_%d", op - OP_MESSAGE_0);
                String name = GetEnvironment().Strings().Find(a);
                action = String::Format("'%s' %d -> %d", name.CString(), b, c);
                break;
            }

            case OP_GET_UPVALUE:
                opName = "GET_UPVALUE";
                action = String::Format("u%d -> %d", a, b);
                break;

            case OP_SET_UPVALUE:
                opName = "SET_UPVALUE";
                action = String::Format("u%d <- %d", a, b);
                break;

            case OP_GET_FIELD:
            {
                opName = "GET_FIELD";
                String name = GetEnvironment().Strings().Find(a);
                action = String::Format("'%s' -> %d", name.CString(), b);
                break;
            }

            case OP_SET_FIELD:
            {
                opName = "SET_FIELD";
                String name = GetEnvironment().Strings().Find(a);
                action = String::Format("'%s' <- %d", name.CString(), b);
                break;
            }

            case OP_GET_GLOBAL:
            {
                opName = "GET_GLOBAL";
                String name = GetEnvironment().FindGlobalName(a);
                action = String::Format("%d '%s' -> %d", a, name.CString(), b);
                break;
            }

            case OP_SET_GLOBAL:
            {
                opName = "SET_GLOBAL";
                String name = GetEnvironment().FindGlobalName(a);
                action = String::Format("%d '%s' <- %d", a, name.CString(), b);
                break;
            }

            case OP_DEF_METHOD:
            {
                opName = "DEF_METHOD";
                String name = GetEnvironment().Strings().Find(a);
                action = String::Format("'%s' %d -> %d", name.CString(), b, c);
                break;
            }

            case OP_DEF_FIELD: // a name, b value, c obj
            {
                opName = "DEF_FIELD";
                String name = GetEnvironment().Strings().Find(a);
                action = String::Format("'%s' %d -> %d", name.CString(), b, c);
                break;
            }

            case OP_END:
                opName = "END";
                action = String::Format("^ %d", a);
                break;

            case OP_RETURN:
                opName = "RETURN";
                action = String::Format("m%d ^ %d", a, b);
                break;

            default:
                opName = String::Format("UNKNOWN OP(%d)", op);
                action = "";
        }

        cout << String::Format("%-14s %-28s  ", opName.CString(), action.CString());
    }

    void Fiber::TraceStack()
    {
        using namespace std;

        int j = mCallFrames.Count() - 1;
        for (int i = 0; i < mStack.Count(); i++)
        {
            if (j >= 0 && mCallFrames[j].stackStart == i)
            {
                cout << " > ";
                j--;
            }
            else
            {
                cout << " | ";
            }

            // Truncate it to fit ten characters.
            stringstream out;
            out << mStack[i];
            String value = String(out.str().c_str());
            value = value.Replace("\n", "\\n");
            if (value.Length() > 10)
            {
                value = value.Substring(0, 9) + "…";
            }

            cout << left << setw(10) << value;
        }
        cout << endl;
    }
#endif
}

