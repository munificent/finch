#pragma once

#include "Environment.h"
#include "Macros.h"
#include "Object.h"

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
        Interpreter(IInterpreterHost & host)
        :   mHost(host)
        {}
        
        // Reads from the given source and executes the results in a new fiber
        // in this interpreter.
        void Interpret(ILineReader & reader, bool showResult);
        
        // Gets the Environment owned by this Interpreter.
        Environment & GetEnvironment() { return mEnvironment; }
        
        //### bob: exposing the entire host here is a bit dirty.
        IInterpreterHost & GetHost() { return mHost; }
        
        //### bob: should not be part of public api.
        Ref<Object> GetCurrentFiber() { return mCurrentFiber; }
        void SwitchToFiber(Ref<Object> fiber);

        // Binds an external function to a message handler for a named global
        // object.
        // - objectName The name of the global object to bind the method on.
        // - message    The name of the message to bind the method to.
        // - method     The function to call when the object receives the
        //              message.
        void BindMethod(String objectName, String message,
                        PrimitiveMethod method);

    private:
        Ref<Object> Run();
        Ref<Expr>   Parse(ILineReader & reader);

        IInterpreterHost & mHost;
        Environment        mEnvironment;
        
        Ref<Object> mLastFiber;
        Ref<Object> mCurrentFiber;
        
        NO_COPY(Interpreter);
    };
}

