#include <iostream>

#include "ConsolePrimitives.h"

namespace Finch
{
    Ref<Object> ConsoleWrite(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args)
    {
        String text = args[0]->AsString();
        std::cout << text;
        
        return Ref<Object>();
    }
    
    Ref<Object> ConsoleWriteLine(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args)
    {
        String text = args[0]->AsString();
        std::cout << text << std::endl;
        
        return Ref<Object>();
    }
}
