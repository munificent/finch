#include <iostream>

#include "BlockExpr.h"
#include "Compiler.h"
#include "DefExpr.h"
#include "KeywordExpr.h"
#include "NameExpr.h"
#include "NumberExpr.h"
#include "OperatorExpr.h"
#include "SequenceExpr.h"
#include "SetExpr.h"
#include "StringExpr.h"
#include "UnaryExpr.h"
#include "UndefExpr.h"

namespace Finch
{
    /*
    Ref<Object> Compiler::Evaluate(Ref<Expr> expr)
    {
        return expr->Accept(*this);
    }
    
    Ref<Object> Compiler::Visit(const BlockExpr & expr)
    {
        // compile block to bytecode
        // put it somewhere identifiable
        // push its id onto the stack
    }
    
    Ref<Object> Compiler::Visit(const DefExpr & expr)
    {
        // push name
        // push value
        // write def opcode
    }
    
    Ref<Object> Compiler::Visit(const KeywordExpr & expr)
    {
        // push name
        // push args
        // write call
    }
    
    Ref<Object> Compiler::Visit(const NameExpr & expr)
    {
        // push name
        // write lookup
    }
    
    Ref<Object> Compiler::Visit(const NumberExpr & expr)
    {
        // push value
    }
    
    Ref<Object> Compiler::Visit(const OperatorExpr & expr)
    {
        // push name
        // push args
        // write call
    }
    
    Ref<Object> Compiler::Visit(const SequenceExpr & expr)
    {
        // compile first
        // compile second
        //### bob: what about return value of first expr?
    }
    
    Ref<Object> Compiler::Visit(const SetExpr & expr)
    {
        // push name
        // push value
        // write set
        //### bob: should use different opcodes for different scopes
    }
    
    Ref<Object> Compiler::Visit(const StringExpr & expr)
    {
        // push string
        //### bob: probably need to intern it and push id
    }
    
    Ref<Object> Compiler::Visit(const UnaryExpr & expr)
    {
        // push name
        // push args
        // write call
    }
    
    Ref<Object> Compiler::Visit(const UndefExpr & expr)
    {
        // push name
        // write undef
    }
     */
}