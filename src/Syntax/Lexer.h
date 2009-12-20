#pragma once

#include "Macros.h"
#include "Ref.h"
#include "Token.h"
#include "ITokenSource.h"

namespace Finch
{
    class ILineReader;
    
    class Lexer : public ITokenSource
    {
    public:
        Lexer(ILineReader * reader)
        :   mReader(reader),
            mState(LEX_NOT_STARTED),
            mIndex(0),
            mTokenStart(0)
        {}
        
        virtual Ref<Token> ReadToken();
        
    private:
        enum State
        {
            LEX_NOT_STARTED,
            LEX_DEFAULT,
            LEX_IN_NUMBER,
            LEX_IN_NAME,
            LEX_IN_OPERATOR,
            LEX_IN_STRING,
            LEX_IN_COMMENT,
            LEX_AT_END_OF_LINE,
            LEX_NEED_NEW_LINE,
            LEX_DONE
        };
        
        void StartLine();
        
        Ref<Token> SingleToken(TokenType type);
        void       StartToken(State state, bool skipFirstChar = false);
        Ref<Token> TextToken(bool condition, bool isKeyword, TokenType type);
        
        Ref<Token> EmitNumber(TokenType type);
        Ref<Token> EmitText(TokenType type);

        bool IsAlpha(char c) const;
        bool IsDigit(char c) const;
        bool IsOperator(char c) const;
        bool IsSpace(char c) const;
        bool IsNull(char c) const;
        
        ILineReader * mReader;
        State         mState;
        
        String        mLine;
        int           mIndex;
        int           mTokenStart;
        
        NO_COPY(Lexer)
    };
}
