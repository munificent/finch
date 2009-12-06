#pragma once

#include "Macros.h"
#include "Token.h"

namespace Finch
{
    class ITokenReader;
    
    class Lexer
    {
    public:
        Lexer(ITokenReader * reader)
        :   mState(LEX_DEFAULT),
            mIndex(0),
            mTokenStart(0),
            mReader(reader)
        {
            ASSERT_NOT_NULL(reader);
        }
        
        void Process(const char * line);
        
    private:
        enum State
        {
            LEX_DEFAULT,
            LEX_IN_NAME,
            LEX_IN_OPERATOR
        };
        
        void SingleToken(TokenType type);
        void StartToken(State state);
        void NameToken(bool condition, bool isKeyword, TokenType type);
        
        void Emit(TokenType type);

        bool IsAlpha(char c) const;
        bool IsDigit(char c) const;
        bool IsOperator(char c) const;
        bool IsSpace(char c) const;
        bool IsNull(char c) const;
        
        State mState;
        
        const char *    mLine;
        int             mIndex;
        int             mTokenStart;
        ITokenReader *  mReader;
        
        NO_COPY(Lexer)
    };
}
