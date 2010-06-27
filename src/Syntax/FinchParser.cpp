#include "ArrayExpr.h"
#include "BlockExpr.h"
#include "DefExpr.h"
#include "FinchParser.h"
#include "ILineReader.h"
#include "KeywordExpr.h"
#include "NameExpr.h"
#include "NumberExpr.h"
#include "OperatorExpr.h"
#include "SelfExpr.h"
#include "SequenceExpr.h"
#include "SetExpr.h"
#include "StringExpr.h"
#include "UnaryExpr.h"
#include "UndefineExpr.h"

namespace Finch
{
    Ref<Expr> FinchParser::ParseFile()
    {
        // since expression includes sequence expressions, this will parse
        // as many lines as we have
        return Expression();
    }
    
    Ref<Expr> FinchParser::ParseLine()
    {
        // skip past Sequence() otherwise we'll keep reading lines forever
        Ref<Expr> expr = Assignment();
        
        // eat any trailing line
        Match(TOKEN_LINE);
        
        return expr;
    }
    
    Ref<Expr> FinchParser::Expression()
    {
        Ref<Expr> expr = Sequence();
        
        // eat any trailing line
        Match(TOKEN_LINE);
        
        return expr;
    }
    
    Ref<Expr> FinchParser::Sequence()
    {
        Array<Ref<Expr> > expressions;
        
        ParseSequence(expressions);
        
        if (expressions.Count() == 0) return ParseError("Expected sequence.");
        
        // if there's just one, don't wrap it in a sequence
        if (expressions.Count() == 1) return expressions[0];
        
        return Ref<Expr>(new SequenceExpr(expressions));
    }
    
    Ref<Expr> FinchParser::Assignment()
    {
        if (LookAhead(TOKEN_NAME, TOKEN_ARROW, TOKEN_UNDEFINED))
        {
            String name = Consume().Text();
            
            Consume(); // the arrow
            Consume(); // undefined
            
            return Ref<Expr>(new UndefineExpr(name));
        }
        else if (LookAhead(TOKEN_NAME, TOKEN_ARROW))
        {
            String name = Consume().Text();
            
            Consume(); // the arrow
            
            // get the initial value
            Ref<Expr> value = Keyword();
            if (value.IsNull()) return ParseError();
            
            return Ref<Expr>(new DefExpr(name, value));
        }
        else if (LookAhead(TOKEN_NAME, TOKEN_LONG_ARROW))
        {
            String name = Consume().Text();
            
            Consume(); // the arrow
            
            // get the initial value
            Ref<Expr> value = Keyword();
            if (value.IsNull()) return ParseError();
            
            return Ref<Expr>(new SetExpr(name, value));
        }
        else return Keyword();
    }
    
    Ref<Expr> FinchParser::Keyword()
    {
        Ref<Expr> object = Operator();
        if (object.IsNull()) return ParseError();
        
        Ref<Expr> keyword = ParseKeyword(object);
        if (!keyword.IsNull()) return keyword;
        
        return object;
    }
    
    Ref<Expr> FinchParser::Operator()
    {
        Ref<Expr> object = Unary();
        if (object.IsNull()) return ParseError();
        
        while (LookAhead(TOKEN_OPERATOR))
        {
            String op = Consume().Text();
            Ref<Expr> arg = Unary();
            if (arg.IsNull()) return ParseError("Expect expression after operator.");

            object = Ref<Expr>(new OperatorExpr(object, op, arg));
        }
        
        return object;
    }
    
    Ref<Expr> FinchParser::Unary()
    {
        Ref<Expr> object = Primary();
        if (object.IsNull()) return ParseError();
        
        while (LookAhead(TOKEN_NAME))
        {
            String message = Consume().Text();
            object = Ref<Expr>(new UnaryExpr(object, message));
        }
        
        return object;
    }
    
    Ref<Expr> FinchParser::Primary()
    {
        if (LookAhead(TOKEN_NAME))
        {
            return Ref<Expr>(new NameExpr(Consume().Text()));
        }
        else if (LookAhead(TOKEN_NUMBER))
        {
            return Ref<Expr>(new NumberExpr(Consume().Number()));
        }
        else if (LookAhead(TOKEN_STRING))
        {
            return Ref<Expr>(new StringExpr(Consume().Text()));
        }
        else if (LookAhead(TOKEN_KEYWORD))
        {
            // implicit receiver keyword message
            return ParseKeyword(Ref<Expr>(new NameExpr("Ether")));
        }
        //### getting rid of this for now to possibly free it up for some other
        // use
        /*
        else if (Match(TOKEN_DOT))
        {
            return Ref<Expr>(new NameExpr("self"));
        }*/
        else if (Match(TOKEN_SELF))
        {
            return Ref<Expr>(new SelfExpr());
        }
        else if (Match(TOKEN_LEFT_PAREN))
        {
            Ref<Expr> expression = Expression();
            if (expression.IsNull()) return ParseError("Expect expression after '('.");
            
            if (!Match(TOKEN_RIGHT_PAREN)) return ParseError("Expect closing ')'.");
            
            return expression;
        }
        else if (Match(TOKEN_LEFT_BRACKET))
        {
            Array<Ref<Expr> > expressions;
            
            // allow zero-element arrays
            if (!LookAhead(TOKEN_RIGHT_BRACKET))
            {
                ParseSequence(expressions);
            }
            
            if (!Match(TOKEN_RIGHT_BRACKET)) return ParseError("Expect closing ']'.");
            
            return Ref<Expr>(new ArrayExpr(expressions));
        }
        else if (Match(TOKEN_LEFT_BRACE))
        {
            Array<String> args;
            
            // see if there are args
            if (Match(TOKEN_PIPE))
            {
                while (LookAhead(TOKEN_NAME))
                {
                    args.Add(Consume().Text());
                }
                
                if (!Match(TOKEN_PIPE)) return ParseError("Expect closing '|' after block arguments.");
                
                // if there were no named args, but there were pipes (||),
                // use an automatic "it" arg
                if (args.Count() == 0) args.Add("it");
            }
            
            Ref<Expr> body = Expression();
            if (body.IsNull()) return ParseError("Expect expression body inside block.");
            
            if (!Match(TOKEN_RIGHT_BRACE)) return ParseError("Expect closing '}' after block.");
            
            return Ref<Expr>(new BlockExpr(args, body));
        }
        else return ParseError("Couldn't parse primary expression.");
    }
    
    void FinchParser::ParseSequence(Array<Ref<Expr> > & expressions)
    {
        Ref<Expr> expression = Assignment();
        //### bob: need error reporting
        if (expression.IsNull()) return;
        
        expressions.Add(expression);
        
        while (Match(TOKEN_LINE))
        {
            // there may be a trailing line after the last expression in a
            // block. if we eat the line and then see a closing brace or eof,
            // just stop here.
            if (LookAhead(TOKEN_RIGHT_PAREN)) break;
            if (LookAhead(TOKEN_RIGHT_BRACKET)) break;
            if (LookAhead(TOKEN_RIGHT_BRACE)) break;
            if (LookAhead(TOKEN_EOF)) break;
            
            Ref<Expr> next = Assignment();
            //### bob: need error reporting
            if (next.IsNull()) return;
            
            expressions.Add(next);
        }
    }

    // Parses just the message send part of a keyword message: "foo: a bar: b"
    Ref<Expr> FinchParser::ParseKeyword(Ref<Expr> object)
    {
        Array<String>      keywords;
        Array<Ref<Expr> >  args;
        
        while (LookAhead(TOKEN_KEYWORD))
        {
            String keyword = Consume().Text();
            Ref<Expr> arg = Operator();
            if (arg.IsNull()) return ParseError("Expect argument after keyword.");
            
            keywords.Add(keyword);
            args.Add(arg);
        }
        
        if (keywords.Count() > 0)
        {
            return Ref<Expr>(new KeywordExpr(object, keywords, args));
        }
        
        return Ref<Expr>();
    }
    
    Ref<Expr> FinchParser::ParseError()
    {
        return Ref<Expr>();
    }
    
    Ref<Expr> FinchParser::ParseError(const char * message)
    {
        std::cout << message << " (got: " << Current() << ")" << std::endl;
        
        return Ref<Expr>();
    }
}

