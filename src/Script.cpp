#include <iostream>
#include <string>

#include "Environment.h"
#include "FileLineReader.h"
#include "FinchParser.h"
#include "FinchString.h"
#include "Interpreter.h"
#include "LineNormalizer.h"
#include "Object.h"
#include "Process.h"
#include "Scope.h"
#include "Script.h"
#include "StandaloneInterpreterHost.h"

namespace Finch
{
    /*
    void Script::Execute(String fileName, Interpreter & interpreter)
    {
        FileLineReader reader(fileName);
        
        if (reader.EndOfLines())
        {
            // couldn't open
            std::cout << "Couldn't open file \"" << fileName << "\"" << std::endl;
            return;
        }
        
        interpreter.Execute(reader, true, false);
    }*/
}

