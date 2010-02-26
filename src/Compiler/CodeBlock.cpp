#include "CodeBlock.h"

namespace Finch
{
    CodeBlock::CodeBlock(const Array<String> & params)
    :   mParams(params),
        mInstructions()
    {
    }

    void CodeBlock::Write(OpCode op)
    {
        Write(op, 0);
    }
    
    void CodeBlock::Write(OpCode op, double number)
    {
        Instruction instruction;
        instruction.op = op;
        instruction.arg.number = number;
        
        mInstructions.Add(instruction);
    }
    
    void CodeBlock::Write(OpCode op, int id)
    {
        Instruction instruction;
        instruction.op = op;
        instruction.arg.id = id;
        
        mInstructions.Add(instruction);
    }
}