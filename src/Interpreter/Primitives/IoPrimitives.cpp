#include <fstream>
#include <iostream>

#include "IoPrimitives.h"
#include "Fiber.h"
#include "Interpreter.h"
#include "Object.h"

namespace Finch
{
    using std::ifstream;
    using std::ios;
    using std::istreambuf_iterator;
    using std::string;
    
    PRIMITIVE(IoReadFile)
    {
        String path = args[0]->AsString();
        
        ifstream stream(path.CString());

        if (stream.fail())
        {
            fiber.Error(String::Format("Could not open file '%s'.", path.CString()));
            return fiber.Nil();
        }
        
        // From: http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring.
        string str;
        
        // Allocate an std::string big enough for the file.
        stream.seekg(0, ios::end);
        str.reserve(stream.tellg());
        stream.seekg(0, ios::beg);
        
        // Read it in.
        str.assign((istreambuf_iterator<char>(stream)),
                   istreambuf_iterator<char>());

        return fiber.CreateString(String(str.c_str()));
    }
}

