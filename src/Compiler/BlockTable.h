#pragma once

#include "Array.h"
#include "CodeBlock.h"
#include "Ref.h"

namespace Finch
{
    class Environment;
    class Expr;
    
    // A table of CodeBlocks. The compiler adds compiled code to this so that
    // a block can be referenced by ID.
    class BlockTable
    {
    public:
        // Adds the given block to the table if not already present, and
        // returns its ID.
        int Add(const Array<String> & params, const Expr & body, Environment & environment);
        
        // Looks up the block with the given ID in the table.
        CodeBlock & Find(int id);
        
    private:
        Array<Ref<CodeBlock> > mBlocks;
    };
}