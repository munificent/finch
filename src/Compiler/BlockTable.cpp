#include "BlockTable.h"
#include "Compiler.h"
#include "Expr.h"

namespace Finch
{
    int BlockTable::Add(const Array<String> & params, const Expr & body,
                        Environment & environment)
    {
        Ref<CodeBlock> code = Ref<CodeBlock>(new CodeBlock(params, 0));
        Compiler::Compile(environment, body, *code, 0);
        
        return Add(code);
    }
    
    int BlockTable::Add(Ref<CodeBlock> code)
    {
        //### bob: doesn't actually check for duplicates yet
        mBlocks.Add(code);
        
        return mBlocks.Count() - 1;
    }
    
    CodeBlock & BlockTable::Find(int id)
    {
        return *mBlocks[id];
    }
}

