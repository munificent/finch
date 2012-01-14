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
        Ref<Object> GetGlobal(int index);
        void SetGlobal(int index, Ref<Object> value);
        
        String FindGlobalName(int index);
        
        // Get the core built-in objects.
        Ref<Object> ObjectPrototype()   const { return mObject; }
        Ref<Object> ArrayPrototype()    const { return mArrayPrototype; }
        Ref<Object> BlockPrototype()    const { return mBlockPrototype; }
        Ref<Object> FiberPrototype()    const { return mFiberPrototype; }
        Ref<Object> NumberPrototype()   const { return mNumberPrototype; }
        Ref<Object> StringPrototype()   const { return mStringPrototype; }
        Ref<Object> Nil()               const { return mNil; }
        Ref<Object> True()              const { return mTrue; }
        Ref<Object> False()             const { return mFalse; }
        
    private:
        Ref<Expr>   Parse(ILineReader & reader);
        
        Ref<Object> MakeGlobal(const char * name);
        void AddPrimitive(Ref<Object> object, String message,
                          PrimitiveMethod primitive);
        
        IInterpreterHost & mHost;

        StringTable mStrings;
        
        // Indexed collection of global variables.
        Array<Ref<Object> > mGlobals;
        // Maps global variable names to their indices. Used by the compiler.
        IdTable<int> mGlobalNames;
        
        Ref<Object> mObject;
        Ref<Object> mArrayPrototype;
        Ref<Object> mBlockPrototype;
        Ref<Object> mFiberPrototype;
        Ref<Object> mNumberPrototype;
        Ref<Object> mStringPrototype;
        Ref<Object> mNil;
        Ref<Object> mTrue;
        Ref<Object> mFalse;
        
        NO_COPY(Interpreter);
    };
}

