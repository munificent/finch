#include <stdarg.h>

#include "IErrorReporter.h"
#include "ILineReader.h"
#include "LexerTests.h"
#include "Lexer.h"

namespace Finch
{
    class FixedLineReader : public ILineReader
    {
    public:
        FixedLineReader(const char * line)
        : mLine(line)
        {}
        
        virtual bool IsInfinite() const { return false; }
        virtual bool EndOfLines() const { return mLine == NULL; }
        
        virtual String NextLine()
        {
            String line = mLine;
            // only read the line once
            mLine = NULL;
            
            return line;
        }
        
    private:
        const char * mLine;
    };

    void LexerTests::Run()
    {
        // test the single character tokens
        TestLex("()[]{}.|,;",
            TOKEN_LEFT_PAREN,
            TOKEN_RIGHT_PAREN,
            TOKEN_LEFT_BRACKET,
            TOKEN_RIGHT_BRACKET,
            TOKEN_LEFT_BRACE,
            TOKEN_RIGHT_BRACE,
            TOKEN_DOT,
            TOKEN_PIPE,
            TOKEN_LINE, // ,
            TOKEN_SEMICOLON,
            TOKEN_LINE, TOKEN_EOF);
        
        // test comments
        TestLex("() // this is ignored",
                TOKEN_LEFT_PAREN,
                TOKEN_RIGHT_PAREN,
                TOKEN_LINE, TOKEN_EOF);
        
        // test the reserved words
        TestLex("= ->",
            TOKEN_EQ,
            TOKEN_ARROW,
            TOKEN_LINE, TOKEN_EOF);
        
        // test numbers
        TestLex("1 0 23467 -2347 -1",
                TOKEN_NUMBER,
                TOKEN_NUMBER,
                TOKEN_NUMBER,
                TOKEN_NUMBER,
                TOKEN_NUMBER,
                TOKEN_LINE, TOKEN_EOF);
        
        EXPECT_EQUAL(0,    LexOne("0")->Number());
        EXPECT_EQUAL(1,    LexOne("1")->Number());
        EXPECT_EQUAL(1234, LexOne("1234")->Number());
        EXPECT_EQUAL(-1,   LexOne("-1")->Number());
        
        // test strings
        TestLex("\"\" \"foo\"",
                TOKEN_STRING,
                TOKEN_STRING,
                TOKEN_LINE, TOKEN_EOF);
        
        EXPECT_EQUAL("",        LexOne("\"\"")->Text());
        EXPECT_EQUAL("a",       LexOne("\"a\"")->Text());
        EXPECT_EQUAL("foo",     LexOne("\"foo\"")->Text());
        EXPECT_EQUAL("fo\\o",   LexOne("\"fo\\\\o\"")->Text());
        EXPECT_EQUAL("\"\n\\",  LexOne("\"\\\"\\n\\\\\"")->Text());

        // test identifiers
        TestLex("_a foo BarBang &foo fo9o!",
                TOKEN_NAME,
                TOKEN_NAME,
                TOKEN_NAME,
                TOKEN_NAME,
                TOKEN_NAME,
                TOKEN_LINE, TOKEN_EOF);

        TestLex("- + = / < > ? ~ ! $ % ^ & * *$-+-==",
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_OPERATOR,
                TOKEN_LINE, TOKEN_EOF);

        TestLex("foo: barBangBaz: :",
                TOKEN_KEYWORD,
                TOKEN_KEYWORD,
                TOKEN_KEYWORD,
                TOKEN_LINE, TOKEN_EOF);
        
        // test combinations
        TestLex("foo[123]+(*\"huh\")",
                TOKEN_NAME,
                TOKEN_LEFT_BRACKET,
                TOKEN_NUMBER,
                TOKEN_RIGHT_BRACKET,
                TOKEN_OPERATOR,
                TOKEN_LEFT_PAREN,
                TOKEN_OPERATOR,
                TOKEN_STRING,
                TOKEN_RIGHT_PAREN,
                TOKEN_LINE, TOKEN_EOF);
        
        // edge cases:
        
        TestLex("<---",
                TOKEN_OPERATOR,
                TOKEN_LINE, TOKEN_EOF);
    }
    
    Ref<Token> LexerTests::LexOne(const char * text)
    {
        FixedLineReader reader(text);
        Lexer lexer(reader);
        
        return lexer.ReadToken();
    }
    
    void LexerTests::TestLex(const char * text, ...)
    {
        FixedLineReader reader(text);
        Lexer lexer(reader);
        
        va_list args;
        va_start(args, text);
        
        while (true)
        {
            TokenType type = static_cast<TokenType>(va_arg(args, int));
            Ref<Token> token = lexer.ReadToken();
            EXPECT_EQUAL(type, token->Type());
            
            if (type == TOKEN_EOF) break;
        }

        va_end(args);
    }
}

