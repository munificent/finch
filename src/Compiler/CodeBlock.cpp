#include "CodeBlock.h"

namespace Finch
{
    CodeBlock::CodeBlock(const Array<String> & params, int methodId)
    :   mParams(params),
        mMethodId(methodId),
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
    
    void CodeBlock::MarkTailCalls()
    {
        for (int i = 1; i < mInstructions.Count(); i++)
        {
            if ((mInstructions[i].op == OP_END_BLOCK) &&
                (mInstructions[i - 1].op >= OP_MESSAGE_0) &&
                (mInstructions[i - 1].op <= OP_MESSAGE_10))
            {
                int numArgs = mInstructions[i - 1].op - OP_MESSAGE_0;
                mInstructions[i - 1].op =
                    static_cast<OpCode>(OP_TAIL_MESSAGE_0 + numArgs);
            }
        }
    }
}

