#pragma once

#include <vector>

#include "CodeBlock.h"
#include "Ref.h"

namespace Finch
{
    using std::vector;
    
    class Environment;
    class Expr;
    
    //### bob: the implementation of this just a temp prototype.
    class BlockTable
    {
    public:
        // Adds the given block to the table if not already present, and
        // returns its ID.
        int Add(const vector<String> & params, const Expr & body, Environment & environment);
        
        // Looks up the block with the given ID in the table.
        CodeBlock & Find(int id);
        
    private:
        static const int MAX_BLOCKS = 4096; //### bob: arbitrary
        
        Ref<CodeBlock>  mBlocks[MAX_BLOCKS];
        int             mNumBlocks;
    };
}