#include <iostream>

#include "Ref.h"
#include "Repl.h"
#include "Script.h"
#include "String.h"

//#define UNIT_TESTS

#ifdef UNIT_TESTS

#include "TestMain.h"

#endif

using namespace Finch;

int main (int argc, char * const argv[])
{
    #ifdef UNIT_TESTS
    TestMain::Run();
    return 0;
    #endif
    
    if (argc == 1)
    {
        // with no arguments (arg zero is app), run in interactive mode
        Repl repl;    
        repl.Run();
    }
    else if (argc == 2)
    {
        // one argument, load and execute the given script
        String fileName = argv[1];
        Script::Run(fileName);
    }
    
    return 0;
}

