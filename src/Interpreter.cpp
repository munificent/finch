#include "CodeBlock.h"
#include "Environment.h"
#include "Interpreter.h"

namespace Finch
{
    Ref<Object> Interpreter::Execute(Environment & environment, const CodeBlock & code)
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
                    mStack.Push(Object::NewNumber(environment, instruction.arg.number));
                    break;
                    
                case OP_STRING_LITERAL:
                    {
                        String string = environment.Strings().Find(instruction.arg.id);
                        mStack.Push(Object::NewString(environment, string));
                    }
                    break;
                    
                case OP_BLOCK_LITERAL:
                    {
                        Ref<Object> block = environment.Blocks().Find(instruction.arg.id);
                        mStack.Push(block);
                    }
                    break;
                    
                case OP_POP:
                    mStack.Pop();
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
                        String string = environment.Strings().Find(instruction.arg.id);
                        Ref<Object> receiver = mStack.Pop();
                        Ref<Object> result = receiver->Receive(receiver, environment, string, args);
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