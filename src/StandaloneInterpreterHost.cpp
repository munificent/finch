#include "StandaloneInterpreterHost.h"

namespace Finch
{
    void StandaloneInterpreterHost::Output(const String & text) const
    {
        std::cout << text;
    }

    void StandaloneInterpreterHost::Error(const String & message) const
    {
        std::cout << ":( " << message << std::endl;
    }
}