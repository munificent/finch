#pragma once

#include "Environment.h"
#include "Macros.h"

namespace Finch
{
    class IInterpreterHost;
    class ILineReader;
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
        
        //### bob: this is more or less temp
        void Execute(ILineReader & reader, bool parseFile, bool callBlock);
        void Execute(Process & process, ILineReader & reader, bool parseFile,
                     bool callBlock);
        
    private:
        IInterpreterHost & mHost;
        Environment        mEnvironment;
        
        NO_COPY(Interpreter);
    };
}

