#include <iostream>

#include "Ref.h"
#include "Repl.h"
#include "Script.h"

using namespace Finch;

int main (int argc, char * const argv[])
{
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
