#pragma once

#include "Environment.h"
#include "Macros.h"

namespace Finch
{
    class IInterpreterHost;
    class ILineReader;
    class Object;
    //### bob: ideally, this wouldn't be in the public api for Interpreter.
    class Process;
    
    // The main top-level class for a Finch virtual machine. To host a Finch
    // interpreter from within your application, you will instantiate one of
    // these and pass in a host object that you've created.
    class Interpreter
    {
    public:
        Interpreter(IInterpreterHost & host)
        :   mHost(host)
        {}
        
        // Reads the .fin file at the given path and executes its contents in
        // a new process in this interpreter.
        bool InterpretFile(String filePath);
        
        // Reads from the given source and executes the results in a new process
        // in this interpreter.
        void InterpretSource(ILineReader & reader);
        
        //### bob: name is wip
        void EtherLoad(Process & process, String filePath);
        
    private:
        Ref<Expr> Parse(ILineReader & reader);
        bool      Execute(Ref<Expr> expr);

        IInterpreterHost & mHost;
        Environment        mEnvironment;
        
        NO_COPY(Interpreter);
    };
}

