#include <iostream>
#include <string>

#include "Compiler.h"
#include "FinchString.h"
#include "Interpreter.h"
#include "Repl.h"
#include "ReplLineReader.h"
#include "Script.h"
#include "StandaloneInterpreterHost.h"

namespace Finch
{
    using std::cin;
    using std::cout;
    using std::endl;
    
    void Repl::Run()
    {
        StandaloneInterpreterHost host;
        Interpreter               interpreter(host);
        
        cout << "Finch 0.0.0d" << endl;
        cout << "------------" << endl;
        
        //### bob: hard-coded path here is a total hack
        // load the base library
#ifdef HACK_ROOT_BASE_PATH
        const char* baseLibPath = "base/main.fin";
#else
        const char* baseLibPath = "../../base/main.fin";
#endif
        Script::Execute(baseLibPath, interpreter);
        
        //### bob: host should provide some external function to set a "quit"
        // flag so we can exit this loop.
        while (true) {
            // ansi color: std::cout << "\033[0;32m";
            ReplLineReader reader;
            interpreter.Execute(reader, false, false);
        }
    }
}

