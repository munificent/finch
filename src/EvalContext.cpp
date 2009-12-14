#include "EvalContext.h"

#include "Evaluator.h"
#include "Expr.h"

namespace Finch
{
    EvalContext::EvalContext()
    {
        // build the global scope
        mGlobals = Ref<Scope>(new Scope()); 
        mCurrentScope = mGlobals;
        
        Ref<Object> rootObject = Object::New(Ref<Object>(), "Object");
        mGlobals->Define("Object", rootObject);
        
        mNil = Object::New(rootObject, "Nil");
        mGlobals->Define("Nil", mNil);
        
        mTrue = Object::New(rootObject, "True");
        mGlobals->Define("True", mTrue);
        
        mFalse = Object::New(rootObject, "False");
        mGlobals->Define("False", mFalse);
    }
    
    Ref<Object> EvalContext::EvaluateBlock(Ref<Expr> expr)
    {
        mCurrentScope = Ref<Scope>(new Scope(mCurrentScope));
        
        Evaluator evaluator(*this);
        Ref<Object> result = evaluator.Evaluate(expr);

        mCurrentScope = mCurrentScope->Parent();
        
        return result;
    }
    
    Ref<Object> EvalContext::EvaluateMethod(Ref<Object> self, Ref<Expr> expr)
    {
        Ref<Object> previousSelf = mSelf;
        mSelf = self;
        
        Evaluator evaluator(*this);
        Ref<Object> result = evaluator.Evaluate(expr);
        
        mSelf = previousSelf;
        
        return result;
    }
}