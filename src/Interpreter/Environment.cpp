#include "Environment.h"

#include "Evaluator.h"
#include "Expr.h"
#include "BlockObject.h"
#include "DynamicObject.h"
#include "BlockPrimitives.h"
#include "EnvironmentPrimitives.h"
#include "NumberPrimitives.h"
#include "ObjectPrimitives.h"
#include "StringPrimitives.h"

namespace Finch
{
    Environment::Environment()
    :   mRunning(true)
    {
        // build the global scope
        mGlobals = Ref<Scope>(new Scope()); 
        mCurrentScope = mGlobals;
        
        // define Object prototype
        Ref<Object> object = Object::NewObject(Ref<Object>(), "Object");
        mGlobals->Define("Object", object);
        DynamicObject* objectObj = &static_cast<DynamicObject&>(*object);
        objectObj->RegisterPrimitive("copy", ObjectCopy);
        objectObj->RegisterPrimitive("add-field:value:", ObjectAddFieldValue);
        objectObj->RegisterPrimitive("add-method:body:", ObjectAddMethodValue);

        // define Block prototype
        mBlock = Object::NewObject(object, "Block");
        mGlobals->Define("Block", mBlock);
        
        DynamicObject* blockObj = &static_cast<DynamicObject&>(*mBlock);
        blockObj->RegisterPrimitive("call", BlockCall);
        blockObj->RegisterPrimitive("call:", BlockCall);
        blockObj->RegisterPrimitive("call::", BlockCall);
        blockObj->RegisterPrimitive("call:::", BlockCall);
        blockObj->RegisterPrimitive("call::::", BlockCall);
        blockObj->RegisterPrimitive("call:::::", BlockCall);
        blockObj->RegisterPrimitive("call::::::", BlockCall);
        blockObj->RegisterPrimitive("call:::::::", BlockCall);
        blockObj->RegisterPrimitive("call::::::::", BlockCall);
        blockObj->RegisterPrimitive("call:::::::::", BlockCall);
        blockObj->RegisterPrimitive("call::::::::::", BlockCall);
        
        // define Number prototype
        mNumber = Object::NewObject(object, "Number");
        mGlobals->Define("Number", mNumber);
        
        DynamicObject* numberObj = &static_cast<DynamicObject&>(*mNumber);
        numberObj->RegisterPrimitive("abs", NumberAbs);
        numberObj->RegisterPrimitive("neg", NumberNeg);
        numberObj->RegisterPrimitive("+",   NumberAdd);
        numberObj->RegisterPrimitive("-",   NumberSubtract);
        numberObj->RegisterPrimitive("*",   NumberMultiply);
        numberObj->RegisterPrimitive("/",   NumberDivide);
        numberObj->RegisterPrimitive("=",   NumberEquals);
        numberObj->RegisterPrimitive("!=",  NumberNotEquals);
        numberObj->RegisterPrimitive("<",   NumberLessThan);
        numberObj->RegisterPrimitive(">",   NumberGreaterThan);
        numberObj->RegisterPrimitive("<=",  NumberLessThanOrEqual);
        numberObj->RegisterPrimitive(">=",  NumberGreaterThanOrEqual);
        
        // define String prototype
        mString = Object::NewObject(object, "String");
        mGlobals->Define("String", mString);
        
        DynamicObject* stringObj = &static_cast<DynamicObject&>(*mString);
        stringObj->RegisterPrimitive("+",       StringAdd);
        stringObj->RegisterPrimitive("length",  StringLength);
        stringObj->RegisterPrimitive("at:",     StringAt);
        
        // define nil
        mNil = Object::NewObject(object, "Nil");
        mGlobals->Define("Nil", mNil);
        
        // define true and false
        mTrue = Object::NewObject(object, "True");
        mGlobals->Define("True", mTrue);
        
        mFalse = Object::NewObject(object, "False");
        mGlobals->Define("False", mFalse);
        
        // define Environment
        Ref<Object> environment = Object::NewObject(object, "Environment");
        mGlobals->Define("Environment", environment);
        
        DynamicObject* environmentObj = &static_cast<DynamicObject&>(*environment);
        environmentObj->RegisterPrimitive("quit",           EnvironmentQuit);
        environmentObj->RegisterPrimitive("if:then:",       EnvironmentIfThen);
        environmentObj->RegisterPrimitive("if:then:else:",  EnvironmentIfThenElse);
        environmentObj->RegisterPrimitive("while:do:",      EnvironmentWhileDo);
        environmentObj->RegisterPrimitive("write:",         EnvironmentWrite);
        environmentObj->RegisterPrimitive("write-line:",    EnvironmentWriteLine);
        environmentObj->RegisterPrimitive("load:",          EnvironmentLoad);
    }
    
    Ref<Object> Environment::EvaluateBlock(const BlockObject * block,
                                           const vector<Ref<Object> > & args)
    {
        // create a scope for the block using its closure as the parent scope
        Ref<Scope> oldScope = mCurrentScope;
        mCurrentScope = Ref<Scope>(new Scope(block->Closure()));
        
        if (block->Params().size() != args.size())
        {
            //### bob: need better error handling
            std::cout << "block expects " << block->Params().size()
                 << " arguments, but got " << args.size() << "." << std::endl;
            
            return Ref<Object>();
        }
        
        // bind the arguments
        for (int i = 0; i < args.size(); i++)
        {
            mCurrentScope->Define(block->Params()[i], args[i]);
        }
        
        Evaluator evaluator(*this);
        Ref<Object> result = evaluator.Evaluate(block->Body());

        mCurrentScope = oldScope;
        
        return result;
    }
    
    Ref<Object> Environment::EvaluateMethod(Ref<Object> self,
                                            const BlockObject * block,
                                            const vector<Ref<Object> > & args)
    {
        // swap out the current self object
        Ref<Object> previousSelf = mSelf;
        mSelf = self;
        
        Ref<Object> result = EvaluateBlock(block, args);
        
        mSelf = previousSelf;
        
        return result;
    }
}