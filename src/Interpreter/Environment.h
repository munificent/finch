#pragma once

#include <iostream>

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "Scope.h"
#include "FinchString.h"
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
        
        StringTable & Strings() { return mStrings; }
                
        // Get the core built-in objects.
        Ref<Object> ObjectPrototype()   const { return mObject; }
        Ref<Object> ArrayPrototype()    const { return mArrayPrototype; }
        Ref<Object> BlockPrototype()    const { return mBlockPrototype; }
        Ref<Object> FiberPrototype()    const { return mFiberPrototype; }
        Ref<Object> NumberPrototype()   const { return mNumberPrototype; }
        Ref<Object> StringPrototype()   const { return mStringPrototype; }
        Ref<Object> Nil()               const { return mNil; }
        Ref<Object> True()              const { return mTrue; }
        Ref<Object> False()             const { return mFalse; }
        
        // Creates an indexed slot for a global with the given name. If a global
        // with that name already exists, will return that slot.
        int DefineGlobal(const String & name);
        Ref<Object> GetGlobal(int index);
        void SetGlobal(int index, Ref<Object> value);
        
        String FindGlobalName(int index);
        
        Ref<Object> CreateBlock(Ref<Expr> expr);
    private:
        StringTable mStrings;
        
        // Indexed collection of global variables.
        Array<Ref<Object> > mGlobals;
        // Maps global variable names to their indices. Used by the compiler.
        IdTable<int> mGlobalNames;
        
        Ref<Object> mObject;
        Ref<Object> mArrayPrototype;
        Ref<Object> mBlockPrototype;
        Ref<Object> mFiberPrototype;
        Ref<Object> mNumberPrototype;
        Ref<Object> mStringPrototype;
        Ref<Object> mNil;
        Ref<Object> mTrue;
        Ref<Object> mFalse;
        
        Ref<Object> MakeGlobal(const char * name);
        void AddPrimitive(Ref<Object> object, String message,
                          PrimitiveMethod primitive);
        
        NO_COPY(Environment);
    };
}

