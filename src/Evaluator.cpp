#include <iostream>

#include "Evaluator.h"

#include "BlockExpr.h"
#include "KeywordExpr.h"
#include "NameExpr.h"
#include "NumberExpr.h"
#include "OperatorExpr.h"
#include "SequenceExpr.h"
#include "UnaryExpr.h"

namespace Finch
{
    void Evaluator::Evaluate(Ref<Expr> expr)
    {
        Ref<Object> result = expr->Accept(*this);
        
        if (!result.IsNull())
        {
            std::cout << "< " << *result << std::endl;
        }
        else
        {
            std::cout << "< ERROR NULL RESULT OBJECT" << std::endl;
        }
    }
    
    Ref<Object> Evaluator::Visit(const NameExpr & expr)
    {
        return Ref<Object>();
    }

    Ref<Object> Evaluator::Visit(const NumberExpr & expr)
    {
        return Object::New(expr.Value());
    }

    Ref<Object> Evaluator::Visit(const UnaryExpr & expr)
    {
        Ref<Object> receiver = expr.Receiver()->Accept(*this);
        
        return receiver->Receive(expr.Message(), vector<Ref<Object> >());
    }

    Ref<Object> Evaluator::Visit(const OperatorExpr & expr)
    {
        return Ref<Object>();
    }

    Ref<Object> Evaluator::Visit(const KeywordExpr & expr)
    {
        return Ref<Object>();
    }

    Ref<Object> Evaluator::Visit(const SequenceExpr & expr)
    {
        return Ref<Object>();
    }

    Ref<Object> Evaluator::Visit(const BlockExpr & expr)
    {
        return Ref<Object>();
    }
}