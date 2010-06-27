#pragma once

#include "Test.h"

namespace Finch
{
    class ArrayTests : public Test
    {
    public:
        static void Run();
        
    private:
        static void TestCtor();
        static void TestSubscript();
        static void TestRemove();
    };
}

