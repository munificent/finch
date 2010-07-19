#pragma once

#include "FinchString.h"

namespace Finch
{
    // Interface class that provides a Finch interpreter's window into the
    // environment that is hosting it. To use a Finch interpreter, you must
    // subclass this and provide the base functionality Finch needs to run.
    class IInterpreterHost
    {
    public:
        virtual ~IInterpreterHost() {}
        
        // Override this to allocate a chunk of memory of the given size.
        virtual void * Allocate(size_t size) = 0;
        
        // Override this to deallocate the given chunk of memory.
        virtual void Free(void * data) = 0;
        
        // Override this to display the given string to the user in some way.
        virtual void Output(const String & text) = 0;
        
        // Override this to if you want to display the given error to the user
        // in some way. This will be called when a runtime error has occurred
        // in the interpreter. After a runtime error has occurred, the
        // interpreter will still be in a usable state (although possibly not
        // the state the user intended). This method is just so that the problem
        // can be displayed to the user or logged by the host.
        virtual void Error(const String & message) = 0;
    };
}
