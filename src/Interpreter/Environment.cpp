#include "ArrayPrimitives.h"
#include "BlockObject.h"
#include "BlockPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "EtherPrimitives.h"
#include "Expr.h"
#include "Interpreter.h"
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
        
        DynamicObject* objectObj = &static_cast<DynamicObject&>(*mObjectPrototype);
        objectObj->RegisterPrimitive("=",               ObjectEquals);
        objectObj->RegisterPrimitive("!=",              ObjectNotEquals);
        objectObj->RegisterPrimitive("copy",            ObjectCopy);
        objectObj->RegisterPrimitive("copyWith:",       ObjectCopyWith);
        objectObj->RegisterPrimitive("toString",        ObjectToString);
        objectObj->RegisterPrimitive("addMethod:body:", ObjectAddMethodBody);
        objectObj->RegisterPrimitive("parent",          ObjectGetParent);
        objectObj->RegisterPrimitive("parent:",         ObjectSetParent);

        // define Array prototype
        mArrayPrototype = Object::NewObject(mObjectPrototype, "array prototype");
        
        DynamicObject* arrayObj = &static_cast<DynamicObject&>(*mArrayPrototype);
        arrayObj->RegisterPrimitive("length",      ArrayLength);
        arrayObj->RegisterPrimitive("add:",        ArrayAdd);
        arrayObj->RegisterPrimitive("at:",         ArrayAt);
        arrayObj->RegisterPrimitive("at:put:",     ArrayAtPut);
        
        // define Block type object
        mBlockPrototype = Object::NewObject(mObjectPrototype, "block prototype");
        
        DynamicObject* blockObj = &static_cast<DynamicObject&>(*mBlockPrototype);
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
        
        // define Number type object
        mNumberPrototype = Object::NewObject(mObjectPrototype, "number prototype");
        
        DynamicObject* numberObj = &static_cast<DynamicObject&>(*mNumberPrototype);
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
        
        // define String type object
        mStringPrototype = Object::NewObject(mObjectPrototype, "string prototype");
        
        DynamicObject* stringObj = &static_cast<DynamicObject&>(*mStringPrototype);
        stringObj->RegisterPrimitive("+",           StringAdd);
        stringObj->RegisterPrimitive("length",      StringLength);
        stringObj->RegisterPrimitive("at:",         StringAt);
        stringObj->RegisterPrimitive("=",           StringEquals);
        stringObj->RegisterPrimitive("!=",          StringNotEquals);
        stringObj->RegisterPrimitive("hashCode",    StringHashCode);
        
        // define nil
        mNil = Object::NewObject(mObjectPrototype, "Nil");
        mGlobals->Define("Nil", mNil);
        
        // define true and false
        mTrue = Object::NewObject(mObjectPrototype, "true");
        mGlobals->Define("True", mTrue);
        
        mFalse = Object::NewObject(mNil, "false");
        mGlobals->Define("False", mFalse);

        // define Ether
        Ref<Object> ether = Object::NewObject(mObjectPrototype, "Ether");
        mGlobals->Define("Ether", ether);
        
        DynamicObject* etherObj = &static_cast<DynamicObject&>(*ether);
        etherObj->RegisterPrimitive("quit",           EtherQuit);
        etherObj->RegisterPrimitive("do:",            EtherDo);
        etherObj->RegisterPrimitive("if:then:",       EtherIfThen);
        etherObj->RegisterPrimitive("if:then:else:",  EtherIfThenElse);
        
        etherObj->RegisterPrimitive("while:do:",      EtherWhileDo);
        etherObj->RegisterPrimitive("write:",         EtherWrite);
        etherObj->RegisterPrimitive("writeLine:",     EtherWriteLine);
        etherObj->RegisterPrimitive("load:",          EtherLoad);
        
        // define bare primitive object
        Ref<Object> primitives = Object::NewObject(mObjectPrototype);
        mGlobals->Define("Prims**", primitives);

        DynamicObject* primsObj = &static_cast<DynamicObject&>(*primitives);
        primsObj->RegisterPrimitive("objectPrototype",  ObjectGetPrototype);
        primsObj->RegisterPrimitive("blockPrototype",   BlockGetPrototype);
        primsObj->RegisterPrimitive("numberPrototype",  NumberGetPrototype);
        primsObj->RegisterPrimitive("stringPrototype",  StringGetPrototype);
        primsObj->RegisterPrimitive("arrayPrototype",   ArrayGetPrototype);
    }
}

