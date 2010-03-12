#include "ArrayPrimitives.h"
#include "BlockObject.h"
#include "BlockPrimitives.h"
#include "BooleanPrimitives.h"
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
        Ref<Object> object = Object::NewObject(Ref<Object>(), "Object");
        mGlobals->Define("Object", object);
        DynamicObject* objectObj = &static_cast<DynamicObject&>(*object);
        objectObj->RegisterPrimitive("=",               ObjectEquals);
        objectObj->RegisterPrimitive("!=",              ObjectNotEquals);
        objectObj->RegisterPrimitive("copy",            ObjectCopy);
        objectObj->RegisterPrimitive("copyWith:",       ObjectCopyWith);
        objectObj->RegisterPrimitive("toString",        ObjectToString);
        objectObj->RegisterPrimitive("addMethod:body:", ObjectAddMethodBody);
        objectObj->RegisterPrimitive("prototype",       ObjectGetPrototype);
        objectObj->RegisterPrimitive("prototype:",      ObjectSetPrototype);

        // any non-true object is implicitly "false", so sending "not" to it
        // returns true
        objectObj->RegisterPrimitive("not", BooleanTrue);
                
        // define Array prototype
        mArray = Object::NewObject(object, "Array");
        mGlobals->Define("Array", mArray);
        
        DynamicObject* arrayObj = &static_cast<DynamicObject&>(*mArray);
        arrayObj->RegisterPrimitive("length",      ArrayLength);
        arrayObj->RegisterPrimitive("add:",        ArrayAdd);
        arrayObj->RegisterPrimitive("at:",         ArrayAt);
        arrayObj->RegisterPrimitive("at:put:",     ArrayAtPut);
        
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
        numberObj->RegisterPrimitive("mod:", NumberMod);
        numberObj->RegisterPrimitive("floor", NumberFloor);
        numberObj->RegisterPrimitive("ceiling", NumberCeiling);
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
        mString = Object::NewObject(object, "String");
        mGlobals->Define("String", mString);
        
        DynamicObject* stringObj = &static_cast<DynamicObject&>(*mString);
        stringObj->RegisterPrimitive("+",           StringAdd);
        stringObj->RegisterPrimitive("length",      StringLength);
        stringObj->RegisterPrimitive("at:",         StringAt);
        stringObj->RegisterPrimitive("=",           StringEquals);
        stringObj->RegisterPrimitive("!=",          StringNotEquals);
        stringObj->RegisterPrimitive("hashCode",    StringHashCode);
        
        // define nil
        mNil = Object::NewObject(object, "Nil");
        mGlobals->Define("Nil", mNil);
        
        // define true and false
        mTrue = Object::NewObject(object, "True");
        mGlobals->Define("True", mTrue);
        
        DynamicObject* trueObj = &static_cast<DynamicObject&>(*mTrue);
        trueObj->RegisterPrimitive("not", BooleanFalse);
        
        mFalse = Object::NewObject(mNil, "False");
        mGlobals->Define("False", mFalse);

        // define Ether
        Ref<Object> ether = Object::NewObject(object, "Ether");
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
        Ref<Object> primitives = Object::NewObject(object);
        mGlobals->Define("Primitives__", primitives);

        DynamicObject* primitivesObj = primitives->AsDynamic();
        primitivesObj->RegisterPrimitive("newArrayCount:", ArrayNew);
    }
}