#include <iostream>

#include "ArrayExpr.h"
#include "BindExpr.h"
#include "BlockExpr.h"
#include "Compiler.h"
#include "Environment.h"
#include "MessageExpr.h"
#include "NameExpr.h"
#include "NumberExpr.h"
#include "ObjectExpr.h"
#include "ReturnExpr.h"
#include "SelfExpr.h"
#include "SequenceExpr.h"
#include "SetExpr.h"
#include "StringExpr.h"
#include "UndefineExpr.h"
#include "VarExpr.h"

namespace Finch
{
    Ref<BlockExemplar> Compiler::CompileExpression(Environment & environment, const Expr & expr)
    {
        Ref<BlockExemplar> exemplar = Ref<BlockExemplar>(
            new BlockExemplar(Array<String>()));
        
        // TODO(bob): Temp!
        int constant = exemplar->AddConstant(
            Object::NewString(environment, "constant!"));
        exemplar->Write(OP_CONSTANT, constant, 0);
        exemplar->Write(OP_RETURN, 0);
        exemplar->SetNumRegisters(1);
        
        return exemplar;
    }
    
    /*
    int Compiler::sNextMethodId = 1;
    
    Ref<CodeBlock> Compiler::CompileTopLevel(Environment & environment, const Expr & expr)
    {
        Compiler compiler = Compiler(environment, NULL);
        compiler.Compile(Array<String>(), expr, 0);
        
        return compiler.mCode;
    }
    
    Compiler::Compiler(Environment & environment, Compiler * parent)
    :   mEnvironment(environment),
        mCode(),
        mParent(parent),
        mHasReturn(false)
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
        mCode->Write(OP_CREATE_ARRAY, expr.Elements().Count());
    }

    void Compiler::Visit(const BindExpr & expr)
    {
        // push the first copy of the target onto the stack
        expr.Target()->Accept(*this);

        CompileDefinitions(expr);
    }

    void Compiler::Visit(const BlockExpr & expr)
    {
        int id = CompileBlock(expr, 0);
        mCode->Write(OP_BLOCK, id);
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
                mCode->Write(OP_DUP);
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
            mCode->Write(op, id);

            // if we're cascading and have another send coming, discard the
            // return of this message.
            if (i < messageCount - 1)
            {
                mCode->Write(OP_POP);
            }
        }
    }

    void Compiler::Visit(const NameExpr & expr)
    {
        int id = mEnvironment.Strings().Add(expr.Name());
        if (Expr::IsField(expr.Name()))
        {
            mCode->Write(OP_LOAD_FIELD, id);
        }
        else
        {
            mCode->Write(OP_LOAD_LOCAL, id);
        }
    }

    void Compiler::Visit(const NumberExpr & expr)
    {
        // TODO(bob): Should check for duplicates. Only need one copy of any
        // given constant.
        Ref<Object> number = Object::NewNumber(mEnvironment, expr.Value());
        int index = mCode->AddConstant(number);
        mCode->Write(OP_CONSTANT, index);
    }

    void Compiler::Visit(const ObjectExpr & expr)
    {
        expr.Parent()->Accept(*this);
        mCode->Write(OP_START_OBJECT);
        CompileDefinitions(expr);
        mCode->Write(OP_END_OBJECT);
    }

    void Compiler::Visit(const ReturnExpr & expr)
    {
        expr.Result()->Accept(*this);
        if (expr.IsReturn()) {
            mCode->Write(OP_RETURN, GetEnclosingMethodId());
            SetHasReturn();
        } else {
            mCode->Write(OP_END_BLOCK);
        }
    }

    void Compiler::Visit(const SelfExpr & expr)
    {
        mCode->Write(OP_LOAD_SELF);
    }

    void Compiler::Visit(const SequenceExpr & expr)
    {
        for (int i = 0; i < expr.Expressions().Count(); i++)
        {
            expr.Expressions()[i]->Accept(*this);

            // discard all but the last expression's return value
            if (i < expr.Expressions().Count() - 1) mCode->Write(OP_POP);
        }
    }

    void Compiler::Visit(const SetExpr & expr)
    {
        expr.Value()->Accept(*this);

        int id = mEnvironment.Strings().Add(expr.Name());
        if (Expr::IsField(expr.Name()))
        {
            mCode->Write(OP_SET_FIELD, id);
        }
        else
        {
            mCode->Write(OP_SET_LOCAL, id);
        }
    }

    void Compiler::Visit(const StringExpr & expr)
    {
        // TODO(bob): Should check for duplicates. Only need one copy of any
        // given constant.
        Ref<Object> string = Object::NewString(mEnvironment, expr.Value());
        int index = mCode->AddConstant(string);
        mCode->Write(OP_CONSTANT, index);
    }

    void Compiler::Visit(const UndefineExpr & expr)
    {
        int id = mEnvironment.Strings().Add(expr.Name());
        if (Expr::IsField(expr.Name()))
        {
            mCode->Write(OP_UNDEF_FIELD, id);
        }
        else
        {
            mCode->Write(OP_UNDEF_LOCAL, id);
        }
    }

    void Compiler::Visit(const VarExpr & expr)
    {
        expr.Value()->Accept(*this);
        
        int id = mEnvironment.Strings().Add(expr.Name());
        if (Expr::IsField(expr.Name()))
        {
            mCode->Write(OP_DEF_FIELD, id);
        }
        else
        {
            mCode->Write(OP_DEF_LOCAL, id);
        }
    }
    
    void Compiler::Compile(const Array<String> & params, const Expr & body, int methodId)
    {
        mCode = Ref<CodeBlock>(new CodeBlock(params, methodId));
        body.Accept(*this);
        mCode->Write(OP_END_BLOCK);
    }
    
    int Compiler::CompileBlock(const BlockExpr & expr, int methodId)
    {
        Compiler compiler = Compiler(mEnvironment, this);
        compiler.Compile(expr.Params(), *expr.Body(), methodId);
        
        // if the method contains any non-local returns, we can't eliminate it
        // on a tail call because it needs to stay on the stack so the
        // non-local return can find it when unwinding.
        if ((methodId == 0) || !compiler.mHasReturn)
        {
            compiler.mCode->MarkTailCalls();
        }
        
        return mCode->AddCodeBlock(compiler.mCode);
    }
    
    void Compiler::CompileDefinitions(const DefineExpr & expr)
    {
        // compile each of the definitions
        int count = expr.Definitions().Count();
        for (int i = 0; i < count; i++)
        {
            const Definition & definition = expr.Definitions()[i];
            int id = mEnvironment.Strings().Add(definition.GetName());
            
            if (definition.IsMethod()) {
                BlockExpr & body = static_cast<BlockExpr &>(
                    *definition.GetBody());
                // create a unique id for the method
                int blockId = CompileBlock(body, sNextMethodId++);
                
                // TODO(bob): Hackish. Cram the name id and code id into int.
                int arg = (blockId & 0xffff) | ((id & 0xffff) << 16);
                
                mCode->Write(OP_BIND_METHOD, arg);
            }
            else
            {
                // compile the body
                definition.GetBody()->Accept(*this);
                mCode->Write(OP_BIND_OBJECT, id);
            }
        }
    }
    
    Compiler * Compiler::GetEnclosingMethod()
    {
        Compiler * compiler = this;
        while ((compiler != NULL) && (compiler->mCode->MethodId() == 0))
        {
            compiler = compiler->mParent;
        }
        
        return compiler;
    }
    
    int Compiler::GetEnclosingMethodId()
    {
        Compiler * compiler = GetEnclosingMethod();
        if (compiler == NULL) return 0;

        return compiler->mCode->MethodId();
    }

    void Compiler::SetHasReturn()
    {
        Compiler * compiler = GetEnclosingMethod();
        
        if (compiler != NULL)
        {
            compiler->mHasReturn = true;
        }
    }
     */
}

