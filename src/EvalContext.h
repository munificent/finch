#pragma once

#include <iostream>

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "Scope.h"

namespace Finch
{
    class Expr;
    
    class EvalContext
    {
    public:
        EvalContext();
        
        Ref<Scope>  Globals()       const { return mGlobals; }
        Ref<Scope>  CurrentScope()  const { return mCurrentScope; }

        Ref<Object> Self()          const { return mSelf; }

        Ref<Object> Nil()           const { return mNil; }
        Ref<Object> True()          const { return mTrue; }
        Ref<Object> False()         const { return mFalse; }
        
        Ref<Object> EvaluateBlock(Ref<Expr> expr);
        Ref<Object> EvaluateMethod(Ref<Object> self, Ref<Expr> expr);
        
    private:
        Ref<Scope> mGlobals;
        Ref<Scope> mCurrentScope;
        Ref<Object> mSelf;
        Ref<Object> mNil;
        Ref<Object> mTrue;
        Ref<Object> mFalse;
    };
}