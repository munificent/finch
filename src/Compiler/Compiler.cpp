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
        // TODO(bob): This isn't right. It's creating a new block for each 
        // top level expression entered into the REPL. That means that variables
        // declared here won't be shared across the session. If you do:
        // >> a <- "hi"
        // >> a
        // You'll get an error. We'll have to do something smarter here.
        Array<String> params;
        return CompileBlock(environment, params, expr);
    }
    
    Ref<BlockExemplar> Compiler::CompileBlock(Environment & environment,
        const Array<String> params, const Expr & expr)
    {
        Ref<BlockExemplar> exemplar = Ref<BlockExemplar>(
            new BlockExemplar(Array<String>()));
        
        Compiler compiler(environment, exemplar);
        
        // Result register goes first.
        int resultRegister = compiler.ReserveRegister();
        
        // Reserve registers for the params.
        for (int i = 0; i < params.Count(); i++)
        {
            compiler.ReserveRegister();
            compiler.mLocals.Add(params[i]);
        }
        
        expr.Accept(compiler, resultRegister);
        exemplar->Write(OP_RETURN, resultRegister);
                
        return exemplar;
    }
    
    Compiler::Compiler(Environment & environment, Ref<BlockExemplar> exemplar)
    :   mEnvironment(environment),
        mExemplar(exemplar),
        mInUseRegisters(0),
        mLocals()
    {}
    
    void Compiler::Visit(const ArrayExpr & expr, int dest)
    {
        // Create the empty array.
        // TODO(bob): Only allows arrays up to 255 elements. Should use two
        // registers for size.
        mExemplar->Write(OP_ARRAY, expr.Elements().Count(), dest);
        
        // Write the instructions to add each item.
        int elementReg = ReserveRegister();
        for (int i = 0; i < expr.Elements().Count(); i++)
        {
            // Evaluate the element and store it in new register.
            expr.Elements()[i]->Accept(*this, elementReg);
            
            // Now add it to the array.
            mExemplar->Write(OP_ARRAY_ELEMENT, elementReg, dest);
        }
        ReleaseRegister();
    }
    
    void Compiler::Visit(const BindExpr & expr, int dest)
    {
        ASSERT(false, "Compiling BindExpr not implemented yet.");
    }
    
    void Compiler::Visit(const BlockExpr & expr, int dest)
    {
        int index = CompileNestedBlock(expr);
        mExemplar->Write(OP_BLOCK, index, dest);
        // TODO(bob): Once closures are implemented, need to write code to
        // load upvals.
    }
    
    void Compiler::Visit(const MessageExpr & expr, int dest)
    {
        // Load the receiver.
        int receiverReg = ReserveRegister();
        expr.Receiver()->Accept(*this, receiverReg);
        
        // Compile each of the message sends.
        for (int i = 0; i < expr.Messages().Count(); i++)
        {
            const MessageSend & message = expr.Messages()[i];
            
            // Compile the arguments.
            for (int arg = 0; arg < message.GetArguments().Count(); arg++)
            {
                int argReg = ReserveRegister();
                message.GetArguments()[arg]->Accept(*this, argReg);
            }
            
            // Compile the message send.
            // TODO(bob): Right now, we're only giving 8-bits to the name, which
            // will run out quickly.
            int messageId = mEnvironment.Strings().Add(message.GetName());
            OpCode op = static_cast<OpCode>(OP_MESSAGE_0 +
                message.GetArguments().Count());
            
            mExemplar->Write(op, messageId, receiverReg, dest);
            
            // Free the argument registers.
            for (int arg = 0; arg < message.GetArguments().Count(); arg++)
            {
                ReleaseRegister();
            }
        }
        
        // Free the receiver register.
        ReleaseRegister();
    }
    
    void Compiler::Visit(const NameExpr & expr, int dest)
    {
        if (Expr::IsField(expr.Name()))
        {
            // Accessing a field.
            int index = mEnvironment.Strings().Add(expr.Name());
            mExemplar->Write(OP_GET_FIELD, index, dest);
        }
        else
        {
            // It's a lexical variable name. Here, we assume that locals always
            // get allocated starting at register 1.
            int index = mLocals.IndexOf(expr.Name()) + 1;
            
            // TODO(bob): Handle unknown name.
            // TODO(bob): Handle names defined in outer scopes.
            
            // Copy the local to the destination register.
            mExemplar->Write(OP_MOVE, index, dest);
        }
    }
    
    void Compiler::Visit(const NumberExpr & expr, int dest)
    {
        Ref<Object> number = Object::NewNumber(mEnvironment, expr.GetValue());
        CompileConstant(number, dest);
    }
    
    void Compiler::Visit(const ObjectExpr & expr, int dest)
    {
        // Compile the parent.
        // TODO(bob): Enable this once closures are working so we can resolve
        // 'Object'.
        //expr.Parent()->Accept(*this, dest);
        mExemplar->Write(OP_OBJECT, dest);
        CompileDefinitions(expr, dest);
    }
    
    void Compiler::Visit(const ReturnExpr & expr, int dest)
    {
        ASSERT(false, "Compiling ReturnExpr not implemented yet.");
    }
    
    void Compiler::Visit(const SelfExpr & expr, int dest)
    {
        mExemplar->Write(OP_SELF, dest);
    }
    
    void Compiler::Visit(const SequenceExpr & expr, int dest)
    {
        // Compile each expression.
        for (int i = 0; i < expr.Expressions().Count(); i++)
        {
            // TODO(bob): Use DISCARD_REGISTER for all but last expr and make
            // compiler use that to avoid some unnecessary work.
            expr.Expressions()[i]->Accept(*this, dest);
        }
    }
    
    void Compiler::Visit(const SetExpr & expr, int dest)
    {
        ASSERT(false, "Compiling SetExpr not implemented yet.");
    }
    
    void Compiler::Visit(const StringExpr & expr, int dest)
    {
        Ref<Object> string = Object::NewString(mEnvironment, expr.GetValue());
        CompileConstant(string, dest);
    }
    
    void Compiler::Visit(const UndefineExpr & expr, int dest)
    {
        ASSERT(false, "Compiling UndefineExpr not implemented yet.");
    }
    
    void Compiler::Visit(const VarExpr & expr, int dest)
    {
        if (Expr::IsField(expr.Name()))
        {
            // Evaluate the value.
            int value = ReserveRegister();
            expr.Value()->Accept(*this, value);
            
            int name = mEnvironment.Strings().Add(expr.Name());
            mExemplar->Write(OP_SET_FIELD, name, value);
            
            // Also copy to the destination register.
            // Handles cases like: foo: _bar <- baz
            mExemplar->Write(OP_MOVE, value, dest);
            
            ReleaseRegister();
        }
        else
        {
            // Doing <- on an existing name just assigns.
            int local = mLocals.IndexOf(expr.Name());
            if (local == -1) {
                // Create a new local.
                local = ReserveRegister();
                mLocals.Add(expr.Name());
                
                // Local variable registers should always be contiguous starting
                // at register 1. NameExpr assumes that.
                ASSERT(local == mLocals.Count(),
                    "Local should be in right register.");
            }
            
            // Evaluate the value and store in the local.
            expr.Value()->Accept(*this, local);
            
            // Also copy to the destination register.
            // Handles cases like: foo: bar <- baz
            mExemplar->Write(OP_MOVE, local, dest);
        }
    }
    
    int Compiler::CompileNestedBlock(const BlockExpr & block)
    {
        Ref<BlockExemplar> exemplar = Compiler::CompileBlock(
            mEnvironment, block.Params(), *block.Body());
        return mExemplar->AddExemplar(exemplar);
    }
    
    void Compiler::CompileConstant(Ref<Object> constant, int dest)
    {
        // TODO(bob): Should check for duplicates. Only need one copy of any
        // given constant.
        int index = mExemplar->AddConstant(constant);
        mExemplar->Write(OP_CONSTANT, index, dest);
    }
    
    void Compiler::CompileDefinitions(const DefineExpr & expr, int dest)
    {
        // Compile each of the definitions.
        int count = expr.Definitions().Count();
        for (int i = 0; i < count; i++)
        {
            const Definition & definition = expr.Definitions()[i];
            int name = mEnvironment.Strings().Add(definition.GetName());
            
            if (definition.IsMethod()) {
                BlockExpr & body = static_cast<BlockExpr &>(
                    *definition.GetBody());
                
                // Create a unique id for the method.
                int exemplar = CompileNestedBlock(body);
                
                // TODO(bob): Right now, we're only giving 8-bits to the name,
                // which will run out quickly.
                mExemplar->Write(OP_DEFINE_METHOD, name, exemplar, dest);
            }
            else
            {
                // Compile the initializer.
                int value = ReserveRegister();
                definition.GetBody()->Accept(*this, value);
                mExemplar->Write(OP_DEFINE_FIELD, name, value, dest);
                ReleaseRegister();
            }
        }
    }
    
    int Compiler::ReserveRegister()
    {
        mInUseRegisters++;
        
        if (mExemplar->GetNumRegisters() < mInUseRegisters)
        {
            mExemplar->SetNumRegisters(mInUseRegisters);
        }
        
        return mInUseRegisters - 1;
    }
    
    void Compiler::ReleaseRegister()
    {
        mInUseRegisters--;
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

    void Compiler::Visit(const BindExpr & expr)
    {
        // push the first copy of the target onto the stack
        expr.Target()->Accept(*this);

        CompileDefinitions(expr);
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

