#include "BlockTable.h"
#include "Compiler.h"
#include "Expr.h"

namespace Finch
{
    int BlockTable::Add(const vector<String> & params, const Expr & body,
                        Environment & environment)
    {
        Ref<CodeBlock> code = Ref<CodeBlock>(new CodeBlock(params));
        Compiler::Compile(environment, body, *code);
        
        //### bob: doesn't actually check for duplicates yet
        mBlocks[mNumBlocks] = code;
        
        return mNumBlocks++;
    }
    
    CodeBlock & BlockTable::Find(int id)
    {
        ASSERT_INDEX(id, mNumBlocks);
        
        return *mBlocks[id];
    }
}
