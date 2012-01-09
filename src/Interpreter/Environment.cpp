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
        // Build the global scope.
        
        // Object.
        mObject = MakeGlobal("Object");
        DynamicObject* objectObj = mObject->AsDynamic();
        /*
        objectObj->AddPrimitive("===",             ObjectSame);
        objectObj->AddPrimitive("to-string",       ObjectToString);
        objectObj->AddPrimitive("parent",          ObjectGetParent);
        objectObj->AddPrimitive("parent:",         ObjectSetParent);
         */
        
        // Arrays.
        mArrayPrototype = MakeGlobal("Arrays");
        DynamicObject* arrayObj = mArrayPrototype->AsDynamic();
        /*
        arrayObj->AddPrimitive("count",       ArrayCount);
        arrayObj->AddPrimitive("add:",        ArrayAdd);
        arrayObj->AddPrimitive("at:",         ArrayAt);
        arrayObj->AddPrimitive("at:put:",     ArrayAtPut);
        arrayObj->AddPrimitive("remove-at:",  ArrayRemoveAt);
*/
        // Blocks.
        mBlockPrototype = MakeGlobal("Blocks");
        AddPrimitive(mBlockPrototype, "call", BlockCall);
        AddPrimitive(mBlockPrototype, "call:", BlockCall);
        AddPrimitive(mBlockPrototype, "call::", BlockCall);
        AddPrimitive(mBlockPrototype, "call:::", BlockCall);
        AddPrimitive(mBlockPrototype, "call::::", BlockCall);
        AddPrimitive(mBlockPrototype, "call:::::", BlockCall);
        AddPrimitive(mBlockPrototype, "call::::::", BlockCall);
        AddPrimitive(mBlockPrototype, "call:::::::", BlockCall);
        AddPrimitive(mBlockPrototype, "call::::::::", BlockCall);
        AddPrimitive(mBlockPrototype, "call:::::::::", BlockCall);
        AddPrimitive(mBlockPrototype, "call::::::::::", BlockCall);

        // Fibers.
        mFiberPrototype = MakeGlobal("Fibers");
        DynamicObject* fiberObj = mFiberPrototype->AsDynamic();
        /*
        fiberObj->AddPrimitive("running?", FiberRunning);
        fiberObj->AddPrimitive("done?", FiberDone);
*/
        
        // Numbers.
        mNumberPrototype = MakeGlobal("Numbers");
        AddPrimitive(mNumberPrototype, "abs", NumberAbs);
        AddPrimitive(mNumberPrototype, "neg", NumberNeg);
        AddPrimitive(mNumberPrototype, "mod:", NumberMod);
        AddPrimitive(mNumberPrototype, "floor", NumberFloor);
        AddPrimitive(mNumberPrototype, "ceiling", NumberCeiling);
        AddPrimitive(mNumberPrototype, "sqrt",  NumberSqrt);
        AddPrimitive(mNumberPrototype, "sin",   NumberSin);
        AddPrimitive(mNumberPrototype, "cos",   NumberCos);
        AddPrimitive(mNumberPrototype, "tan",   NumberTan);
        AddPrimitive(mNumberPrototype, "asin",  NumberAsin);
        AddPrimitive(mNumberPrototype, "acos",  NumberAcos);
        AddPrimitive(mNumberPrototype, "atan",  NumberAtan);
        AddPrimitive(mNumberPrototype, "atan:", NumberAtan2);
        AddPrimitive(mNumberPrototype, "+number:", NumberAdd);
        AddPrimitive(mNumberPrototype, "-number:", NumberSubtract);
        AddPrimitive(mNumberPrototype, "*number:", NumberMultiply);
        AddPrimitive(mNumberPrototype, "/number:", NumberDivide);
        AddPrimitive(mNumberPrototype, "=number:", NumberEquals);
        AddPrimitive(mNumberPrototype, "!=",  NumberNotEquals);
        AddPrimitive(mNumberPrototype, "<",   NumberLessThan);
        AddPrimitive(mNumberPrototype, ">",   NumberGreaterThan);
        AddPrimitive(mNumberPrototype, "<=",  NumberLessThanOrEqual);
        AddPrimitive(mNumberPrototype, ">=",  NumberGreaterThanOrEqual);
        
        // Strings.
        mStringPrototype = MakeGlobal("Strings");
        AddPrimitive(mStringPrototype, "count",       StringCount);
        AddPrimitive(mStringPrototype, "at:",         StringAt);
        AddPrimitive(mStringPrototype, "from:count:", StringFromCount);
        AddPrimitive(mStringPrototype, "hash-code",   StringHashCode);
        AddPrimitive(mStringPrototype, "index-of:",   StringIndexOf);

        // Ether.
        MakeGlobal("Ether");
        
        // Io.
        Ref<Object> io = MakeGlobal("Io");
        DynamicObject* ioObj = io->AsDynamic();
        /*
        ioObj->AddPrimitive("read-file:", IoReadFile);
*/
        // Bare primitive object.
        Ref<Object> primitives = MakeGlobal("*primitive*");
        DynamicObject* primsObj = primitives->AsDynamic();
        /*
        primsObj->AddPrimitive("string-concat:and:",       PrimitiveStringConcat);
        primsObj->AddPrimitive("string-compare:to:",       PrimitiveStringCompare);
        primsObj->AddPrimitive("write:",                   PrimitiveWrite);
        primsObj->AddPrimitive("new-fiber:",               PrimitiveNewFiber);
        primsObj->AddPrimitive("current-fiber",            PrimitiveGetCurrentFiber);
        primsObj->AddPrimitive("switch-to-fiber:passing:", PrimitiveSwitchToFiber);
        primsObj->AddPrimitive("callstack-depth",          PrimitiveGetCallstackDepth);
        */
        
        // The special singleton values.
        mNil = MakeGlobal("nil");
        mTrue = MakeGlobal("true");
        mFalse = MakeGlobal("false");
    }

    int Environment::DefineGlobal(const String & name)
    {
        int nameId = mStrings.Add(name);
        
        int index;
        if (mGlobalNames.Find(nameId, &index))
        {
            // Already exists, so use that slot.
            return index;
        }
        
        // New global.
        mGlobalNames.Insert(nameId, mGlobals.Count());
        
        // Add an empty slot. Here, "empty" means that the global has been
        // declared but hasn't been initialized yet.
        mGlobals.Add(Ref<Object>());
        return mGlobals.Count() - 1;
    }

    Ref<Object> Environment::GetGlobal(int index)
    {
        return mGlobals[index];
    }
    
    void Environment::SetGlobal(int index, Ref<Object> value)
    {
        mGlobals[index] = value;
    }

    // TODO(bob): Come up with better name for this.
    Ref<Object> Environment::CreateBlock(Ref<Expr> expr)
    {
        Ref<BlockExemplar> exemplar = Compiler::CompileTopLevel(*this, *expr);
        return Object::NewBlock(*this, exemplar, mNil);
    }
    
    Ref<Object> Environment::MakeGlobal(const char * name)
    {
        int id = mStrings.Add(name);
        Ref<Object> global = Object::NewObject(mObject, name);
        mGlobals.Add(global);
        mGlobalNames.Insert(id, mGlobals.Count() - 1);
        
        return global;
    }
    
    void Environment::AddPrimitive(Ref<Object> object, String message,
                                   PrimitiveMethod primitive)
    {
        int id = mStrings.Add(message);
        object->AsDynamic()->AddPrimitive(id, primitive);
    }
}

