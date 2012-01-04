#include "CodeBlock.h"

namespace Finch
{
    BlockExemplar::BlockExemplar(const Array<String> & params)
    :   mParams(params),
        mCode(),
        mConstants(),
        mNumRegisters(0)
    {
    }

    int BlockExemplar::AddConstant(Ref<Object> object)
    {
        // TODO(bob): Unify duplicates.
        mConstants.Add(object);
        return mConstants.Count() - 1;
    }

    // Writes an instruction.
    void BlockExemplar::Write(OpCode op, int a, int b, int c)
    {
        ASSERT_RANGE(a, 256);
        ASSERT_RANGE(b, 256);
        ASSERT_RANGE(c, 256);
        
        Instruction instruction = (op << 24) |
                                  ((a & 0xff) << 16) |
                                  ((b & 0xff) << 8) |
                                  (c & 0xff);
        
        mCode.Add(instruction);
    }

    /*
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
    
    int CodeBlock::AddConstant(Ref<Object> constant)
    {
        mConstants.Add(constant);
        return mConstants.Count() - 1;
    }
    
    int CodeBlock::AddCodeBlock(Ref<CodeBlock> codeBlock)
    {
        mCodeBlocks.Add(codeBlock);
        return mCodeBlocks.Count() - 1;
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
     */
}

