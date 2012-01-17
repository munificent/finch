#pragma once

#include "Macros.h"
#include "Token.h"
#include "ITokenSource.h"

namespace Finch
{
    class ILineReader;
    
    class Lexer : public ITokenSource
    {
    public:
        Lexer(ILineReader & reader)
        :   mReader(reader),
            mNeedsLine(true),
            mPos(0),
            mStart(0)
        {}
        
        // Will be true if the line reader is.
        virtual bool IsInfinite() const;
        
        // Lexes and returns the next full Token read from the source. If the
        // ILineReader is out of lines, this will return an EOF Token.
        virtual Ref<Token> ReadToken();
        
    private:
        bool IsDone() const;
        
        char Peek(int ahead = 0) const;
        
        char Advance();
                
        void SkipBlockComment();
        Ref<Token> SingleToken(TokenType type);
        Ref<Token> ReadString();
        Ref<Token> ReadNumber();
        Ref<Token> ReadName();
        Ref<Token> ReadOperator();
        
        void AdvanceLine();
        
        bool IsWhitespace(char c) const;
        bool IsAlpha(char c) const;
        bool IsDigit(char c) const;
        bool IsOperator(char c) const;
        
        ILineReader & mReader;
        
        bool    mNeedsLine;
        String  mLine;
        int     mPos;
        int     mStart;
        
        NO_COPY(Lexer);
    };
}

