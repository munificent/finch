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
            
            switch (op)
            {
                case OP_CONSTANT:
                    //cout << "CONSTANT " << a << " -> " << b << endl;
                    Store(frame, b, frame.Block().GetConstant(a));
                    TraceStack();
                    break;
                
                case OP_OBJECT:
                {
                    // TODO(bob): Support parents.
                    Ref<Object> parent = GetEnvironment().ObjectPrototype();
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
                    Store(frame, b, block);
                    TraceStack();
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
                    TraceStack();
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
                    TraceStack();
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
                    
                case OP_DEFINE_METHOD:
                {
                    Ref<BlockExemplar> exemplar = frame.Block().GetExemplar(b);
                    
                    // TODO(bob): Lot of overlap here with OP_BLOCK.
                    // TODO(bob): Capture closure.
                    // TODO(bob): Should use enclosing self instead of Nil.
                    Ref<Object> body = Object::NewBlock(GetEnvironment(),
                        exemplar, GetEnvironment().Nil());
                    // TODO(bob): Capture upvals.
                    
                    // Get the object we're attaching the method to.
                    DynamicObject * object = Load(frame, c)->AsDynamic();
                    // TODO(bob): What should this do if you try to bind a
                    // method to something non-dynamic?
                    ASSERT_NOT_NULL(object);
                    
                    object->AddMethod(a, body);
                    break;
                }
                    
                case OP_DEFINE_FIELD:
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
                    
                    mCallFrames.Pop();
                    
                    if (mCallFrames.Count() > 0)
                    {
                        // Discard the callee frame's registers.
                        CallFrame & caller = mCallFrames.Peek();
                        int newStackSize = caller.stackStart +
                            caller.Block().GetNumRegisters();
                        
                        // TODO(bob): Make this a single array op.
                        while (mStack.Count() > newStackSize)
                        {
                            mStack.RemoveAt(mStack.Count() - 1);
                        }
                        
                        // Store the result back in the caller's dest register.
                        Instruction messageInstruction = caller.Block().GetCode()[caller.ip - 1];
                        int dest = messageInstruction & 0x000000ff; // c
                        Store(caller, dest, result);
                        TraceStack();
                    }
                    else
                    {
                        TraceStack();
                        return result;
                    }

                    break;
                }
                    
                default:
                    ASSERT(false, "Unknown opcode.");
            }
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
        
        // Walk the parent chain looking for a method that matches the message.
        while (true)
        {
            // See if the object has a method bound to that name.
            Ref<Object> method = receiver->FindMethod(messageId);
            if (!method.IsNull())
            {
                // TODO(bob): Handle binding self.
                ArgReader args(mStack, receiverReg + 1, numArgs);
                CallBlock(self, method, args);
                return Ref<Object>();
            }
            
            // See if the object has a primitive bound to that name.
            PrimitiveMethod primitive = receiver->FindPrimitive(messageId);
            if (primitive != NULL)
            {
                ArgReader args(mStack, receiverReg + 1, numArgs);
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
    Fiber::Fiber(Interpreter & interpreter, Ref<Object> block)
    :   mIsRunning(false),
        mInterpreter(interpreter),
        mEnvironment(interpreter.GetEnvironment())
    {
        // push the starting block
        // when not in any method, self is Nil
        CallMethod(mEnvironment.Nil(), block, Array<Ref<Object> >());
    }

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

    /*
    void Fiber::CallMethod(Ref<Object> self, Ref<Object> blockObj,
                                 const Array<Ref<Object> > & args)
    {
        BlockObject & block = *(blockObj->AsBlock());

        // create a new local scope for the block
        Ref<Scope> scope;
        if (block.Closure().IsNull())
        {
            // if the block doesn't have a closure, that means its a top-level
            // one. in that case, we use global directly (instead of a *child* 
            // of global scope) so that variables defined in it are shared with
            // other top-level blocks.
            scope = mEnvironment.Globals();
        }
        else
        {
            scope = Ref<Scope>(new Scope(block.Closure()));
        }

        // bind the arguments to the parameters. missing arguments will be
        // filled with Nil, and extra arguments will be ignored.
        for (int i = 0; i < block.Params().Count(); i++)
        {
            Ref<Object> arg = (i < args.Count()) ? args[i] : mEnvironment.Nil();
            scope->Define(block.Params()[i], arg);
        }

        // push the call onto the stack
        mCallStack.Push(CallFrame(scope, blockObj, self, mOperands.Count()));
    }
*/
    
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
        
        mCallFrames.Push(CallFrame(args.GetStackStart(), receiver, blockObj));
    }


    void Fiber::Error(const String & message)
    {
        mInterpreter.GetHost().Error(message);
    }
    
    void Fiber::TraceStack() const
    {
        // Uncomment to trace ops.
        // TODO(bob): Set up a nice #define to enable this.
        /*
        using namespace std;
        
        int j = mCallFrames.Count() - 1;
        for (int i = 0; i < mStack.Count(); i++)
        {
            if (j >= 0 && mCallFrames[j].stackStart == i)
            {
                cout << " || ";
                j--;
            }
            else
            {
                cout << " | ";
            }

            cout << left << setw(10) << mStack[i];
            
        }
        cout << endl;
        */
    }
    
    /*
    int Fiber::GetCallstackDepth() const
    {
        return mCallStack.Count();
    }

    void Fiber::SendMessage(int message, int numArgs)
    {
        // pop the arguments. note that we fill the array from back to front
        // because the arguments are on the stack from first to last (so that they
        // were correctly evaluated from left to right) and now we're popping them
        // off from last to first.
        Array<Ref<Object> > args(numArgs, mEnvironment.Nil());
        for (int i = numArgs - 1; i >= 0; i--)
        {
            args[i] = PopOperand();
        }

        // send the message
        String string = mEnvironment.Strings().Find(message);
        Ref<Object> receiver = PopOperand();

        //std::cout << "send '" << string << "' to " << receiver << std::endl;
        
        receiver->Receive(receiver, *this, string, args);
    }
     */
}

