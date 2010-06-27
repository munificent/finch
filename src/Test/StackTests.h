#pragma once

#include "Test.h"

namespace Finch
{
    class StackTests : public Test
    {
    public:
        static void Run();
        
    private:
        static void TestPushPop();
        static void TestCount();
        static void TestSubscript();
    };
}

