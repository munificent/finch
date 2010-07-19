#include <new>

#include "StandaloneInterpreterHost.h"

namespace Finch
{        
    void * StandaloneInterpreterHost::Allocate(size_t size)
    {
        ASSERT(false, "Not implemented yet.");
        return NULL;
    }
    
    void StandaloneInterpreterHost::Free(void * data)
    {
        ASSERT(false, "Not implemented yet.");
    }
    
    void StandaloneInterpreterHost::Output(const String & text)
    {
        std::cout << text;
    }

    void StandaloneInterpreterHost::Error(const String & message)
    {
        std::cout << ":( " << message << std::endl;
    }
}
