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
        //### bob: implement me
        return Unary();
    }
    
    auto_ptr<Expr> FinchParser::Unary()
    {
        auto_ptr<Expr> object = Primary();
        if (object.get() == NULL) return ParseError();
        
        if (CurrentIs(TOKEN_NAME))
        {
            String message = Consume().Text();
            return auto_ptr<Expr>(new UnaryExpr(object, message));
        }
        else return Primary();
    }
    
    auto_ptr<Expr> FinchParser::Primary()
    {
        if (CurrentIs(TOKEN_NAME))
        {
            return auto_ptr<Expr>(new NameExpr(Consume().Text()));
        }
        else return ParseError();
    }
    
    auto_ptr<Expr> FinchParser::ParseError()
    {
        std::cout << "Parse error!" << std::endl;
        
        return auto_ptr<Expr>();
    }
}