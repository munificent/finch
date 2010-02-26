#include "BlockTable.h"
#include "Compiler.h"
#include "Expr.h"

namespace Finch
{
    int BlockTable::Add(const Array<String> & params, const Expr & body,
                        Environment & environment)
    {
        Ref<CodeBlock> code = Ref<CodeBlock>(new CodeBlock(params));
        Compiler::Compile(environment, body, *code);
        
        //### bob: doesn't actually check for duplicates yet
        mBlocks.Add(code);
        
        return mBlocks.Count() - 1;
    }
    
    CodeBlock & BlockTable::Find(int id)
    {
        return *mBlocks[id];
    }
}
