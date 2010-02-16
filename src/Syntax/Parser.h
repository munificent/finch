#pragma once

#include "Macros.h"
#include "Lexer.h"
#include "Queue.h"
#include "Ref.h"

namespace Finch
{
    class ILineReader;
    class ITokenSource;
    
    // Base class for a generic recursive descent parser.
    class Parser
    {
    protected:
        Parser(ITokenSource & tokens)
        :   mTokens(tokens)
        {}
        
        // Gets the Token the parser is currently looking at.
        Ref<Token> Current() { return mRead[0]; }
        
        // Returns true if the current Token is the given type.
        bool LookAhead(TokenType type);
        
        // Returns true if the current and next Tokens is the given types (in
        // order).
        bool LookAhead(TokenType current, TokenType next);
        
        // Consumes the current Token and returns true if it is the given type,
        // otherwise returns false.
        bool Match(TokenType type);
        
        // Consumes the current Token and advances the Parser.
        Ref<Token> Consume();

    private:
        void FillLookAhead(int count);
        
        ITokenSource & mTokens;
        
        Queue<Ref<Token>, 2> mRead;
        
        NO_COPY(Parser);
    };
}