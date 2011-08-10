#include "ArrayPrimitives.h"
#include "BlockObject.h"
#include "BlockPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Expr.h"
#include "FiberPrimitives.h"
#include "Primitives.h"
#include "Fiber.h"
#include "NumberPrimitives.h"
#include "ObjectPrimitives.h"
#include "StringPrimitives.h"

namespace Finch
{
    Environment::Environment()
    {
        // build the global scope
        mGlobals = Ref<Scope>(new Scope()); 
        
        // define Object prototype
        mObjectPrototype = Object::NewObject(Ref<Object>(), "object prototype");
        
        DynamicObject* objectObj = mObjectPrototype->AsDynamic();
        objectObj->RegisterPrimitive("=",               ObjectEquals);
        objectObj->RegisterPrimitive("toString",        ObjectToString);
        objectObj->RegisterPrimitive("parent",          ObjectGetParent);
        objectObj->RegisterPrimitive("parent:",         ObjectSetParent);

        // define Array prototype
        mArrayPrototype = Object::NewObject(mObjectPrototype, "array prototype");
        DynamicObject* arrayObj = mArrayPrototype->AsDynamic();
        arrayObj->RegisterPrimitive("length",      ArrayLength);
        arrayObj->RegisterPrimitive("add:",        ArrayAdd);
        arrayObj->RegisterPrimitive("at:",         ArrayAt);
        arrayObj->RegisterPrimitive("at:put:",     ArrayAtPut);
        
        // define Block type object
        mBlockPrototype = Object::NewObject(mObjectPrototype, "block prototype");
        DynamicObject* blockObj = mBlockPrototype->AsDynamic();
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
        
        // define Fiber prototype
        mFiberPrototype = Object::NewObject(mObjectPrototype, "fiber prototype");
        DynamicObject* fiberObj = mFiberPrototype->AsDynamic();
        fiberObj->RegisterPrimitive("running?", FiberRunning);
        fiberObj->RegisterPrimitive("done?", FiberDone);
        
        // define Number type object
        mNumberPrototype = Object::NewObject(mObjectPrototype, "number prototype");
        
        DynamicObject* numberObj = mNumberPrototype->AsDynamic();
        numberObj->RegisterPrimitive("abs", NumberAbs);
        numberObj->RegisterPrimitive("neg", NumberNeg);
        numberObj->RegisterPrimitive("mod:", NumberMod);
        numberObj->RegisterPrimitive("floor", NumberFloor);
        numberObj->RegisterPrimitive("ceiling", NumberCeiling);
        numberObj->RegisterPrimitive("sqrt",  NumberSqrt);
        numberObj->RegisterPrimitive("sin",   NumberSin);
        numberObj->RegisterPrimitive("cos",   NumberCos);
        numberObj->RegisterPrimitive("tan",   NumberTan);
        numberObj->RegisterPrimitive("asin",  NumberAsin);
        numberObj->RegisterPrimitive("acos",  NumberAcos);
        numberObj->RegisterPrimitive("atan",  NumberAtan);
        numberObj->RegisterPrimitive("atan:", NumberAtan2);
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
        mStringPrototype = Object::NewObject(mObjectPrototype, "string prototype");
        
        DynamicObject* stringObj = mStringPrototype->AsDynamic();
        stringObj->RegisterPrimitive("length",      StringLength);
        stringObj->RegisterPrimitive("at:",         StringAt);
        stringObj->RegisterPrimitive("hashCode",    StringHashCode);

        // define Ether
        Ref<Object> ether = Object::NewObject(mObjectPrototype, "Ether");
        mGlobals->Define("Ether", ether);
                
        // define bare primitive object
        Ref<Object> primitives = Object::NewObject(mObjectPrototype);
        mGlobals->Define("#PRIM#", primitives);

        DynamicObject* primsObj = primitives->AsDynamic();
        primsObj->RegisterPrimitive("arrayPrototype",           PrimitiveGetArrayPrototype);
        primsObj->RegisterPrimitive("blockPrototype",           PrimitiveGetBlockPrototype);
        primsObj->RegisterPrimitive("fiberPrototype",           PrimitiveGetFiberPrototype);
        primsObj->RegisterPrimitive("numberPrototype",          PrimitiveGetNumberPrototype);
        primsObj->RegisterPrimitive("objectPrototype",          PrimitiveGetObjectPrototype);
        primsObj->RegisterPrimitive("stringPrototype",          PrimitiveGetStringPrototype);
        primsObj->RegisterPrimitive("copy:",                    PrimitiveCopy);
        primsObj->RegisterPrimitive("run:within:",              PrimitiveRunWithin);
        primsObj->RegisterPrimitive("stringConcat:and:",        PrimitiveStringConcat);
        primsObj->RegisterPrimitive("stringCompare:to:",        PrimitiveStringCompare);
        primsObj->RegisterPrimitive("write:",                   PrimitiveWrite);
        primsObj->RegisterPrimitive("newFiber:",                PrimitiveNewFiber);
        primsObj->RegisterPrimitive("currentFiber",             PrimitiveGetCurrentFiber);
        primsObj->RegisterPrimitive("switchToFiber:passing:",   PrimitiveSwitchToFiber);
        primsObj->RegisterPrimitive("callstackDepth",           PrimitiveGetCallstackDepth);
        
        // make the special values
        mNil = MakeGlobal("Nil");
        mTrue = MakeGlobal("True");
        mFalse = MakeGlobal("False");
    }

    Ref<Object> Environment::CreateBlock(Ref<Expr> expr)
    {
        // add it to the code table
        int id = mBlocks.Add(Array<String>(), *expr, *this);
        const CodeBlock & code = mBlocks.Find(id);
        
        // wrap it in an object
        //### bob: should look for other places that call NewBlock and see if
        // they can be consolidated with this.
        return Object::NewBlock(*this, code, mGlobals, mNil);
    }
    
    Ref<Object> Environment::MakeGlobal(const char * name)
    {
        Ref<Object> global = Object::NewObject(mObjectPrototype, name);
        mGlobals->Define(name, global);
        
        return global;
    }
}

