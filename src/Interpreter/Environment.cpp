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
        
        Ref<Object> rootObject = Object::NewObject(Ref<Object>(), "Object");
        mGlobals->Define("Object", rootObject);
        DynamicObject* objectObj = &static_cast<DynamicObject&>(*rootObject);
        objectObj->RegisterPrimitive("copy", ObjectCopy);
        objectObj->RegisterPrimitive("add-field:value:", ObjectAddFieldValue);
        objectObj->RegisterPrimitive("add-method:body:", ObjectAddMethodValue);

        // define Block prototype
        mBlock = Object::NewObject(rootObject, "Block");
        mGlobals->Define("Block", mBlock);
        
        DynamicObject* blockObj = &static_cast<DynamicObject&>(*mBlock);
        blockObj->RegisterPrimitive("value", BlockValue);
        blockObj->RegisterPrimitive("value:", BlockValue);
        blockObj->RegisterPrimitive("value:value:", BlockValue);
        blockObj->RegisterPrimitive("value:value:value:", BlockValue);
        blockObj->RegisterPrimitive("value:value:value:value:", BlockValue);
        blockObj->RegisterPrimitive("value:value:value:value:value:", BlockValue);
        blockObj->RegisterPrimitive("value:value:value:value:value:value:", BlockValue);
        
        // define Number prototype
        mNumber = Object::NewObject(rootObject, "Number");
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
        mString = Object::NewObject(rootObject, "String");
        mGlobals->Define("String", mString);
        
        DynamicObject* stringObj = &static_cast<DynamicObject&>(*mString);
        stringObj->RegisterPrimitive("+",       StringAdd);
        stringObj->RegisterPrimitive("length",  StringLength);
        stringObj->RegisterPrimitive("at:",     StringAt);
        
        // define nil
        mNil = Object::NewObject(rootObject, "Nil");
        mGlobals->Define("Nil", mNil);
        
        // define true and false
        mTrue = Object::NewObject(rootObject, "True");
        mGlobals->Define("True", mTrue);
        
        mFalse = Object::NewObject(rootObject, "False");
        mGlobals->Define("False", mFalse);
        
        // define Environment
        //### bob: this should actually refer to this object itself. but first
        // Environment will need to be a DynamicObject. note that this will
        // also pretty much kill the ref-counter: Environment will have a
        // circular reference to itself and will in turn reference everything
        // else. :(
        Ref<Object> environment = Object::NewObject(rootObject, "Environment");
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