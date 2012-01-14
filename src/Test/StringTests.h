#pragma once

#include "Test.h"

namespace Finch
{
    class StringTests : public Test
    {
    public:
        static void Run();
        
    private:
        static void TestEmpty();
        static void TestFromChar();
        static void TestLength();
        static void TestSubscript();
        static void TestAddition();
        static void TestAssignment();
        static void TestCompoundAssignment();
        static void TestComparison();
        static void TestSubstring();
        static void TestReplace();
    };
}

