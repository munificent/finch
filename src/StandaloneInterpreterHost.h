#pragma once

#include "IInterpreterHost.h"

namespace Finch
{
    // A basic interpreter host for running Finch as a standalone application.
    class StandaloneInterpreterHost : public IInterpreterHost
    {
    public:
        virtual void Output(const String & text) const;
        virtual void Error(const String & message) const;
    };
}

