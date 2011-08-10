#include <iostream>

#include "ArrayExpr.h"
#include "BindExpr.h"
#include "BlockExpr.h"
#include "Compiler.h"
#include "Environment.h"
#include "MessageExpr.h"
#include "NameExpr.h"
#include "NumberExpr.h"
#include "SelfExpr.h"
#include "SequenceExpr.h"
#include "SetExpr.h"
#include "StringExpr.h"
#include "UndefineExpr.h"
#include "VarExpr.h"

namespace Finch
{
    void Compiler::Compile(Environment & environment, const Expr & expr,
                           CodeBlock & code)
    {
        Compiler compiler = Compiler(environment, code);
        expr.Accept(compiler);
        
        code.Write(OP_END_BLOCK);
    }
    
    Compiler::Compiler(Environment & environment, CodeBlock & code)
    :   mEnvironment(environment),
        mCode(code)
    {
    }
    
    void Compiler::Visit(const ArrayExpr & expr)
    {
        // evaluate and push all of the elements onto the stack
        for (int i = 0; i < expr.Elements().Count(); i++)
        {
            expr.Elements()[i]->Accept(*this);
        }
        
        // pop them into an array object
        mCode.Write(OP_CREATE_ARRAY, expr.Elements().Count());
    }
    
    void Compiler::Visit(const BindExpr & expr)
    {
        // push the first copy of the target onto the stack
        expr.Target()->Accept(*this);
        
        // compile each of the definitions
        int methodCount = expr.Methods().Count();
        for (int i = 0; i < methodCount; i++)
        {
            // push another copy of the target on the stack for the this
            // definition to use so that the first copy is still there for
            // the next one.
            if (i < methodCount - 1)
            {
                mCode.Write(OP_DUP);
            }
            
            const Definition & method = expr.Methods()[i];
            
            // compile the method body
            method.GetBody()->Accept(*this);
            
            // compile the method definition
            int id = mEnvironment.Strings().Add(method.GetName());
            mCode.Write(OP_BIND_METHOD, id);
            
            // if we have another define coming, discard the return of this one.
            if (i < methodCount - 1)
            {
                mCode.Write(OP_POP);
            }
        }
    }
    
    void Compiler::Visit(const BlockExpr & expr)
    {
        int id = mEnvironment.Blocks().Add(expr.Params(), *expr.Body(), mEnvironment);
        mCode.Write(OP_BLOCK_LITERAL, id);
    }
    
    void Compiler::Visit(const MessageExpr & expr)
    {
        // push the first copy of the receiver onto the stack
        expr.Receiver()->Accept(*this);
        
        // compile each of the message sends
        int messageCount = expr.Messages().Count();
        for (int i = 0; i < messageCount; i++)
        {
            // push another copy of the receiver on the stack for the this
            // message send to use so that the first copy is still there for
            // the next send.
            if (i < messageCount - 1)
            {
                mCode.Write(OP_DUP);
            }
            
            const MessageSend & message = expr.Messages()[i];
            
            // compile the arguments
            for (int arg = 0; arg < message.GetArguments().Count(); arg++)
            {
                message.GetArguments()[arg]->Accept(*this);
            }
            
            // compile the message send
            int id = mEnvironment.Strings().Add(message.GetName());
            OpCode op = static_cast<OpCode>(OP_MESSAGE_0 + message.GetArguments().Count());
            mCode.Write(op, id);

            // if we're cascading and have another send coming, discard the
            // return of this message.
            if (i < messageCount - 1)
            {
                mCode.Write(OP_POP);
            }
        }
    }
    
    void Compiler::Visit(const NameExpr & expr)
    {
        int id = mEnvironment.Strings().Add(expr.Name());
        switch (Expr::GetNameScope(expr.Name()))
        {
            case NAMESCOPE_GLOBAL: mCode.Write(OP_LOAD_GLOBAL, id); break;
            case NAMESCOPE_OBJECT: mCode.Write(OP_LOAD_OBJECT, id); break;
            case NAMESCOPE_LOCAL:  mCode.Write(OP_LOAD_LOCAL, id); break;
        }
    }
    
    void Compiler::Visit(const NumberExpr & expr)
    {
        mCode.Write(OP_NUMBER_LITERAL, expr.Value());
    }
    
    void Compiler::Visit(const SelfExpr & expr)
    {
        mCode.Write(OP_LOAD_SELF);
    }

    void Compiler::Visit(const SequenceExpr & expr)
    {
        for (int i = 0; i < expr.Expressions().Count(); i++)
        {
            expr.Expressions()[i]->Accept(*this);
            
            // discard all but the last expression's return value
            if (i < expr.Expressions().Count() - 1) mCode.Write(OP_POP);
        }
    }
    
    void Compiler::Visit(const SetExpr & expr)
    {
        expr.Value()->Accept(*this);
        
        switch (Expr::GetNameScope(expr.Name()))
        {
            case NAMESCOPE_GLOBAL:
            case NAMESCOPE_OBJECT:
                //### bob: need error-handling (or have the parser
                // disallow this). should not be able to <-- on
                // globals or object vars
                break;
                
            case NAMESCOPE_LOCAL:
                int id = mEnvironment.Strings().Add(expr.Name());
                mCode.Write(OP_SET_LOCAL, id);
                break;
        }
    }
    
    void Compiler::Visit(const StringExpr & expr)
    {
        // push string
        int id = mEnvironment.Strings().Add(expr.Value());
        mCode.Write(OP_STRING_LITERAL, id);
    }
    
    void Compiler::Visit(const UndefineExpr & expr)
    {
        int id = mEnvironment.Strings().Add(expr.Name());
        switch (Expr::GetNameScope(expr.Name()))
        {
            case NAMESCOPE_GLOBAL: mCode.Write(OP_UNDEF_GLOBAL, id); break;
            case NAMESCOPE_OBJECT: mCode.Write(OP_UNDEF_OBJECT, id); break;
            case NAMESCOPE_LOCAL:  mCode.Write(OP_UNDEF_LOCAL, id); break;
        }
    }
    
    void Compiler::Visit(const VarExpr & expr)
    {
        expr.Value()->Accept(*this);
        
        int id = mEnvironment.Strings().Add(expr.Name());
        switch (Expr::GetNameScope(expr.Name()))
        {
            case NAMESCOPE_GLOBAL:
                mCode.Write(OP_DEF_GLOBAL, id);
                break;
                
            case NAMESCOPE_OBJECT:
                mCode.Write(OP_DEF_OBJECT, id);
                break;
                
            case NAMESCOPE_LOCAL:
                mCode.Write(OP_DEF_LOCAL, id);
                break;
        }
    }
}

