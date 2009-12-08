#include "FinchParser.h"

#include "KeywordExpr.h"
#include "NameExpr.h"
#include "OperatorExpr.h"
#include "UnaryExpr.h"

namespace Finch
{
    auto_ptr<Expr> FinchParser::ParseLine(const char * line)
    {
        StartLine(line);
        
        return Expression();
        /*
        while (true)
        {
            const Token & token = Consume();
            
            if (token.Type() == TOKEN_EOF) break;
            
            //### bob: temp
            std::cout << token << std::endl;
        }*/
    }
    
    auto_ptr<Expr> FinchParser::Expression()
    {
        return Keyword();
    }
    
    auto_ptr<Expr> FinchParser::Keyword()
    {
        //### bob: implement me
        return Operator();
    }
    
    auto_ptr<Expr> FinchParser::Operator()
    {
        // a + b + c + d
        // (((a + b) + c) + d)
        auto_ptr<Expr> object = Unary();
        if (object.get() == NULL) return ParseError();
        
        while (CurrentIs(TOKEN_OPERATOR))
        {
            String op = Consume()->Text();
            auto_ptr<Expr> arg = Unary();
            if (arg.get() == NULL) return ParseError();

            object = auto_ptr<Expr>(new OperatorExpr(object, op, arg));
        }
        
        return object;
    }
    
    auto_ptr<Expr> FinchParser::Unary()
    {
        auto_ptr<Expr> object = Primary();
        if (object.get() == NULL) return ParseError();
        
        while (CurrentIs(TOKEN_NAME))
        {
            String message = Consume()->Text();
            object = auto_ptr<Expr>(new UnaryExpr(object, message));
        }
        
        return object;
    }
    
    auto_ptr<Expr> FinchParser::Primary()
    {
        if (CurrentIs(TOKEN_NAME))
        {
            return auto_ptr<Expr>(new NameExpr(Consume()->Text()));
        }
        else return ParseError();
    }
    
    auto_ptr<Expr> FinchParser::ParseError()
    {
        std::cout << "Parse error!" << std::endl;
        
        return auto_ptr<Expr>();
    }
}