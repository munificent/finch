#pragma once

#include "Array.h"
#include "Macros.h"
#include "Lexer.h"
#include "Queue.h"

namespace Finch
{
    class IErrorReporter;
    class ILineReader;
    class ITokenSource;
    
    // Base class for a generic recursive descent parser.
    class Parser
    {
    protected:
        Parser(ITokenSource & tokens, IErrorReporter & errorReporter)
        :   mTokens(tokens),
            mErrorReporter(errorReporter),
            mCanBacktrack(false),
            mHadError(false)
        {}
        
        virtual ~Parser() {}
        
        // Gets whether or not the token source for this parser is infinite.
        bool IsInfinite() const;
        
        // Gets the Token the parser is currently looking at.
        const Token & Current() { return *mRead[0]; }
        
        // Returns true if the current Token is the given type.
        bool LookAhead(TokenType type);

        // Returns true if the current and next Tokens is the given types (in
        // order).
        bool LookAhead(TokenType current, TokenType next);
        
        // Returns true if the tokens are the given types, in order.
        bool LookAhead(TokenType first, TokenType second, TokenType third);

        // Returns true if the Token [ahead] tokens ahead of the current one is
        // the given type.
        bool LookAhead(int ahead, TokenType type);

        // Consumes the current Token and returns true if it is the given type,
        // otherwise returns false.
        bool Match(TokenType type);
        
        // Verifies the current Token if it matches the expected type, and
        // reports an error if it doesn't. Does not consume the token either
        // way.
        void Expect(TokenType expected, const char * errorMessage);
        
        // Consumes the current Token and advances the Parser.
        Ref<Token> Consume();
        
        // Consumes the current Token if it matches the expected type.
        // Otherwise reports the given error message and returns a null Ref.
        Ref<Token> Consume(TokenType expected, const char * errorMessage);

        // Reports the given error message relevant to the current token.
        void Error(const char * message);

        // Gets whether or not any errors have been reported.
        bool HadError() const { return mHadError; }
        
    private:
        void FillLookAhead(int count);
        
        ITokenSource & mTokens;

        Queue<Ref<Token> > mRead;
        
        IErrorReporter & mErrorReporter;
        bool mCanBacktrack;
        bool mHadError;
        
        NO_COPY(Parser);
    };
}

