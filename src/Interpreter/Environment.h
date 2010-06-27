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

        //### bob: this needs to be cleaned up to make a distinction between the type objects
        // and the prototypes. a type object ("String") is a singleton object that basically
        // contain "static" methods on a type, including constructors. a prototype
        // ("String prototype") is the prototypical object that new instances of a type use as
        // their parent. 
        // Get the core built-in prototype objects.
        Ref<Object> ObjectPrototype()   const { return mObjectPrototype; }
        Ref<Object> Nil()               const { return mNil; }
        Ref<Object> ArrayPrototype()    const { return mArray; }
        Ref<Object> Block()             const { return mBlock; }
        Ref<Object> Number()            const { return mNumber; }
        Ref<Object> StringPrototype()   const { return mString; }
        Ref<Object> True()              const { return mTrue; }
        Ref<Object> False()             const { return mFalse; }
        
    private:
        
        BlockTable  mBlocks;
        StringTable mStrings;
        
        Ref<Scope> mGlobals;
		Ref<Object> mObjectPrototype;
        Ref<Object> mNil;
        Ref<Object> mArray;
        Ref<Object> mBlock;
        Ref<Object> mNumber;
        Ref<Object> mString;
        Ref<Object> mTrue;
        Ref<Object> mFalse;
        
        NO_COPY(Environment);
    };
}

