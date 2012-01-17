#include <sstream>

#include "Parser.h"
#include "IErrorReporter.h"
#include "ILineReader.h"
#include "ITokenSource.h"

namespace Finch
{
    bool Parser::IsInfinite() const
    {
        return mTokens.IsInfinite();
    }

    bool Parser::LookAhead(TokenType type)
    {
        FillLookAhead(1);
        
        return mRead[0]->Type() == type;
    }
    
    bool Parser::LookAhead(TokenType current, TokenType next)
    {
        FillLookAhead(2);

        return (mRead[0]->Type() == current) &&
               (mRead[1]->Type() == next);
    }

    bool Parser::LookAhead(TokenType first, TokenType second, TokenType third)
    {
        FillLookAhead(3);
        
        return (mRead[0]->Type() == first) &&
               (mRead[1]->Type() == second) &&
               (mRead[2]->Type() == third);
    }

    bool Parser::Match(TokenType type)
    {
        if (LookAhead(type))
        {
            Consume();
            return true;
        }
        else
        {
            return false;
        }
    }
    
    void Parser::Expect(TokenType expected, const char * errorMessage)
    {
        if (!LookAhead(expected))
        {
            Error(errorMessage);
        }
    }
    
    Ref<Token> Parser::Consume()
    {
        FillLookAhead(1);
        
        return mRead.Dequeue();
    }
    
    Ref<Token> Parser::Consume(TokenType expected, const char * errorMessage)
    {
        if (LookAhead(expected))
        {
            return Consume();
        }
        else
        {
            Error(errorMessage);
            return Ref<Token>();
        }
    }
    
    void Parser::Error(const char * message)
    {
        mHadError = true;
        std::stringstream error;
        error << "Parse error on '" << Current() << "': " << message;
        mErrorReporter.Error(String(error.str().c_str()));
    }
    
    void Parser::FillLookAhead(int count)
    {
        while (mRead.Count() < count)
        {
            mRead.Enqueue(mTokens.ReadToken());
        }
    }
}

