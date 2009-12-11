#include <iostream>

#include "Evaluator.h"

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
    
    Ref<Object> Evaluator::Visit(const BlockExpr & expr)
    {
        //### bob: not implemented yet
        return Ref<Object>();
    }
    
    Ref<Object> Evaluator::Visit(const DefExpr & expr)
    {
        //### bob: what happens if you define a variable already defined in
        // this scope?
        Ref<Object> value;
        
        if (!expr.Value().IsNull())
        {
            value = expr.Value()->Accept(*this);
        }
        
        //### bob: hack temp. always define in global scope
        return mScope.Assign(expr.Name(), value);
    }
    
    Ref<Object> Evaluator::Visit(const KeywordExpr & expr)
    {
        // get the object receiving the message
        Ref<Object>          receiver = expr.Receiver()->Accept(*this);
        
        // evaluate the arguments
        String               fullName;
        vector<Ref<Object> > args;
        
        for (int i = 0; i < expr.Keywords().size(); i++)
        {
            fullName.append(expr.Keywords()[i]);
            args.push_back(expr.Arguments()[i]->Accept(*this));
        }
        
        // send the message
        return receiver->Receive(fullName, args);
    }
    
    Ref<Object> Evaluator::Visit(const NameExpr & expr)
    {
        //### bob: hack temp. always look up in global scope
        return mScope.LookUp(expr.Name());
    }

    Ref<Object> Evaluator::Visit(const NumberExpr & expr)
    {
        return Object::New(expr.Value());
    }
    
    Ref<Object> Evaluator::Visit(const OperatorExpr & expr)
    {
        Ref<Object> receiver = expr.Receiver()->Accept(*this);
        Ref<Object> arg      = expr.Argument()->Accept(*this);
        
        vector<Ref<Object> > args;
        args.push_back(arg);
        
        return receiver->Receive(expr.Operator(), args);
    }    
    
    Ref<Object> Evaluator::Visit(const SequenceExpr & expr)
    {
        // evaluate the first, discarding the result
        expr.First()->Accept(*this);
        
        return expr.Second()->Accept(*this);
    }
    
    Ref<Object> Evaluator::Visit(const SetExpr & expr)
    {
        Ref<Object> value = expr.Value()->Accept(*this);
        
        //### bob: hack temp. always set in global scope
        // should look up name to figure out which scope it's defined in and
        // set there
        return mScope.Assign(expr.Name(), value);
    }
    
    Ref<Object> Evaluator::Visit(const UnaryExpr & expr)
    {
        Ref<Object> receiver = expr.Receiver()->Accept(*this);
        
        return receiver->Receive(expr.Message(), vector<Ref<Object> >());
    }
}