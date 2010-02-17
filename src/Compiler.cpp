#include <iostream>

#include "BlockExpr.h"
#include "CodeBlock.h"
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
    Compiler::Compiler(Ref<CodeBlock> code)
    :   mCode(code)
    {
    }
    
    Ref<CodeBlock> Compiler::Compile(const Expr & expr)
    {
         //### bob: hack hard-coded size
        Ref<CodeBlock> code = Ref<CodeBlock>(new CodeBlock(256));
        
        Compiler compiler = Compiler(code);
        expr.Accept(compiler);
        
        return code;
    }
    
    void Compiler::Visit(const BlockExpr & expr)
    {
        // compile block to bytecode
        // put it somewhere identifiable
        // push its id onto the stack
    }
    
    void Compiler::Visit(const DefExpr & expr)
    {
        // push name
        // push value
        // write def opcode
    }
    
    void Compiler::Visit(const KeywordExpr & expr)
    {
        // push name
        // push args
        // write call
    }
    
    void Compiler::Visit(const NameExpr & expr)
    {
        // push name
        // write lookup
    }
    
    void Compiler::Visit(const NumberExpr & expr)
    {
        mCode->Write(OP_NUMBER_LITERAL, expr.Value());
    }
    
    void Compiler::Visit(const OperatorExpr & expr)
    {
        // push name
        // push args
        // write call
    }
    
    void Compiler::Visit(const SequenceExpr & expr)
    {
        // compile first
        // compile second
        //### bob: what about return value of first expr?
    }
    
    void Compiler::Visit(const SetExpr & expr)
    {
        // push name
        // push value
        // write set
        //### bob: should use different opcodes for different scopes
    }
    
    void Compiler::Visit(const StringExpr & expr)
    {
        // push string
        //### bob: probably need to intern it and push id
    }
    
    void Compiler::Visit(const UnaryExpr & expr)
    {
        // push name
        // push args
        // write call
    }
    
    void Compiler::Visit(const UndefExpr & expr)
    {
        // push name
        // write undef
    }
}