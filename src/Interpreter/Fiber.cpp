#include <iostream>
#include <iomanip> // For debug tracing.

#include "ArrayObject.h"
#include "BlockObject.h"
#include "CodeBlock.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "FiberObject.h"
#include "IInterpreterHost.h"
#include "Interpreter.h"
#include "Fiber.h"

#ifdef TRACE_INSTRUCTIONS
#define TRACE_INSTRUCTION(instruction) TraceInstruction(instruction)
#define TRACE_STACK() TraceStack()
#else
#define TRACE_INSTRUCTION(instruction) 0
#define TRACE_STACK() 0
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
        CallBlock(interpreter.GetEnvironment().Nil(), block, args);
    }
    
    bool Fiber::IsDone() const
    {
        return mCallFrames.Count() == 0;
    }
    
    Ref<Object> Fiber::Execute()
    {
        // TODO(bob): Implement!
        mIsRunning = true;
        
        // Continue processing bytecode until the entire callstack has returned
        // or we pause and switch to another fiber.
        while (mIsRunning)
        {
            CallFrame & frame = mCallFrames.Peek();
            
            // Read and decode the next instruction.
            Instruction instruction = frame.Block().GetCode()[frame.ip++];
            OpCode op = static_cast<OpCode>((instruction & 0xff000000) >> 24);
            int a = (instruction & 0x00ff0000) >> 16;
            int b = (instruction & 0x0000ff00) >> 8;
            int c = instruction & 0x000000ff;
            
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
                    Ref<Object> object = Object::NewObject(parent);
                    Store(frame, a, object);
                    break;
                }
                    
                case OP_BLOCK:
                {
                    //cout << "BLOCK    " << a << " -> " << b << endl;
                    // Create a new block from the exemplar.
                    Ref<BlockExemplar> exemplar = frame.Block().GetExemplar(a);
                    
                    // TODO(bob): Capture closure.
                    
                    Ref<Object> block = Object::NewBlock(GetEnvironment(),
                        exemplar, Self());
                    
                    BlockObject * blockObj = block->AsBlock();
                    
                    // Capture upvalues.
                    for (int i = 0; i < exemplar->GetNumUpvalues(); i++)
                    {
                        Instruction capture = frame.Block().GetCode()[frame.ip++];
                        OpCode captureOp = static_cast<OpCode>((capture & 0xff000000) >> 24);
                        int captureIndex = (capture & 0x00ff0000) >> 16;
                        
                        // TODO(bob): This is wrong. This is capturing the
                        // *value* when it should capture the *variable. If
                        // other code assigns to the captured variable's, this
                        // new block won't see the change.
                        // To fix this, we need Lua's Upvalue objects.
                        switch (captureOp)
                        {
                            case OP_CAPTURE_LOCAL:
                                blockObj->AddUpvalue(CaptureUpvalue(
                                    frame.stackStart + captureIndex));
                                break;
                                
                            case OP_CAPTURE_UPVALUE:
                                blockObj->AddUpvalue(frame.Block().GetUpvalue(captureIndex));
                                break;
                                
                            default:
                                ASSERT(false, "Unexpected capture pseudo-op.");
                        }
                    }
                    
                    Store(frame, b, block);
                    break;
                }
                
                case OP_ARRAY:
                {
                    // Create the empty array with enough capacity. Subsequent
                    // OP_ARRAY_ELEMENT instructions will fill it.
                    Ref<Object> array = Object::NewArray(GetEnvironment(), a);
                    Store(frame, b, array);
                    break;
                }
                    
                case OP_ARRAY_ELEMENT:
                {
                    // Add the item to the array.
                    Ref<Object> element = Load(frame, a);
                    Ref<Object> array = Load(frame, b);
                    array->AsArray()->Elements().Add(element);
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
                    upvalue->Set(mStack, Load(frame, b));
                    break;
                }
                    
                case OP_GET_FIELD:
                {
                    Ref<Object> field = Self()->GetField(a);
                    if (field.IsNull())
                    {
                        // TODO(bob): Should this be an error instead?
                        field = Nil();
                    }
                    
                    Store(frame, b, field);
                    break;
                }
                    
                case OP_SET_FIELD:
                {
                    Self()->SetField(a, Load(frame, b));
                    break;
                }
                    
                case OP_GET_GLOBAL:
                {
                    Ref<Object> value = GetEnvironment().GetGlobal(a);
                    
                    if (value.IsNull())
                    {
                        String name = GetEnvironment().FindGlobalName(a);
                        Error(String::Format(
                            "Trying to access undefined global '%s'.",
                            name.CString()));
                        value = GetEnvironment().Nil();
                    }
                    
                    // TODO(bob): Handle undefined globals.
                    Store(frame, b, value);
                    break;
                }
                    
                case OP_SET_GLOBAL:
                {
                    GetEnvironment().SetGlobal(a, Load(frame, b));
                    break;
                }
                
                case OP_DEF_METHOD:
                {
                    // Get the object we're attaching the method to.
                    DynamicObject * object = Load(frame, c)->AsDynamic();
                    // TODO(bob): What should this do if you try to bind a
                    // method to something non-dynamic?
                    ASSERT_NOT_NULL(object);
                    
                    object->AddMethod(a, Load(frame, b));
                    break;
                }
                    
                case OP_DEF_FIELD:
                {
                    // Get the object we're attaching the field to.
                    DynamicObject * object = Load(frame, c)->AsDynamic();
                    // TODO(bob): What should this do if you try to bind a
                    // field to something non-dynamic?
                    ASSERT_NOT_NULL(object);
                    
                    object->SetField(a, Load(frame, b));
                    break;
                }
                    
                case OP_RETURN:
                {
                    //cout << "RETURN   " << a << endl;
                    Ref<Object> result = Load(frame, a);
                    
                    int oldStackSize = frame.stackStart + frame.Block().GetNumRegisters();
                    mCallFrames.Pop();
                    
                    if (mCallFrames.Count() > 0)
                    {
                        int before = mStack.Count();
                        
                        // Discard the callee frame's registers.
                        CallFrame & caller = mCallFrames.Peek();
                        int newStackSize = caller.stackStart +
                            caller.Block().GetNumRegisters();
                        
                        // Close any open upvalues that are being popped off
                        // the stack.
                        for (int i = mOpenUpvalues.Count() - 1; i >= 0; i--)
                        {
                            Ref<Upvalue> upvalue = mOpenUpvalues[i];
                            if (upvalue->Index() >= newStackSize)
                            {
                                upvalue->Close(mStack);
                                mOpenUpvalues.RemoveAt(i);
                            }
                        }
                        
                        // Clear any discarded registers on the stack.
                        // Note that we don't actually truncate the stack here.
                        // This is important because we may still need those
                        // registers. Consider:
                        //
                        // 0               0
                        // [ .  .  .  .  . ] We push a frame with a lot of temp
                        //                   registers.
                        // 0   1    1      0
                        // [ . [.  .] .  . ] Then we push a frame with just a
                        //                   few registers that overlaps the
                        // 0   1  2 1   2  0 previous one a lot.
                        // [ . [. [.] . ]. ] Then we push another.
                        //
                        // 0   1    1 
                        // [ . [.  .] ?????? Then we return from that one and
                        //                   truncate to the caller's (1's) num
                        //                   registers.
                        // Oops! We've trashed registers that 0 may actually
                        // need later. Overlapping register windows are hard,
                        // let's go shopping!
                        //
                        // Instead, we'll just *clear* the popped registers
                        // (because we know they aren't in use after the call
                        // even though they may get used again) but keep them
                        // around in case later callers need them.
                        
                        for (int i = newStackSize; i < oldStackSize; i++)
                        {
                            mStack[i] = Ref<Object>();
                        }
                        
                        // Store the result back in the caller's dest register.
                        Instruction messageInstruction = caller.Block().GetCode()[caller.ip - 1];
                        int dest = messageInstruction & 0x000000ff; // c
                        Store(caller, dest, result);
                    }
                    else
                    {
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
        
        return Ref<Object>(Object::NewString(
            mInterpreter.GetEnvironment(), "implement me"));
    }
    
    Ref<Object> Fiber::Load(const CallFrame & frame, int reg)
    {
        return mStack[frame.stackStart + reg];
    }
    
    void Fiber::Store(const CallFrame & frame, int reg, Ref<Object> value)
    {
        mStack[frame.stackStart + reg] = value;
    }

    Environment & Fiber::GetEnvironment()
    {
        return mInterpreter.GetEnvironment();
    }
    
    // TODO(bob): Move this into Object?
    Ref<Object> Fiber::SendMessage(int messageId, int receiverReg, int numArgs)
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
            Ref<Object> method = receiver->FindMethod(messageId);
            if (!method.IsNull())
            {
                CallBlock(self, method, args);
                return Ref<Object>();
            }
            
            // See if the object has a primitive bound to that name.
            PrimitiveMethod primitive = receiver->FindPrimitive(messageId);
            if (primitive != NULL)
            {
                return primitive(*this, self, args);
            }
            
            // Not found yet, so walk up the parent chain until we bottom out
            // at Object.
            if (receiver == GetEnvironment().ObjectPrototype()) break;
            receiver = receiver->Parent();
        }
        
        // If we got here, the object didn't handle the message.
        String messageName = GetEnvironment().Strings().Find(messageId);
        String error = String::Format("Object '%s' did not handle message '%s'",
            receiver->AsString().CString(), messageName.CString());
        Error(error);

        // Unhandled messages just return nil.
        return GetEnvironment().Nil();
    }
    
    Ref<Object> Fiber::Self()
    {
        /*
        // Within an object literal, self evaluates to the object.
        if (mObjects.Count() > 0) return mObjects.Peek();
        */
        
        return mCallFrames.Peek().receiver;
    }
    
    /*

    Ref<Object> Fiber::Execute()
    {
        mIsRunning = true;

        // continue processing bytecode until the entire callstack has completed
        // or this fiber gets paused (to switch to another fiber)
        while (mIsRunning && mCallStack.Count() > 0)
        {
            CallFrame & frame = mCallStack.Peek();
            const Instruction & instruction = (*frame.Block().Code())[frame.address];

            // advance past the instruction
            frame.address++;

            switch (instruction.op)
            {
                case OP_NOTHING:
                    // do nothing
                    break;

                case OP_BLOCK:
                    {
                        // capture the current scope
                        Ref<Scope> closure = frame.scope;

                        Ref<CodeBlock> code = frame.Block().Code()->GetCodeBlock(instruction.arg.id);
                        Ref<Object> block = Object::NewBlock(mEnvironment, code,
                                closure, Self());

                        PushOperand(block);
                    }
                    break;

                case OP_CONSTANT:
                    PushOperand(frame.Block().Code()->GetConstant(instruction.arg.id));
                    break;
                    
                case OP_CREATE_ARRAY:
                    {
                        // create the array
                        Ref<Object> object = Object::NewArray(mEnvironment, 0);
                        ArrayObject * array = object->AsArray();

                        // pop the elements
                        Array<Ref<Object> > elements;
                        for (int i = 0; i < instruction.arg.id; i++)
                        {
                            array->Elements().Add(PopOperand());
                        }

                        // reverse them since the stack has them in order (so
                        // that elements are evaluated from left to right) and
                        // popping reverses the order
                        array->Elements().Reverse();

                        // return the array
                        PushOperand(object);
                    }
                    break;

                case OP_POP:
                    PopOperand();
                    break;

                case OP_DUP:
                    PushOperand(mOperands.Peek());
                    break;

                case OP_DEF_LOCAL:
                    {
                        // def returns the defined value, so instead of popping
                        // and then pushing the value back on the stack, we'll
                        // just peek
                        Ref<Object> value = mOperands.Peek();
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        CurrentScope()->Define(name, value);
                    }
                    break;
                    
                case OP_DEF_FIELD:
                    {
                        // def returns the defined value, so instead of popping
                        // and then pushing the value back on the stack, we'll
                        // just peek
                        Ref<Object> value = mOperands.Peek();
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        if (!Self().IsNull())
                        {
                            Self()->ObjectScope()->Define(name, value);
                        }
                    }
                    break;


                case OP_UNDEF_LOCAL:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        CurrentScope()->Undefine(name);
                        PushNil();
                    }
                    break;
                    
                case OP_UNDEF_FIELD:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        if (!Self().IsNull())
                        {
                            Self()->ObjectScope()->Undefine(name);
                        }
                        PushNil();
                    }
                    break;
                    
                case OP_SET_LOCAL:
                    {
                        // set returns the assigned value, so instead of popping
                        // and then pushing the value back on the stack, we'll
                        // just peek
                        Ref<Object> value = mOperands.Peek();
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        CurrentScope()->Set(name, value);
                    }
                    break;
                    
                case OP_SET_FIELD:
                    {
                        // set returns the assigned value, so instead of popping
                        // and then pushing the value back on the stack, we'll
                        // just peek
                        Ref<Object> value = mOperands.Peek();
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        Self()->ObjectScope()->Set(name, value);
                    }
                    break;
                    
                case OP_LOAD_LOCAL:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        Ref<Object> value = CurrentScope()->LookUp(name);
                        PushOperand(value.IsNull() ? mEnvironment.Nil() : value);
                    }
                    break;
                    
                case OP_LOAD_FIELD:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        if (!Self().IsNull())
                        {
                            Ref<Object> value = Self()->ObjectScope()->LookUp(name);
                            PushOperand(value.IsNull() ? mEnvironment.Nil() : value);
                        }
                        else
                        {
                            PushOperand(Ref<Object>());
                        }
                    }
                    break;

                case OP_LOAD_SELF:
                    PushOperand(Self());
                    break;

                case OP_BIND_METHOD:
                    {
                        // The id is the string table index of the name and the
                        // code table index of the compiled body.
                        int nameId = (instruction.arg.id & 0xffff0000) >> 16;
                        int bodyId = instruction.arg.id & 0xffff;

                        String name = mEnvironment.Strings().Find(nameId);
                        Ref<CodeBlock> code = frame.Block().Code()->GetCodeBlock(bodyId);

                        // Capture the current scope.
                        Ref<Scope> closure = frame.scope;
                                                
                        // Get the object we're attaching the method to. Don't
                        // pop since the target is the result of a bind.
                        Ref<Object> target = mOperands.Peek();
                        DynamicObject * object = target->AsDynamic();
                        ASSERT_NOT_NULL(object);

                        object->AddMethod(target, *this, name, closure, code);
                    }
                    break;

                case OP_BIND_OBJECT:
                    {
                        // The stack has the target of the object variable and
                        // the value. The name is in the instruction.
                        Ref<Object> value = PopOperand();
                        
                        // Don't pop since the target is the result of a bind.
                        Ref<Object> object = mOperands.Peek();
                        String name = mEnvironment.Strings().Find(instruction.arg.id);

                        object->ObjectScope()->Define(name, value);
                    }
                    break;

                case OP_START_OBJECT:
                    {
                        Ref<Object> parent = PopOperand();
                        Ref<Object> object = Object::NewObject(parent);
                        mObjects.Push(object);
                        PushOperand(object);
                    }
                    break;

                case OP_END_OBJECT:
                    mObjects.Pop();
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
                    SendMessage(instruction.arg.id, instruction.op - OP_MESSAGE_0);
                    break;

                case OP_TAIL_MESSAGE_0:
                case OP_TAIL_MESSAGE_1:
                case OP_TAIL_MESSAGE_2:
                case OP_TAIL_MESSAGE_3:
                case OP_TAIL_MESSAGE_4:
                case OP_TAIL_MESSAGE_5:
                case OP_TAIL_MESSAGE_6:
                case OP_TAIL_MESSAGE_7:
                case OP_TAIL_MESSAGE_8:
                case OP_TAIL_MESSAGE_9:
                case OP_TAIL_MESSAGE_10:
                    // don't do tail call elimination on the very last call
                    // frame. otherwise, the interpreter will think this fiber
                    // has completed when it may have just done a 'yield' as a
                    // tail call and be waiting to resume.
                    if (mCallStack.Count() > 1)
                    {
                        // eliminate the current frame first
                        mCallStack.Pop();
                    }
                    
                    SendMessage(instruction.arg.id, instruction.op - OP_TAIL_MESSAGE_0);
                    break;

                case OP_RETURN:
                    {
                        int methodId = instruction.arg.id;

                        // The return value is on top of the stack, but we may
                        // need to discard some operands under it, so pull it
                        // off.
                        Ref<Object> result = PopOperand();
                        
                        // find the enclosing method on the callstack
                        int frame;
                        for (frame = 0; frame < mCallStack.Count(); frame++) {
                            if (mCallStack[frame].Block().Code()->MethodId() == methodId) {
                                // found it
                                break;
                            }
                        }

                        if (frame == mCallStack.Count()) {
                            Error("Cannot return from a block whose enclosing method has already returned.");
                            // unwind the whole stack now and bail
                            frame = mCallStack.Count() - 1;
                        }

                        // unwind until we reach the method
                        while (frame >= 0) {
                            // Discard any leftover operands that this call
                            // frame pushed.
                            int numOperands = mCallStack.Peek().numOperands;
                            while (mOperands.Count() > numOperands)
                            {
                                PopOperand();
                            }
                            
                            mCallStack.Pop();
                            frame--;
                        }
                        
                        // Restore the return value so that the code that called
                        // this method can get it.
                        PushOperand(result);
                    }
                    break;

                case OP_END_BLOCK:
                    mCallStack.Pop();
                    break;

                default:
                    ASSERT(false, "Unknown op code.");
            }
        }
        
        // the last operation the fiber performed leaves its result on
        // the stack. that's the result of executing the fiber's block.
        if (IsDone())
        {
            return PopOperand();
        }

        return Ref<Object>();
    }

     */

    Ref<Object> Fiber::Nil()
    {
        return GetEnvironment().Nil();
    }

    Ref<Object> Fiber::CreateBool(bool value)
    {
        return value ? GetEnvironment().True() : GetEnvironment().False();
    }
    
    Ref<Object> Fiber::CreateNumber(double value)
    {
        return Object::NewNumber(GetEnvironment(), value);
    }

    Ref<Object> Fiber::CreateString(const String & value)
    {
        return Object::NewString(GetEnvironment(), value);
    }

    void Fiber::CallBlock(Ref<Object> receiver, Ref<Object> blockObj, ArgReader & args)
    {
        BlockObject & block = *(blockObj->AsBlock());
        
        // TODO(bob): Need to handle binding self.
        // TODO(bob): Need to handle param/arg count mismatch.
        
        // Allocate this frame's registers.
        // TODO(bob): Make this a single operation on Array.
        while (mStack.Count() < args.GetStackStart() + block.GetNumRegisters())
        {
            mStack.Add(Ref<Object>());
        }
        
        // If there aren't enough arguments, nil out the remaining parameters.
        for (int i = args.GetNumArgs(); i < block.GetNumParams(); i++)
        {
            mStack[args.GetStackStart() + i] = Nil();
        }
        
        mCallFrames.Push(CallFrame(args.GetStackStart(), receiver, blockObj));
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
        // See if we already have an open upvalue for that variable.
        // TODO(bob): Shouldn't use an Array for this. Too slow to remove.
        // Instead, make Upvalue a linked list.
        for (int i = 0; i < mOpenUpvalues.Count(); i++)
        {
            if (mOpenUpvalues[i]->Index() == stackIndex)
            {
                return mOpenUpvalues[i];
            }
        }
        
        // Not closed over already, so create it.
        Ref<Upvalue> upvalue = Ref<Upvalue>(new Upvalue(stackIndex));
        mOpenUpvalues.Add(upvalue);
        return upvalue;
    }
    
#ifdef TRACE_INSTRUCTIONS
    void Fiber::TraceInstruction(Instruction instruction)
    {
        using namespace std;
        
        OpCode op = static_cast<OpCode>((instruction & 0xff000000) >> 24);
        int a = (instruction & 0x00ff0000) >> 16;
        int b = (instruction & 0x0000ff00) >> 8;
        int c = instruction & 0x000000ff;
        
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
                action = String::Format("g%d -> %d", a, b);
                break;
            }
                
            case OP_SET_GLOBAL:
            {
                opName = "SET_GLOBAL";
                action = String::Format("g%d <- %d", a, b);
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
                
            case OP_RETURN:
                opName = "RETURN";
                action = String::Format("%d", a);
                break;
                
            default:
                opName = String::Format("UNKNOWN OP(%d)", op);
                action = "";
        }
        
        cout << String::Format("%-14s %-20s  ", opName.CString(), action.CString());
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

            cout << left << setw(10) << mStack[i];
            
        }
        cout << endl;
    }
#endif
}

