#include <sstream>

#include "ArrayObject.h"
#include "ArrayPrimitives.h"
#include "BlockObject.h"
#include "BlockPrimitives.h"
#include "Compiler.h"
#include "DynamicObject.h"
#include "Expr.h"
#include "Fiber.h"
#include "FiberObject.h"
#include "FiberPrimitives.h"
#include "FileLineReader.h"
#include "FinchParser.h"
#include "IErrorReporter.h"
#include "IInterpreterHost.h"
#include "Interpreter.h"
#include "IoPrimitives.h"
#include "Lexer.h"
#include "LineNormalizer.h"
#include "NumberObject.h"
#include "NumberPrimitives.h"
#include "ObjectPrimitives.h"
#include "Primitives.h"
#include "StringObject.h"
#include "StringPrimitives.h"

namespace Finch
{
    // Error reporter passed to the parser so that parse errors can be routed
    // through the interpreter host.
    class InterpreterErrorReporter : public IErrorReporter
    {
    public:
        InterpreterErrorReporter(Interpreter & interpreter)
        :   mInterpreter(interpreter)
        {}
        
        virtual void Error(String message)
        {
            mInterpreter.GetHost().Error(message);
        }
        
    private:
        Interpreter & mInterpreter;
    };
    
    Interpreter::Interpreter(IInterpreterHost & host)
    :   mHost(host)
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
    
    void Interpreter::Interpret(ILineReader & reader, bool showResult)
    {
        Ref<Expr> expr = Parse(reader);
        
        // Bail if we failed to parse.
        if (expr.IsNull()) return;
        
        // Create a starting fiber for the expression.
        Ref<Block> block = Compiler::CompileTopLevel(*this, *expr);
        Ref<Object> blockObj = NewBlock(block, mNil);
        Ref<Object> fiber = NewFiber(blockObj);
        
        // Run the interpreter.
        Ref<Object> result = fiber->AsFiber()->GetFiber().Execute();
        
        if (showResult)
        {
            std::stringstream text;
            text << *result << std::endl;
            mHost.Output(String(text.str().c_str()));
        }
    }
    
    void Interpreter::BindMethod(String objectName, String message,
                                 PrimitiveMethod method)
    {
        ASSERT_STRING_NOT_EMPTY(objectName);
        ASSERT_STRING_NOT_EMPTY(message);
        ASSERT_NOT_NULL(method);
        
        int globalIndex = DefineGlobal(objectName);
        Ref<Object> object = GetGlobal(globalIndex);
        ASSERT(!object.IsNull(), "Must be an existing global variable.");

        DynamicObject* dynamicObj = object->AsDynamic();
        ASSERT_NOT_NULL(dynamicObj);
        
        StringId messageId = mStrings.Add(message);
        dynamicObj->AddPrimitive(messageId, method);
    }
    
    StringId Interpreter::AddString(const String & string)
    {
        return mStrings.Add(string);
    }

    String Interpreter::FindString(StringId id)
    {
        return mStrings.Find(id);
    }
    
    int Interpreter::FindGlobal(const String & name)
    {
        StringId nameId = mStrings.Add(name);
        
        int index;
        if (mGlobalNames.Find(nameId, &index))
        {
            // Found it.
            return index;
        }
        
        // Doesn't exist.
        return -1;
    }
    
    int Interpreter::DefineGlobal(const String & name)
    {
        StringId nameId = mStrings.Add(name);
        
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
    
    Ref<Object> Interpreter::GetGlobal(int index)
    {
        return mGlobals[index];
    }
    
    void Interpreter::SetGlobal(int index, Ref<Object> value)
    {
        mGlobals[index] = value;
    }
    
    String Interpreter::FindGlobalName(int index)
    {
        StringId nameId = mGlobalNames.FindKeyForValue(index);
        ASSERT(nameId != -1, "Not a known global.");
        
        return mStrings.Find(nameId);
    }
    
    
    Ref<Object> Interpreter::NewObject(Ref<Object> parent, String name)
    {
        Ref<Object> object = Ref<Object>(new DynamicObject(parent, name));
        
        // if the object has no parent, use itself as it
        if (parent.IsNull())
        {
            object->SetParent(object);
        }
        
        return object;
    }
    
    Ref<Object> Interpreter::NewObject(Ref<Object> parent)
    {
        return NewObject(parent, "");
    }
    
    Ref<Object> Interpreter::NewNumber(double value)
    {
        return Ref<Object>(new NumberObject(mNumberPrototype, value));
    }
    
    Ref<Object> Interpreter::NewString(String value)
    {
        return Ref<Object>(new StringObject(mStringPrototype, value));
    }
    
    Ref<Object> Interpreter::NewArray(int capacity)
    {
        return Ref<Object>(new ArrayObject(mArrayPrototype, capacity));
    }
    
    Ref<Object> Interpreter::NewBlock(Ref<Block> block, Ref<Object> self)
    {
        return Ref<Object>(new BlockObject(mBlockPrototype, block, self));
    }
    
    Ref<Object> Interpreter::NewFiber(Ref<Object> block)
    {
        return Ref<Object>(new FiberObject(mFiberPrototype, *this, block));
    }
    
    Ref<Expr> Interpreter::Parse(ILineReader & reader)
    {
        InterpreterErrorReporter errorReporter(*this);
        Lexer          lexer(reader);
        LineNormalizer normalizer(lexer);
        FinchParser    parser(normalizer, errorReporter);
        
        return parser.Parse();
    }
    
    Ref<Object> Interpreter::MakeGlobal(const char * name)
    {
        int index = DefineGlobal(String(name));
        Ref<Object> global = NewObject(mObject, name);
        SetGlobal(index, global);
        return global;
    }
    
    void Interpreter::AddPrimitive(Ref<Object> object, String message,
                                   PrimitiveMethod primitive)
    {
        StringId id = mStrings.Add(message);
        object->AsDynamic()->AddPrimitive(id, primitive);
    }
}
