#include <iostream>

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
        mEnvironment(interpreter.GetEnvironment())
    {
        // push the starting block
        // when not in any method, self is Nil
        CallMethod(mEnvironment.Nil(), block, Array<Ref<Object> >());
    }

    bool Fiber::IsDone() const
    {
        return mCallStack.Count() == 0;
    }

    Ref<Object> Fiber::Execute()
    {
        mIsRunning = true;

        // continue processing bytecode until the entire callstack has completed
        // or this fiber gets paused (to switch to another fiber)
        while (mIsRunning && mCallStack.Count() > 0)
        {
            CallFrame & frame = mCallStack.Peek();
            const Instruction & instruction = frame.Block().GetCode()[frame.address];

            // advance past the instruction
            frame.address++;

            switch (instruction.op)
            {
                case OP_NOTHING:
                    // do nothing
                    break;

                case OP_NUMBER_LITERAL:
                    PushOperand(Object::NewNumber(mEnvironment, instruction.arg.number));
                    break;

                case OP_STRING_LITERAL:
                    {
                        String string = mEnvironment.Strings().Find(instruction.arg.id);
                        PushOperand(Object::NewString(mEnvironment, string));
                    }
                    break;

                case OP_BLOCK_LITERAL:
                    {
                        // capture the current scope
                        Ref<Scope> closure = frame.scope;

                        const CodeBlock & code = mEnvironment.Blocks().Find(instruction.arg.id);
                        Ref<Object> block = Object::NewBlock(mEnvironment, code,
                                closure, Self());

                        PushOperand(block);
                    }
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
                        Push(object);
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
                        // the stack has the target of the method and the body.
                        // the name is in the instruction.
                        Ref<Object> body = mOperands.Pop();
                        Ref<Object> target = mOperands.Pop();
                        String name = mEnvironment.Strings().Find(instruction.arg.id);

                        DynamicObject * object = target->AsDynamic();
                        ASSERT_NOT_NULL(object);

                        object->AddMethod(target, *this, name, body);
                        // note: does not push a result. the object the
                        // definition is being applied to will still be on the
                        // stack after this and is the result.
                    }
                    break;

                case OP_BIND_OBJECT:
                    {
                        // the stack has the target of the object variable and
                        // the value.
                        // the name is in the instruction.
                        Ref<Object> value = mOperands.Pop();
                        Ref<Object> object = mOperands.Pop();
                        String name = mEnvironment.Strings().Find(instruction.arg.id);

                        object->ObjectScope()->Define(name, value);
                        // note: does not push a result. the object the
                        // definition is being applied to will still be on the
                        // stack after this and is the result.
                    }
                    break;

                case OP_MAKE_OBJECT:
                    {
                        Ref<Object> parent = mOperands.Pop();
                        PushOperand(Object::NewObject(parent));
                    }
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

                        // find the enclosing method on the callstack
                        int frame;
                        for (frame = 0; frame < mCallStack.Count(); frame++) {
                            if (mCallStack[frame].Block().GetCode().MethodId() == methodId) {
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
                            mCallStack.Pop();
                            frame--;
                        }
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

    void Fiber::Push(Ref<Object> object)
    {
        PushOperand(object);
    }

    void Fiber::PushNil()
    {
        Push(mEnvironment.Nil());
    }

    void Fiber::PushBool(bool value)
    {
        PushOperand(value ? mEnvironment.True() : mEnvironment.False());
    }

    void Fiber::PushNumber(double value)
    {
        Push(Object::NewNumber(mEnvironment, value));
    }

    void Fiber::PushString(const String & value)
    {
        Push(Object::NewString(mEnvironment, value));
    }

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
        mCallStack.Push(CallFrame(scope, blockObj, self));
    }

    void Fiber::CallBlock(Ref<Object> blockObj,
                                const Array<Ref<Object> > & args)
    {
        BlockObject & block = *(blockObj->AsBlock());
        CallMethod(block.Self(), blockObj, args);
    }

    void Fiber::Error(const String & message)
    {
        mInterpreter.GetHost().Error(message);
    }

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

        receiver->Receive(receiver, *this, string, args);
    }

    Ref<Object> Fiber::Self()
    {
        return mCallStack.Peek().receiver;
    }

    void Fiber::PushOperand(Ref<Object> object)
    {
        ASSERT(!object.IsNull(), "Cannot push a null object. (Should be Nil instead.)");

        //std::cout << "push " << object << std::endl;
        mOperands.Push(object);
    }

    Ref<Object> Fiber::PopOperand()
    {
        Ref<Object> object = mOperands.Pop();

        //std::cout << "pop  " << object << std::endl;
        return object;
    }
}

