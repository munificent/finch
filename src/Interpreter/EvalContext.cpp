#include "EvalContext.h"

#include "Evaluator.h"
#include "Expr.h"
#include "DynamicObject.h"
#include "BlockPrimitives.h"
#include "ConsolePrimitives.h"
#include "NilPrimitives.h"
#include "NumberPrimitives.h"

namespace Finch
{
    EvalContext::EvalContext()
    {
        // build the global scope
        mGlobals = Ref<Scope>(new Scope()); 
        mCurrentScope = mGlobals;
        
        Ref<Object> rootObject = Object::New(Ref<Object>(), "Object");
        mGlobals->Define("Object", rootObject);
        
        // define Block prototype
        mBlock = Object::New(rootObject, "Block");
        mGlobals->Define("Block", mBlock);
        
        DynamicObject* blockObj = &static_cast<DynamicObject&>(*mBlock);
        blockObj->RegisterPrimitive("value", BlockValue);
        
        // define Number prototype
        mNumber = Object::New(rootObject, "Number");
        mGlobals->Define("Number", mNumber);
        
        DynamicObject* numberObj = &static_cast<DynamicObject&>(*mNumber);
        numberObj->RegisterPrimitive("abs", NumberAbs);
        numberObj->RegisterPrimitive("neg", NumberNeg);
        numberObj->RegisterPrimitive("+",   NumberPlus);
        numberObj->RegisterPrimitive("-",   NumberMinus);
        numberObj->RegisterPrimitive("*",   NumberMultiply);
        numberObj->RegisterPrimitive("/",   NumberDivide);
        numberObj->RegisterPrimitive("=",   NumberEquals);
        numberObj->RegisterPrimitive("!=",  NumberNotEquals);
        numberObj->RegisterPrimitive("<",   NumberLessThan);
        numberObj->RegisterPrimitive(">",   NumberGreaterThan);
        numberObj->RegisterPrimitive("<=",  NumberLessThanOrEqual);
        numberObj->RegisterPrimitive(">=",  NumberGreaterThanOrEqual);
        
        // define Symbol prototype
        mSymbol = Object::New(rootObject, "Symbol");
        mGlobals->Define("Symbol", mSymbol);
        
        // define String prototype
        mString = Object::New(rootObject, "String");
        mGlobals->Define("String", mString);
        
        // define nil
        mNil = Object::New(rootObject, "Nil");
        mGlobals->Define("Nil", mNil);
        
        DynamicObject* nilObj = &static_cast<DynamicObject&>(*mNil);
        nilObj->RegisterPrimitive("while:do:", NilWhileDo);
        nilObj->RegisterPrimitive("if:then:", NilIfThen);
        nilObj->RegisterPrimitive("if:then:else:", NilIfThenElse);
        
        // define true and false
        mTrue = Object::New(rootObject, "True");
        mGlobals->Define("True", mTrue);
        
        mFalse = Object::New(rootObject, "False");
        mGlobals->Define("False", mFalse);
        
        // define Console
        Ref<Object> console = Object::New(rootObject, "Console");
        mGlobals->Define("Console", console);
        
        // add its methods
        DynamicObject* consoleObj = &static_cast<DynamicObject&>(*console);
        consoleObj->RegisterPrimitive("write:", ConsoleWrite);
        consoleObj->RegisterPrimitive("writeLine:", ConsoleWriteLine);
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