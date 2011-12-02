#include "ArrayPrimitives.h"
#include "BlockObject.h"
#include "BlockPrimitives.h"
#include "Compiler.h"
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
        
        // define Object
        mObject = Object::NewObject(Ref<Object>(), "Object");
        mGlobals->Define("Object", mObject);

        DynamicObject* objectObj = mObject->AsDynamic();
        objectObj->RegisterPrimitive("=",               ObjectEquals);
        objectObj->RegisterPrimitive("toString",        ObjectToString);
        objectObj->RegisterPrimitive("parent",          ObjectGetParent);
        objectObj->RegisterPrimitive("parent:",         ObjectSetParent);

        // define Array prototype
        mArrayPrototype = Object::NewObject(mObject, "Arrays");
        DynamicObject* arrayObj = mArrayPrototype->AsDynamic();
        arrayObj->RegisterPrimitive("length",      ArrayLength);
        arrayObj->RegisterPrimitive("add:",        ArrayAdd);
        arrayObj->RegisterPrimitive("at:",         ArrayAt);
        arrayObj->RegisterPrimitive("at:put:",     ArrayAtPut);
        mGlobals->Define("Arrays", mArrayPrototype);

        // define Blocks prototype
        mBlockPrototype = Object::NewObject(mObject, "Blocks");
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
        mGlobals->Define("Blocks", mBlockPrototype);

        // define Fiber prototype
        mFiberPrototype = Object::NewObject(mObject, "Fibers");
        DynamicObject* fiberObj = mFiberPrototype->AsDynamic();
        fiberObj->RegisterPrimitive("running?", FiberRunning);
        fiberObj->RegisterPrimitive("done?", FiberDone);
        mGlobals->Define("Fibers", mFiberPrototype);

        // define Number type object
        mNumberPrototype = Object::NewObject(mObject, "Numbers");
        mGlobals->Define("Numbers", mNumberPrototype);

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
        mStringPrototype = Object::NewObject(mObject, "Strings");
        mGlobals->Define("Strings", mStringPrototype);

        DynamicObject* stringObj = mStringPrototype->AsDynamic();
        stringObj->RegisterPrimitive("length",      StringLength);
        stringObj->RegisterPrimitive("at:",         StringAt);
        stringObj->RegisterPrimitive("from:count:", StringFromCount);
        stringObj->RegisterPrimitive("hashCode",    StringHashCode);
        stringObj->RegisterPrimitive("indexOf:",    StringIndexOf);

        // define Ether
        Ref<Object> ether = Object::NewObject(mObject, "Ether");
        mGlobals->Define("Ether", ether);
                
        // define bare primitive object
        Ref<Object> primitives = Object::NewObject(mObject);
        mGlobals->Define("#PRIM#", primitives);

        DynamicObject* primsObj = primitives->AsDynamic();
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
        Ref<CodeBlock> code = Compiler::CompileTopLevel(*this, *expr);
        mBlocks.Add(code);
        
        // wrap it in an object
        //### bob: should look for other places that call NewBlock and see if
        // they can be consolidated with this.
        return Object::NewBlock(*this, *code, mGlobals, mNil);
    }
    
    Ref<Object> Environment::MakeGlobal(const char * name)
    {
        Ref<Object> global = Object::NewObject(mObject, name);
        mGlobals->Define(name, global);
        
        return global;
    }
}

