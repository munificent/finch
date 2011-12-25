#pragma once

#include "Ref.h"
#include "Test.h"

namespace Finch
{
    class Token;
    
    class LexerTests : public Test
    {
    public:
        static void Run();
        
    private:
        static Ref<Token> LexOne(const char * text);
        static void TestLex(const char * text, ...);
    };
}

