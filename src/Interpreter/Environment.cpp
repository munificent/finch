#include "ArrayPrimitives.h"
#include "BlockObject.h"
#include "BlockPrimitives.h"
#include "Compiler.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Expr.h"
#include "FiberPrimitives.h"
#include "Fiber.h"
#include "IoPrimitives.h"
#include "NumberPrimitives.h"
#include "ObjectPrimitives.h"
#include "Primitives.h"
#include "StringPrimitives.h"

namespace Finch
{
    Environment::Environment()
    {
        // build the global scope
        mGlobals = Ref<Scope>(new Scope()); 
        
        // define Object
        mObject = MakeGlobal("Object");
        DynamicObject* objectObj = mObject->AsDynamic();
        objectObj->AddPrimitive("===",             ObjectSame);
        objectObj->AddPrimitive("to-string",       ObjectToString);
        objectObj->AddPrimitive("parent",          ObjectGetParent);
        objectObj->AddPrimitive("parent:",         ObjectSetParent);

        // define Array prototype
        mArrayPrototype = MakeGlobal("Arrays");
        DynamicObject* arrayObj = mArrayPrototype->AsDynamic();
        arrayObj->AddPrimitive("count",       ArrayCount);
        arrayObj->AddPrimitive("add:",        ArrayAdd);
        arrayObj->AddPrimitive("at:",         ArrayAt);
        arrayObj->AddPrimitive("at:put:",     ArrayAtPut);

        // define Blocks prototype
        mBlockPrototype = MakeGlobal("Blocks");
        DynamicObject* blockObj = mBlockPrototype->AsDynamic();
        blockObj->AddPrimitive("call", BlockCall);
        blockObj->AddPrimitive("call:", BlockCall);
        blockObj->AddPrimitive("call::", BlockCall);
        blockObj->AddPrimitive("call:::", BlockCall);
        blockObj->AddPrimitive("call::::", BlockCall);
        blockObj->AddPrimitive("call:::::", BlockCall);
        blockObj->AddPrimitive("call::::::", BlockCall);
        blockObj->AddPrimitive("call:::::::", BlockCall);
        blockObj->AddPrimitive("call::::::::", BlockCall);
        blockObj->AddPrimitive("call:::::::::", BlockCall);
        blockObj->AddPrimitive("call::::::::::", BlockCall);

        // define Fiber prototype
        mFiberPrototype = MakeGlobal("Fibers");
        DynamicObject* fiberObj = mFiberPrototype->AsDynamic();
        fiberObj->AddPrimitive("running?", FiberRunning);
        fiberObj->AddPrimitive("done?", FiberDone);

        // define Number prototype
        mNumberPrototype = MakeGlobal("Numbers");
        DynamicObject* numberObj = mNumberPrototype->AsDynamic();
        numberObj->AddPrimitive("abs", NumberAbs);
        numberObj->AddPrimitive("neg", NumberNeg);
        numberObj->AddPrimitive("mod:", NumberMod);
        numberObj->AddPrimitive("floor", NumberFloor);
        numberObj->AddPrimitive("ceiling", NumberCeiling);
        numberObj->AddPrimitive("sqrt",  NumberSqrt);
        numberObj->AddPrimitive("sin",   NumberSin);
        numberObj->AddPrimitive("cos",   NumberCos);
        numberObj->AddPrimitive("tan",   NumberTan);
        numberObj->AddPrimitive("asin",  NumberAsin);
        numberObj->AddPrimitive("acos",  NumberAcos);
        numberObj->AddPrimitive("atan",  NumberAtan);
        numberObj->AddPrimitive("atan:", NumberAtan2);
        numberObj->AddPrimitive("+number:", NumberAdd);
        numberObj->AddPrimitive("-number:", NumberSubtract);
        numberObj->AddPrimitive("*number:", NumberMultiply);
        numberObj->AddPrimitive("/number:", NumberDivide);
        numberObj->AddPrimitive("=number:", NumberEquals);
        numberObj->AddPrimitive("!=",  NumberNotEquals);
        numberObj->AddPrimitive("<",   NumberLessThan);
        numberObj->AddPrimitive(">",   NumberGreaterThan);
        numberObj->AddPrimitive("<=",  NumberLessThanOrEqual);
        numberObj->AddPrimitive(">=",  NumberGreaterThanOrEqual);
        
        // define String prototype
        mStringPrototype = MakeGlobal("Strings");
        DynamicObject* stringObj = mStringPrototype->AsDynamic();
        stringObj->AddPrimitive("count",       StringCount);
        stringObj->AddPrimitive("at:",         StringAt);
        stringObj->AddPrimitive("from:count:", StringFromCount);
        stringObj->AddPrimitive("hash-code",   StringHashCode);
        stringObj->AddPrimitive("index-of:",   StringIndexOf);

        // define Ether
        MakeGlobal("Ether");
        
        // define IO
        Ref<Object> io = MakeGlobal("Io");
        DynamicObject* ioObj = io->AsDynamic();
        ioObj->AddPrimitive("read-file:", IoReadFile);

        // define bare primitive object
        Ref<Object> primitives = MakeGlobal("#PRIM#");
        DynamicObject* primsObj = primitives->AsDynamic();
        primsObj->AddPrimitive("string-concat:and:",       PrimitiveStringConcat);
        primsObj->AddPrimitive("string-compare:to:",       PrimitiveStringCompare);
        primsObj->AddPrimitive("write:",                   PrimitiveWrite);
        primsObj->AddPrimitive("new-fiber:",               PrimitiveNewFiber);
        primsObj->AddPrimitive("current-fiber",            PrimitiveGetCurrentFiber);
        primsObj->AddPrimitive("switch-to-fiber:passing:", PrimitiveSwitchToFiber);
        primsObj->AddPrimitive("callstack-depth",          PrimitiveGetCallstackDepth);
        
        // make the special values
        mNil = MakeGlobal("nil");
        mTrue = MakeGlobal("true");
        mFalse = MakeGlobal("false");
    }

    Ref<Object> Environment::CreateBlock(Ref<Expr> expr)
    {
        // add it to the code table
        Ref<CodeBlock> code = Compiler::CompileTopLevel(*this, *expr);
        mBlocks.Add(code);
        
        // wrap it in an object
        //### bob: should look for other places that call NewBlock and see if
        // they can be consolidated with this.
        return Object::NewBlock(*this, *code, Ref<Scope>(), mNil);
    }
    
    Ref<Object> Environment::MakeGlobal(const char * name)
    {
        Ref<Object> global = Object::NewObject(mObject, name);
        mGlobals->Define(name, global);
        
        return global;
    }
}

