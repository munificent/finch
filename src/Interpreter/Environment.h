#pragma once

#include <iostream>

#include "BlockTable.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "Scope.h"
#include "String.h"
#include "StringTable.h"

namespace Finch
{
    class BlockObject;
    class Expr;
    
    // Represents the global runtime environment. Code is executed withing the
    // context of this object. It provides access to global variables as well
    // as tracking the current local scope and other contextual information.
    class Environment
    {
    public:
        Environment();
        
        BlockTable &  Blocks()  { return mBlocks; }
        StringTable & Strings() { return mStrings; }
        
        Ref<Scope>  Globals()       const { return mGlobals; }

        // Get the core built-in prototype objects.
        Ref<Object> ObjectPrototype()   const { return mObjectPrototype; }
        Ref<Object> ArrayPrototype()    const { return mArrayPrototype; }
        Ref<Object> BlockPrototype()    const { return mBlockPrototype; }
        Ref<Object> NumberPrototype()   const { return mNumberPrototype; }
        Ref<Object> StringPrototype()   const { return mStringPrototype; }
        Ref<Object> Nil()               const { return mNil; }
        Ref<Object> True()              const { return mTrue; }
        Ref<Object> False()             const { return mFalse; }
        
    private:
        
        BlockTable  mBlocks;
        StringTable mStrings;
        
        Ref<Scope> mGlobals;
		Ref<Object> mObjectPrototype;
        Ref<Object> mArrayPrototype;
        Ref<Object> mBlockPrototype;
        Ref<Object> mNumberPrototype;
        Ref<Object> mStringPrototype;
        Ref<Object> mNil;
        Ref<Object> mTrue;
        Ref<Object> mFalse;
        
        NO_COPY(Environment);
    };
}

