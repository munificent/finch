#include <iostream>
#include <stdlib.h> // realpath
#include <sys/param.h> // PATH_MAX

#include "FileLineReader.h"
#include "FinchString.h"
#include "Interpreter.h"
#include "Fiber.h"
#include "Ref.h"
#include "ReplLineReader.h"
#include "StandaloneInterpreterHost.h"

using namespace Finch;

using std::cout;
using std::endl;

Ref<ILineReader> OpenFile(String filePath);
bool InterpretFile(Interpreter & interpreter, String filePath);
PRIMITIVE(LoadFile);

//### bob: should move this stuff into a "standalone" class
Ref<ILineReader> OpenFile(String filePath)
{
    Ref<ILineReader> reader = Ref<ILineReader>(new FileLineReader(filePath));
    
    if (reader->EndOfLines())
    {
        // couldn't open
        std::cout << "Couldn't open file \"" << filePath << "\"" << std::endl;
        return Ref<ILineReader>();
    }
    
    return reader;
}

bool InterpretFile(Interpreter & interpreter, String filePath)
{
    Ref<ILineReader> reader = OpenFile(filePath);
    if (reader.IsNull()) return false;
    
    interpreter.Interpret(*reader, false);
    return true;
}
/*
PRIMITIVE(LoadFile)
{
    String filePath = args[0]->AsString();
    Ref<ILineReader> reader = OpenFile(filePath);
    
    fiber.GetInterpreter().Interpret(*reader, fiber);
}
*/
int main (int argc, char * const argv[])
{    
    StandaloneInterpreterHost host;
    Interpreter               interpreter(host);
    /*
    // Set up the standalone-provided behavior.
    interpreter.BindMethod("Ether", "load:", LoadFile);
    */
    // Figure out the absolute path to the core library, relative to the
    // executable. Assumes a directory layout like:
    // finch
    // - build/<config>/finch
    // - lib/core.fin
    char coreLibPath[PATH_MAX];
    char fullPath[PATH_MAX];
    strncpy(coreLibPath, argv[0], PATH_MAX);
    strncat(coreLibPath, "/../../../lib/core.fin", PATH_MAX);
    realpath(coreLibPath, fullPath);

    // Load the core library.
    if (!InterpretFile(interpreter, fullPath))
    {
        cout << "Could not load core library." << endl;
        return 2;
    }
    
    if (argc == 1)
    {
        // With no arguments (arg zero is app), run in interactive mode.
        cout << "Finch 0.0.0d" << endl;
        cout << "------------" << endl;
        
        //### bob: host should provide some external function to set a "quit"
        // Flag so we can exit this loop.
        while (true) {
            // ANSI color: std::cout << "\033[0;32m";
            ReplLineReader reader;
            interpreter.Interpret(reader, true);
        }
    }
    else if (argc == 2)
    {
        // One argument, load and execute the given script.
        String fileName = argv[1];
        return InterpretFile(interpreter, fileName) ? 0 : 1;
    }
    
    return 0;
}

