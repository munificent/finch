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
        AddPrimitive(mObject, "===",             ObjectSame);
        AddPrimitive(mObject, "to-string",       ObjectToString);
        AddPrimitive(mObject, "parent",          ObjectGetParent);
        
        // Arrays.
        mArrayPrototype = MakeGlobal("Arrays");
        AddPrimitive(mArrayPrototype, "count",       ArrayCount);
        AddPrimitive(mArrayPrototype, "add:",        ArrayAdd);
        AddPrimitive(mArrayPrototype, "at:",         ArrayAt);
        AddPrimitive(mArrayPrototype, "at:put:",     ArrayAtPut);
        AddPrimitive(mArrayPrototype, "remove-at:",  ArrayRemoveAt);

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
        AddPrimitive(mFiberPrototype, "running?", FiberRunning);
        AddPrimitive(mFiberPrototype, "done?", FiberDone);
        
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
        AddPrimitive(io, "read-file:", IoReadFile);

        // Bare primitive object.
        Ref<Object> primitives = MakeGlobal("*primitive*");
        AddPrimitive(primitives, "string-concat:and:",       PrimitiveStringConcat);
        AddPrimitive(primitives, "string-compare:to:",       PrimitiveStringCompare);
        AddPrimitive(primitives, "write:",                   PrimitiveWrite);
        /*
        AddPrimitive(primitives, "new-fiber:",               PrimitiveNewFiber);
        AddPrimitive(primitives, "current-fiber",            PrimitiveGetCurrentFiber);
        AddPrimitive(primitives, "switch-to-fiber:passing:", PrimitiveSwitchToFiber);
         */
        AddPrimitive(primitives, "callstack-depth",          PrimitiveGetCallstackDepth);
        
        // The special singleton values.
        mNil = MakeGlobal("nil");
        mTrue = MakeGlobal("true");
        mFalse = MakeGlobal("false");
    }
    
    int Environment::FindGlobal(const String & name)
    {
        int nameId = mStrings.Add(name);
        
        int index;
        if (mGlobalNames.Find(nameId, &index))
        {
            // Found it.
            return index;
        }
        
        // Doesn't exist.
        return -1;
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

    String Environment::FindGlobalName(int index)
    {
        int nameId = mGlobalNames.FindKeyForValue(index);
        ASSERT(nameId != -1, "Not a known global.");

        return mStrings.Find(nameId);
    }

    // TODO(bob): Come up with better name for this.
    Ref<Object> Environment::CreateBlock(Ref<Expr> expr)
    {
        Ref<BlockExemplar> exemplar = Compiler::CompileTopLevel(*this, *expr);
        return Object::NewBlock(*this, exemplar, mNil);
    }
    
    Ref<Object> Environment::MakeGlobal(const char * name)
    {
        int index = DefineGlobal(String(name));
        Ref<Object> global = Object::NewObject(mObject, name);
        SetGlobal(index, global);
        return global;
    }
    
    void Environment::AddPrimitive(Ref<Object> object, String message,
                                   PrimitiveMethod primitive)
    {
        int id = mStrings.Add(message);
        object->AsDynamic()->AddPrimitive(id, primitive);
    }
}

