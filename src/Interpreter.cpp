#include "CodeBlock.h"
#include "Environment.h"
#include "Interpreter.h"

namespace Finch
{
    Ref<Object> Interpreter::Execute(const CodeBlock & code)
    {
        int address = 0;
        
        while (address < code.Size())
        {
            const Instruction & instruction = code[address];
            
            switch (instruction.op)
            {
                case OP_NOTHING:
                    // do nothing
                    break;
                    
                case OP_NUMBER_LITERAL:
                    mStack.Push(Object::NewNumber(mEnvironment, instruction.arg.number));
                    break;
                    
                case OP_STRING_LITERAL:
                    {
                        String string = mEnvironment.Strings().Find(instruction.arg.id);
                        mStack.Push(Object::NewString(mEnvironment, string));
                    }
                    break;
                    
                case OP_BLOCK_LITERAL:
                    {
                        Ref<Object> block = mEnvironment.Blocks().Find(instruction.arg.id);
                        mStack.Push(block);
                    }
                    break;
                    
                case OP_POP:
                    mStack.Pop();
                    break;
                    
                case OP_DEF_GLOBAL:
                    {
                        Ref<Object> value = mStack.Pop();
                        //### bob: if we get strings fully interned (i.e. no dupes in
                        // string table), then the global name scope doesn't need the
                        // actual string at all, just the id in the string table
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        mEnvironment.Globals()->Define(name, value);                        
                        
                        // return the assigned value
                        mStack.Push(value);
                    }
                    break;
                    
                case OP_DEF_OBJECT:
                    {
                        Ref<Object> value = mStack.Pop();
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        if (!mEnvironment.Self().IsNull())
                        {
                            mEnvironment.Self()->ObjectScope()->Define(name, value);
                        }
                        
                        // return the assigned value
                        mStack.Push(value);
                    }
                    break;
                    
                case OP_LOAD_GLOBAL:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        mStack.Push(mEnvironment.Globals()->LookUp(name));
                    }
                    break;
                    
                case OP_LOAD_OBJECT:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        if (!mEnvironment.Self().IsNull())
                        {
                            mStack.Push(mEnvironment.Self()->ObjectScope()->LookUp(name));
                        }
                        else
                        {
                            mStack.Push(Ref<Object>());
                        }
                    }
                    break;
                    
                case OP_LOAD_LOCAL:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        mStack.Push(mEnvironment.CurrentScope()->LookUp(name));
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
                            args.push_back(mStack.Pop());
                        }
                        
                        // reverse them since the stack has them in order and popping
                        // reverses
                        reverse(args.begin(), args.end());
                        
                        // send the message
                        String string = mEnvironment.Strings().Find(instruction.arg.id);
                        Ref<Object> receiver = mStack.Pop();
                        Ref<Object> result = receiver->Receive(receiver, mEnvironment, string, args);
                        mStack.Push(result);
                    }
                    break;
                    
                default:
                    ASSERT(false, "Unknown op code.");
            }
            
            // advance to the next instruction
            address++;
        }
        
        // there should be one object left on the stack: the final return
        return mStack.Pop();
    }
}