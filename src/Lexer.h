#pragma once

#include <memory>

#include "Macros.h"
#include "Token.h"

namespace Finch
{
    using std::auto_ptr;
    
    class ITokenReader;
    
    class Lexer
    {
    public:
        Lexer()
        :   mState(LEX_DEFAULT),
            mIndex(0),
            mTokenStart(0)
        {}
        
        void StartLine(const char * line);
        
        auto_ptr<Token> ReadToken();
        
        /*
        //### bob: instead of having lexer drive parser, refactor to have
        // NextToken() function in lexer that parser repeatedly calls. that
        // way, it's easier to make recursive descent parser.
        void Process(const char * line);
        */
        
    private:
        enum State
        {
            LEX_DEFAULT,
            LEX_IN_NAME,
            LEX_IN_OPERATOR,
            LEX_AT_END
        };
        
        auto_ptr<Token> SingleToken(TokenType type);
        void            StartToken(State state);
        auto_ptr<Token> NameToken(bool condition, bool isKeyword, TokenType type);
        
        auto_ptr<Token> Emit(TokenType type);

        bool IsAlpha(char c) const;
        bool IsDigit(char c) const;
        bool IsOperator(char c) const;
        bool IsSpace(char c) const;
        bool IsNull(char c) const;
        
        State mState;
        
        const char *    mLine;
        int             mIndex;
        int             mTokenStart;
        
        NO_COPY(Lexer)
    };
}
