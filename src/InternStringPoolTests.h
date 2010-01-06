#pragma once

#include "Test.h"

namespace Finch
{
    class InternStringPoolTests : public Test
    {
    public:
        static void Run();
        
    private:
        static void TestIntern();
    };
}
