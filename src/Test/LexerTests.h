#pragma once

#include "Test.h"

namespace Finch
{
    class Token;
    
    class LexerTests : public Test
    {
    public:
        static void Run();
        
    private:
        static Token LexOne(const char * text);
        static void TestLex(const char * text, ...);
    };
}

