#include <vector>

#include "FinchParser.h"

#include "BlockExpr.h"
#include "DefExpr.h"
#include "ILineReader.h"
#include "KeywordExpr.h"
#include "NameExpr.h"
#include "NumberExpr.h"
#include "OperatorExpr.h"
#include "SequenceExpr.h"
#include "SetExpr.h"
#include "StringExpr.h"
#include "UnaryExpr.h"

namespace Finch
{
    using std::vector;
    
    Ref<Expr> FinchParser::ParseFile()
    {
        // since expression includes sequence expressions, this will parse
        // as many lines as we have
        return Expression();
    }
    
    Ref<Expr> FinchParser::ParseLine()
    {
        // skip past Sequence() otherwise we'll keep reading lines forever
        Ref<Expr> expr = Variable();
        
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
        Ref<Expr> expression = Variable();
        if (expression.IsNull()) return ParseError();
        
        while (Match(TOKEN_LINE))
        {
            // there may be a trailing line after the last expression in a
            // block. if we eat the line and then see a closing brace or eof,
            // just stop here.
            if (LookAhead(TOKEN_RIGHT_BRACE)) break;
            if (LookAhead(TOKEN_EOF)) break;
            
            Ref<Expr> second = Variable();
            if (second.IsNull()) return ParseError("Expect expression after ';'.");
            
            expression = Ref<Expr>(new SequenceExpr(expression, second));
        }
        
        return expression;
    }
    
    Ref<Expr> FinchParser::Variable()
    {
        if (LookAhead(TOKEN_NAME, TOKEN_ARROW))
        {
            String name = Consume()->Text();
            
            Consume(); // the arrow
            
            // get the initial value
            Ref<Expr> value = Keyword();
            if (value.IsNull()) return ParseError();
            
            return Ref<Expr>(new DefExpr(name, value));
        }
        else if (LookAhead(TOKEN_NAME, TOKEN_LONG_ARROW))
        {
            String name = Consume()->Text();
            
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
        
        Ref<Expr> keyword = KeywordMessage(object);
        if (!keyword.IsNull()) return keyword;
        
        return object;
    }
    
    Ref<Expr> FinchParser::Operator()
    {
        Ref<Expr> object = Unary();
        if (object.IsNull()) return ParseError();
        
        while (LookAhead(TOKEN_OPERATOR))
        {
            String op = Consume()->Text();
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
            String message = Consume()->Text();
            object = Ref<Expr>(new UnaryExpr(object, message));
        }
        
        return object;
    }
    
    Ref<Expr> FinchParser::Primary()
    {
        if (LookAhead(TOKEN_NAME))
        {
            return Ref<Expr>(new NameExpr(Consume()->Text()));
        }
        else if (LookAhead(TOKEN_NUMBER))
        {
            return Ref<Expr>(new NumberExpr(Consume()->Number()));
        }
        else if (LookAhead(TOKEN_STRING))
        {
            return Ref<Expr>(new StringExpr(Consume()->Text()));
        }
        else if (LookAhead(TOKEN_KEYWORD))
        {
            // implicit receiver keyword message
            return KeywordMessage(Ref<Expr>(new NameExpr("Ether")));
        }
        else if (Match(TOKEN_DOT))
        {
            return Ref<Expr>(new NameExpr("self"));
        }
        else if (Match(TOKEN_LEFT_PAREN))
        {
            Ref<Expr> expression = Expression();
            if (expression.IsNull()) return ParseError("Expect expression after '('.");
            
            if (!Match(TOKEN_RIGHT_PAREN)) return ParseError("Expect closing ')'.");
            
            return expression;
        }
        if (Match(TOKEN_LEFT_BRACE))
        {
            vector<String> args;
            
            // see if there are args
            if (Match(TOKEN_PIPE))
            {
                while (LookAhead(TOKEN_NAME))
                {
                    args.push_back(Consume()->Text());
                }
                
                if (!Match(TOKEN_PIPE)) return ParseError("Expect closing '|' after block arguments.");
                
                // if there were no named args, but there were pipes (||),
                // use an automatic "it" arg
                if (args.size() == 0) args.push_back("it");
            }
            
            Ref<Expr> body = Expression();
            if (body.IsNull()) return ParseError("Expect expression body inside block.");
            
            if (!Match(TOKEN_RIGHT_BRACE)) return ParseError("Expect closing '}' after block.");
            
            return Ref<Expr>(new BlockExpr(args, body));
        }
        else return ParseError("Couldn't parse primary expression.");
    }
    
    // Parses just the message send part of a keyword message: "foo: a bar: b"
    Ref<Expr> FinchParser::KeywordMessage(Ref<Expr> object)
    {
        vector<String>      keywords;
        vector<Ref<Expr> >  args;
        
        while (LookAhead(TOKEN_KEYWORD))
        {
            String keyword = Consume()->Text();
            Ref<Expr> arg = Operator();
            if (arg.IsNull()) return ParseError("Expect argument after keyword.");
            
            keywords.push_back(keyword);
            args.push_back(arg);
        }
        
        if (keywords.size() > 0)
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
        std::cout << message;
        
        if (Current().IsNull())
        {
            std::cout << " (got null token)" << std::endl;
        }
        else
        {
            std::cout << " (got: " << *Current() << ")" << std::endl;
        }
        
        return Ref<Expr>();
    }
}