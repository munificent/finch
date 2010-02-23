#include <iostream>

#include "BlockObject.h"
#include "CodeBlock.h"
#include "Environment.h"
#include "Interpreter.h"

namespace Finch
{
    using std::cout;
    using std::endl;
    
    Interpreter::Interpreter(Environment & environment)
    :   mIsRunning(true),
        mEnvironment(environment)
    {}

    Ref<Object> Interpreter::Execute(const CodeBlock & code)
    {
        // push the starting block
        mCallStack.Push(CallFrame(&code, mEnvironment.Globals(), mEnvironment.Nil()));
        
        // continue processing bytecode until the entire callstack has
        // completed
        while (mCallStack.Count() > 0)
        {
            CallFrame & frame = mCallStack.Peek();
            const Instruction & instruction = (*frame.code)[frame.address];
            
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
                        Ref<Scope> closure = mCallStack.Peek().scope;
                        
                        const CodeBlock & code = mEnvironment.Blocks().Find(instruction.arg.id);
                        Ref<Object> block = Object::NewBlock(mEnvironment, code, closure);
                        PushOperand(block);
                    }
                    break;
                    
                case OP_POP:
                    PopOperand();
                    break;
                    
                case OP_DEF_GLOBAL:
                    {
                        // def returns the defined value, so instead of popping
                        // and then pushing the value back on the stack, we'll
                        // just peek
                        Ref<Object> value = mOperands.Peek();
                        //### bob: if we get strings fully interned (i.e. no dupes in
                        // string table), then the global name scope doesn't need the
                        // actual string at all, just the id in the string table
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        mEnvironment.Globals()->Define(name, value);                        
                    }
                    break;
                    
                case OP_DEF_OBJECT:
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
                    
                case OP_SET_LOCAL:
                    {
                        // def returns the defined value, so instead of popping
                        // and then pushing the value back on the stack, we'll
                        // just peek
                        Ref<Object> value = mOperands.Peek();
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        CurrentScope()->Set(name, value);
                    }
                    break;
                    
                case OP_LOAD_GLOBAL:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        Ref<Object> value = mEnvironment.Globals()->LookUp(name);
                        PushOperand(value.IsNull() ? mEnvironment.Nil() : value);
                    }
                    break;
                    
                case OP_LOAD_OBJECT:
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
                    
                case OP_LOAD_LOCAL:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        Ref<Object> value = CurrentScope()->LookUp(name);
                        PushOperand(value.IsNull() ? mEnvironment.Nil() : value);
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
                    {
                        // pop the arguments
                        vector<Ref<Object> > args;
                        for (int i = 0; i < instruction.op - OP_MESSAGE_0; i++)
                        {
                            args.push_back(PopOperand());
                        }
                        
                        // reverse them since the stack has them in order (so
                        // that arguments are evaluated from left to right) and
                        // popping reverses the order
                        reverse(args.begin(), args.end());
                        
                        // send the message
                        String string = mEnvironment.Strings().Find(instruction.arg.id);
                        Ref<Object> receiver = PopOperand();
                        
                        receiver->Receive(receiver, *this, string, args);
                    }
                    break;
                    
                case OP_END_BLOCK:
                    mCallStack.Pop();
                    break;
                    
                default:
                    ASSERT(false, "Unknown op code.");
            }
            
            // advance to the next instruction
            frame.address++;
        }
        
        // there should be one object left on the stack: the final return
        return PopOperand();
    }
    
    void Interpreter::Push(Ref<Object> object)
    {
        PushOperand(object);
    }
    
    void Interpreter::PushNil()
    {
        Push(mEnvironment.Nil());
    }

    void Interpreter::PushBool(bool value)
    {
        PushOperand(value ? mEnvironment.True() : mEnvironment.False());
    }

    void Interpreter::PushNumber(double value)
    {
        Push(Object::NewNumber(mEnvironment, value));
    }

    void Interpreter::CallBlock(const BlockObject & block,
                                const vector<Ref<Object> > & args)
    {
        // continue using the current self object
        Ref<Object> self = mCallStack.Peek().self;
        
        CallMethod(self, block, args);
    }
    
    void Interpreter::CallMethod(Ref<Object> self,
                                 const BlockObject & block,
                                 const vector<Ref<Object> > & args)
    {
        // make sure we have the right number of arguments
        //### bob: could change to ignore extra args and pad missing ones with
        // nil if we want to be "looser" about calling convention
        if (block.Params().size() != args.size())
        {
            RuntimeError(String::Format("Block expects %d arguments, but was passed %d.",
                                        block.Params().size(), args.size()));
            PushNil();
            return;
        }
        
        // create a new local scope for the block
        Ref<Scope> scope = Ref<Scope>(new Scope(block.Closure()));
        
        // bind the arguments
        for (unsigned int i = 0; i < args.size(); i++)
        {
            scope->Define(block.Params()[i], args[i]);
        }
        
        // push the call onto the stack
        mCallStack.Push(CallFrame(&block.GetCode(), scope, self));
    }
    
    void Interpreter::RuntimeError(const String & message)
    {
        //### bob: ideally, this should be programmatically configurable from
        // within Finch
        cout << "Runtime error: " << message << endl;
    }
    
    void Interpreter::PushOperand(Ref<Object> object)
    {
        ASSERT(!object.IsNull(), "Cannot push a null object. (Should be Nil instead.)");
        
        //std::cout << "push " << object << std::endl;
        mOperands.Push(object);
    }
    
    Ref<Object> Interpreter::PopOperand()
    {
        Ref<Object> object = mOperands.Pop();
        
        //std::cout << "pop  " << object << std::endl;
        return object;
    }
}