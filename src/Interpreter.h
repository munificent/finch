#pragma once

#include "Dictionary.h"
#include "Macros.h"
#include "Object.h"
#include "StringTable.h"

namespace Finch
{
    class IInterpreterHost;
    class ILineReader;
    //### bob: ideally, this stuff wouldn't be in the public api for Interpreter.
    class Object;
    class Fiber;
    
    // The main top-level class for a Finch virtual machine. To host a Finch
    // interpreter from within your application, you will instantiate one of
    // these and pass in a host object that you've created.
    class Interpreter
    {
    public:
        Interpreter(IInterpreterHost & host);
        
        // Reads from the given source and executes the results in a new fiber
        // in this interpreter.
        void Interpret(ILineReader & reader, bool showResult);
        
        //### bob: exposing the entire host here is a bit dirty.
        IInterpreterHost & GetHost() { return mHost; }

        // Binds an external function to a message handler for a named global
        // object.
        // - objectName The name of the global object to bind the method on.
        // - message    The name of the message to bind the method to.
        // - method     The function to call when the object receives the
        //              message.
        void BindMethod(String objectName, String message,
                        PrimitiveMethod method);

        StringId AddString(const String & string);
        String FindString(StringId id);
        
        // Returns the slot for the global with the given name. Returns -1 if
        // the global doesn't exist.
        int FindGlobal(const String & name);
        
        // Creates an indexed slot for a global with the given name. If a global
        // with that name already exists, will return that slot.
        int DefineGlobal(const String & name);
        const Value & GetGlobal(int index);
        void SetGlobal(int index, const Value & value);
        
        String FindGlobalName(int index);
        
        // Object constructors.
        Value NewObject(const Value & parent, String name);
        Value NewObject(const Value & parent);
        Value NewNumber(double value);
        Value NewString(String value);
        Value NewArray(int capacity);
        Value NewBlock(Ref<Block> block, const Value & self);
        Value NewFiber(const Value & block);
        
        // Get built-in objects.
        const Value & Nil()   const { return mNil; }
        const Value & True()  const { return mTrue; }
        const Value & False() const { return mFalse; }
        
    private:
        Ref<Expr> Parse(ILineReader & reader);
        
        Value MakeGlobal(const char * name);
        void AddPrimitive(const Value & object, String message,
                          PrimitiveMethod primitive);
        
        IInterpreterHost & mHost;

        StringTable mStrings;
        
        // Indexed collection of global variables.
        Array<Value> mGlobals;
        
        // Maps global variable names to their indices. Used by the compiler.
        IdTable<int> mGlobalNames;
        
        Value mObject;
        Value mArrayPrototype;
        Value mBlockPrototype;
        Value mFiberPrototype;
        Value mNumberPrototype;
        Value mStringPrototype;
        Value mNil;
        Value mTrue;
        Value mFalse;
        
        NO_COPY(Interpreter);
    };
}

