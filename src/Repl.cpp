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
    
    void Repl::Run(Interpreter & interpreter)
    {
        cout << "Finch 0.0.0d" << endl;
        cout << "------------" << endl;
        
        //### bob: host should provide some external function to set a "quit"
        // flag so we can exit this loop.
        while (true) {
            // ansi color: std::cout << "\033[0;32m";
            ReplLineReader reader;
            interpreter.InterpretSource(reader);
        }
    }
}

