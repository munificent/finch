#pragma once

#include "Macros.h"
#include "Lexer.h"
#include "Ref.h"

namespace Finch
{
    class ILineReader;
    class ITokenSource;
    
    // Base class for a generic recursive descent parser.
    class Parser
    {
    protected:
        Parser(ITokenSource * tokens)
        :   mTokens(tokens)
        {}
        
        // Gets the Token the parser is currently looking at.
        Ref<Token> Current() { return mCurrent; }
        
        // Returns true if the current Token is the given type.
        bool CurrentIs(TokenType type);
        
        // Consumes the current Token and returns true if it is the given type,
        // otherwise returns false.
        bool ConsumeIf(TokenType type);
        
        // Consumes the current Token and advances the Parser.
        Ref<Token> Consume();

    private:
        ITokenSource * mTokens;
        Ref<Token>     mCurrent;
        
        NO_COPY(Parser)
    };
}