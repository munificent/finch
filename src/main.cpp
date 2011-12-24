#include <iostream>

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

PRIMITIVE(LoadFile)
{
    String filePath = args[0]->AsString();
    Ref<ILineReader> reader = OpenFile(filePath);
    
    fiber.GetInterpreter().Interpret(*reader, fiber);
}

int main (int argc, char * const argv[])
{
    StandaloneInterpreterHost host;
    Interpreter               interpreter(host);
    
    // set up the standalone-provided behavior
    interpreter.BindMethod("Ether", "load:", LoadFile);
    
    // load the base library
    //### bob: hard-coded path here is a total hack
#ifdef HACK_ROOT_BASE_PATH
    const char* baseLibPath = "lib/core.fin";
#else
    const char* baseLibPath = "../../lib/core.fin";
#endif
    if (!InterpretFile(interpreter, baseLibPath))
    {
        cout << "Could not load base library." << endl;
        return 2;
    }
    
    if (argc == 1)
    {
        // with no arguments (arg zero is app), run in interactive mode
        cout << "Finch 0.0.0d" << endl;
        cout << "------------" << endl;
        
        //### bob: host should provide some external function to set a "quit"
        // flag so we can exit this loop.
        while (true) {
            // ansi color: std::cout << "\033[0;32m";
            ReplLineReader reader;
            interpreter.Interpret(reader, true);
        }
    }
    else if (argc == 2)
    {
        // one argument, load and execute the given script
        String fileName = argv[1];
        return InterpretFile(interpreter, fileName) ? 0 : 1;
    }
    
    return 0;
}

