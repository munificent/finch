#include "CodeBlock.h"
#include "Environment.h"
#include "Interpreter.h"

namespace Finch
{
    void Interpreter::Execute(Environment & environment, const CodeBlock & code)
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
                    
                case OP_POP:
                    mStack.Pop();
                    break;
                    
                default:
                    ASSERT(false, "Unknown op code.");
            }
            
            // advance to the next instruction
            address++;
        }
    }
}