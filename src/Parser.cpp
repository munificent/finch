#include <iostream>

#include "Parser.h"

namespace Finch
{
    void Parser::Process(const char * line)
    {
        mLexer.Process(line);
    }
    
    void Parser::Read(Token token)
    {
        //### bob: temp
        std::cout << token << std::endl;
    }
}