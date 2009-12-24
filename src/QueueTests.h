#pragma once

#ifdef UNIT_TESTS

#include "Test.h"

namespace Finch
{
    class QueueTests : public Test
    {
    public:
        static void Run();
        
    private:
        static void TestEnqueueDequeue();
        static void TestSerialEnqueue();
        static void TestMultipleEnqueue();
        static void TestCount();
        static void TestSubscript();
    };
}

#endif