#pragma once

#include "IInterpreterHost.h"

namespace Finch
{
    // A basic interpreter host for running Finch as a standalone application.
    class StandaloneInterpreterHost : public IInterpreterHost
    {
    public:        
        virtual void * Allocate(size_t size);
        virtual void Free(void * data);
        
        virtual void Output(const String & text);
        virtual void Error(const String & message);
    };
}

