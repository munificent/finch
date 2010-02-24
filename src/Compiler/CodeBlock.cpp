#include "CodeBlock.h"

namespace Finch
{
    CodeBlock::CodeBlock(const vector<String> & params, int capacity)
    :   mParams(params),
        mCapacity(capacity),
        mSize(0)
    {
        mInstructions = new Instruction[capacity];
    }
    
    CodeBlock::~CodeBlock()
    {
        //### bob: may need to decrement ref count or gc references to
        // objects in instructions at some point
        delete [] mInstructions;
    }

    void CodeBlock::Write(OpCode op)
    {
        ASSERT(mSize < mCapacity, "Code block is full.");
        
        mInstructions[mSize].op = op;
        mSize++;
    }
    
    void CodeBlock::Write(OpCode op, double number)
    {
        ASSERT(mSize < mCapacity, "Code block is full.");
        
        mInstructions[mSize].op = op;
        mInstructions[mSize].arg.number = number;
        mSize++;
    }
    
    void CodeBlock::Write(OpCode op, int id)
    {
        ASSERT(mSize < mCapacity, "Code block is full.");
        
        mInstructions[mSize].op = op;
        mInstructions[mSize].arg.id = id;
        mSize++;
    }
}