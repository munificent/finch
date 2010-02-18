#include <iostream>

#include "BlockExpr.h"
#include "Compiler.h"
#include "DefExpr.h"
#include "Environment.h"
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
    Compiler::Compiler(Environment & environment, Ref<CodeBlock> code)
    :   mEnvironment(environment),
        mCode(code)
    {
    }
    
    Ref<CodeBlock> Compiler::Compile(Environment & environment, const Expr & expr)
    {
         //### bob: hack hard-coded size
        Ref<CodeBlock> code = Ref<CodeBlock>(new CodeBlock(256));
        
        Compiler compiler = Compiler(environment, code);
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
        expr.Receiver()->Accept(*this);
        
        // compile the arguments
        for (unsigned int i = 0; i < expr.Keywords().size(); i++)
        {
            expr.Arguments()[i]->Accept(*this);
        }
        
        // get the keyword's full name
        String fullName;
        for (unsigned int i = 0; i < expr.Keywords().size(); i++)
        {
            fullName += expr.Keywords()[i];
        }
        
        int id = mEnvironment.Strings().Add(fullName);
        OpCode op = static_cast<OpCode>(OP_MESSAGE_0 + expr.Keywords().size());
        
        mCode->Write(op, id);
    }
    
    void Compiler::Visit(const NameExpr & expr)
    {
        // determine what kind of scope the name is
        // write lookup opcode with name
    }
    
    void Compiler::Visit(const NumberExpr & expr)
    {
        mCode->Write(OP_NUMBER_LITERAL, expr.Value());
    }
    
    void Compiler::Visit(const OperatorExpr & expr)
    {
        expr.Receiver()->Accept(*this);
        expr.Argument()->Accept(*this);
        
        int id = mEnvironment.Strings().Add(expr.Operator());
        mCode->Write(OP_MESSAGE_1, id);
    }
    
    void Compiler::Visit(const SequenceExpr & expr)
    {
        expr.First()->Accept(*this);
        
        // discard the first expression's return value
        mCode->Write(OP_POP);
        
        expr.Second()->Accept(*this);
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
        int id = mEnvironment.Strings().Add(expr.Value());
        mCode->Write(OP_STRING_LITERAL, id);
    }
    
    void Compiler::Visit(const UnaryExpr & expr)
    {
        expr.Receiver()->Accept(*this);
        
        int id = mEnvironment.Strings().Add(expr.Message());
        mCode->Write(OP_MESSAGE_0, id);
    }
    
    void Compiler::Visit(const UndefExpr & expr)
    {
        // push name
        // write undef
    }
}