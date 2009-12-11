#include <vector>

#include "FinchParser.h"

#include "BlockExpr.h"
#include "DefExpr.h"
#include "KeywordExpr.h"
#include "NameExpr.h"
#include "NumberExpr.h"
#include "OperatorExpr.h"
#include "SequenceExpr.h"
#include "SetExpr.h"
#include "UnaryExpr.h"

namespace Finch
{
    using std::vector;
    
    Ref<Expr> FinchParser::ParseLine(const char * line)
    {
        StartLine(line);
        
        return Expression();
    }
    
    Ref<Expr> FinchParser::Expression()
    {
        if (ConsumeIf(TOKEN_DEF))
        {
            if (!CurrentIs(TOKEN_NAME)) return ParseError();
            
            String name = Consume()->Text();
            
            // get the initial value
            Ref<Expr> value = Block();
            if (value.IsNull()) return ParseError();
            
            return Ref<Expr>(new DefExpr(name, value));
        }
        else if (ConsumeIf(TOKEN_SET))
        {
            if (!CurrentIs(TOKEN_NAME)) return ParseError();
            
            String name = Consume()->Text();
            
            // get the initial value
            Ref<Expr> value = Block();
            if (value.IsNull()) return ParseError();
            
            return Ref<Expr>(new SetExpr(name, value));
        }
        else return Block();
    }

    Ref<Expr> FinchParser::Block()
    {
        if (ConsumeIf(TOKEN_LEFT_BRACE))
        {
            vector<String> args;
            
            // see if there are args
            if (ConsumeIf(TOKEN_PIPE))
            {
                while (CurrentIs(TOKEN_NAME))
                {
                    args.push_back(Consume()->Text());
                }
                
                if (!ConsumeIf(TOKEN_PIPE)) return ParseError();
            }
            
            Ref<Expr> body = Expression();
            if (body.IsNull()) return ParseError();
            
            if (!ConsumeIf(TOKEN_RIGHT_BRACE)) return ParseError();
            
            return Ref<Expr>(new BlockExpr(args, body));
        }
        else return Sequence();
    }
    
    Ref<Expr> FinchParser::Sequence()
    {
        Ref<Expr> expression = Keyword();
        if (expression.IsNull()) return ParseError();
        
        while (ConsumeIf(TOKEN_SEMICOLON))
        {
            Ref<Expr> second = Keyword();
            if (second.IsNull()) return ParseError();
            
            expression = Ref<Expr>(new SequenceExpr(expression, second));
        }
        
        return expression;
    }
    
    Ref<Expr> FinchParser::Keyword()
    {
        Ref<Expr> object = Operator();
        if (object.IsNull()) return ParseError();
        
        vector<String>      keywords;
        vector<Ref<Expr> >  args;
        
        while (CurrentIs(TOKEN_KEYWORD))
        {
            String keyword = Consume()->Text();
            Ref<Expr> arg = Operator();
            if (arg.IsNull()) return ParseError();
            
            keywords.push_back(keyword);
            args.push_back(arg);
        }
        
        if (keywords.size() > 0)
        {
            object = Ref<Expr>(new KeywordExpr(object, keywords, args));
        }
        
        return object;
    }
    
    Ref<Expr> FinchParser::Operator()
    {
        Ref<Expr> object = Unary();
        if (object.IsNull()) return ParseError();
        
        while (CurrentIs(TOKEN_OPERATOR))
        {
            String op = Consume()->Text();
            Ref<Expr> arg = Unary();
            if (arg.IsNull()) return ParseError();

            object = Ref<Expr>(new OperatorExpr(object, op, arg));
        }
        
        return object;
    }
    
    Ref<Expr> FinchParser::Unary()
    {
        Ref<Expr> object = Primary();
        if (object.IsNull()) return ParseError();
        
        while (CurrentIs(TOKEN_NAME))
        {
            String message = Consume()->Text();
            object = Ref<Expr>(new UnaryExpr(object, message));
        }
        
        return object;
    }
    
    Ref<Expr> FinchParser::Primary()
    {
        if (CurrentIs(TOKEN_NAME))
        {
            return Ref<Expr>(new NameExpr(Consume()->Text()));
        }
        else if (CurrentIs(TOKEN_NUMBER))
        {
            return Ref<Expr>(new NumberExpr(Consume()->Number()));
        }
        else if (ConsumeIf(TOKEN_LEFT_PAREN))
        {
            Ref<Expr> expression = Expression();
            if (expression.IsNull()) return ParseError();
            
            if (!ConsumeIf(TOKEN_RIGHT_PAREN)) return ParseError();
            
            return expression;
        }
        else return ParseError();
    }
    
    Ref<Expr> FinchParser::ParseError()
    {
        return Ref<Expr>();
    }
}