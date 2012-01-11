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
    Ref<BlockExemplar> Compiler::CompileTopLevel(Environment & environment, const Expr & expr)
    {
        Array<String> params;
        Compiler compiler(environment, NULL);
        compiler.Compile(params, expr);
        
        /*
        // TODO(bob): Testing!
        compiler.mExemplar->DebugDump(environment, "");
        */
        
        return compiler.mExemplar;
    }
        
    Compiler::Compiler(Environment & environment, Compiler * parent)
    :   mEnvironment(environment),
        mParent(parent),
        mExemplar(),
        mInUseRegisters(0),
        mLocals()
    {}

    void Compiler::Compile(const Array<String> & params, const Expr & expr)
    {
        mExemplar = Ref<BlockExemplar>(new BlockExemplar(params));
        
        // Reserve registers for the params. These have to go first because the
        // caller will place them here.
        for (int i = 0; i < params.Count(); i++)
        {
            ReserveRegister();
            mLocals.Add(params[i]);
        }
        
        // TODO(bob): Instead of having an explicit register for the return,
        // which is often redundant since the result will be in some previous
        // register anyway many times, maybe pass a special DONT_CARE register
        // and have the visitors know to provide a register as needed and then
        // set some mReturnRegister member variable that we then use for the
        // RETURN instruction.
        // Result register goes after params.
        int resultRegister = ReserveRegister();
        // TODO(bob): Hackish. Add a fake local for it so that the indices in
        // mLocals correctly map local names -> register.
        mLocals.Add("(return)");
        
        expr.Accept(*this, resultRegister);
        mExemplar->Write(OP_RETURN, resultRegister);
        
        // Now that all upvalues for this block are known (and its contained
        // blocks have also been compiled, which due to closure flattening may
        // upvalues to this block), we can store the number of upvalues.
        mExemplar->SetNumUpvalues(mUpvalues.Count());
    }
    
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
        // Evaluate the object that stuff is being bound to.
        expr.Target()->Accept(*this, dest);
        
        // Bind the definitions.
        CompileDefinitions(expr, dest);
    }
    
    void Compiler::Visit(const BlockExpr & expr, int dest)
    {
        CompileNestedBlock(expr, dest);
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
        if (mParent == NULL)
        {
            // Accessing a top-level name, so it's a global.
            int index = mEnvironment.DefineGlobal(expr.Name());
            mExemplar->Write(OP_GET_GLOBAL, index, dest);
        }
        else if (Expr::IsField(expr.Name()))
        {
            // Accessing a field.
            int index = mEnvironment.Strings().Add(expr.Name());
            mExemplar->Write(OP_GET_FIELD, index, dest);
        }
        else
        {
            Upvalue upvalue;
            bool isLocal;
            int index;
            Upvalue resolvedUpvalue;
            ResolveName(this, expr.Name(), &upvalue, &isLocal, &index,
                        &resolvedUpvalue);
            
            if (isLocal)
            {
                // Copy the local to the destination register.
                mExemplar->Write(OP_MOVE, index, dest);
            }
            else if (resolvedUpvalue.IsValid())
            {
                // Load the upvalue into the destination register.
                mExemplar->Write(OP_GET_UPVALUE, resolvedUpvalue.Slot(), dest);
            }
            else
            {
                // We couldn't find the name, so assume it's a global. This
                // allows for mutually recursive references to top-level names:
                // as long as the name is initialized before it's actually
                // accessed at runtime, this will work.
                int index = mEnvironment.DefineGlobal(expr.Name());
                mExemplar->Write(OP_GET_GLOBAL, index, dest);
            }
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
        if (mParent == NULL)
        {
            // Globals behave the same with <- and <--.
            CompileSetGlobal(expr.Name(), *expr.Value(), dest);
        }
        else if (Expr::IsField(expr.Name()))
        {
            // Fields behave the same with <- and <--.
            CompileSetField(expr.Name(), *expr.Value(), dest);
        }
        else
        {
            Upvalue upvalue;
            bool isLocal;
            int index;
            Upvalue resolvedUpvalue;
            ResolveName(this, expr.Name(), &upvalue, &isLocal, &index,
                        &resolvedUpvalue);
            
            if (isLocal)
            {
                // Evaluate the value directly into the local.
                expr.Value()->Accept(*this, index);
            }
            else if (resolvedUpvalue.IsValid())
            {
                // Evaluate the value.
                int valueReg = ReserveRegister();
                expr.Value()->Accept(*this, valueReg);
                
                // Store the upvalue.
                mExemplar->Write(OP_SET_UPVALUE, resolvedUpvalue.Slot(), valueReg);
                
                // Also copy to the destination register.
                // Handles cases like: foo: some-upval <- baz
                mExemplar->Write(OP_MOVE, valueReg, dest);
                
                ReleaseRegister();
            }
            else
            {
                CompileSetGlobal(expr.Name(), *expr.Value(), dest);
            }
        }
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
        if (mParent == NULL)
        {
            // We're at the top level, so it's a global.
            CompileSetGlobal(expr.Name(), *expr.Value(), dest);
        }
        else if (Expr::IsField(expr.Name()))
        {
            CompileSetField(expr.Name(), *expr.Value(), dest);
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
                // at register 0. NameExpr assumes that.
                ASSERT(local == mLocals.Count() - 1,
                    "Local should be in right register.");
            }
            
            // Evaluate the value and store in the local.
            expr.Value()->Accept(*this, local);
            
            // Also copy to the destination register.
            // Handles cases like: foo: bar <- baz
            mExemplar->Write(OP_MOVE, local, dest);
        }
    }
    
    // TODO(bob): The number of args here is a bit hairy. Once SetExpr is
    // implemented and using this, see if we can clean it up a bit.
    void Compiler::ResolveName(Compiler * compiler, const String & name,
                               Upvalue * outUpvalue, bool * outIsLocal,
                               int * outIndex, Upvalue * outResolvedUpvalue)
    {
        // Bail if we run out of scopes.
        if (compiler == NULL)
        {
            // TODO(bob): Support globals. For now, just resolve it to an
            // invalid Upvalue.
            *outIsLocal = false;
            *outResolvedUpvalue = Upvalue();
            
            // Set to an invalid empty upvalue.
            *outUpvalue = Upvalue();
            return;
        }
        
        // See if the name is defined here.
        int local = compiler->mLocals.IndexOf(name);
        if (local != -1)
        {
            if (compiler == this)
            {
                // It's defined in the current scope, so resolve it as local.
                *outIsLocal = true;
                *outIndex = local;
            }
            
            *outUpvalue = Upvalue(true, local);
            return;
        }
        
        // Recurse upwards.
        // TODO(bob): Can we just reuse outUpvalue?
        Upvalue upvalue;
        ResolveName(compiler->mParent, name, &upvalue, outIsLocal, outIndex,
                    outResolvedUpvalue);
        
        // Just unwind if we never found the name in any scope.
        if (!upvalue.IsValid())
        {
            *outUpvalue = upvalue;
            return;
        }
        
        // Add an upvalue to this scope. This flattens the closure: if we refer
        // to a variable defined outside of our immediately enclosing block,
        // each intervening block will copy that variable into its upvalues so
        // we can walk it down to the block that uses it.
        upvalue.SetSlot(compiler->mUpvalues.Count());
        compiler->mUpvalues.Add(upvalue);
        
        if (compiler == this)
        {
            // Resolve the name as an upvar.
            *outIsLocal = false;
            *outResolvedUpvalue = upvalue;
        }
        
        *outUpvalue = Upvalue(false, upvalue.Slot());
    }
    
    void Compiler::CompileSetGlobal(const String & name, const Expr & value, int dest)
    {
        // Evaluate the value.
        int valueReg = ReserveRegister();
        value.Accept(*this, valueReg);
        
        // We're compiling a top-level expression, so define it as a global.
        int index = mEnvironment.DefineGlobal(name);
        mExemplar->Write(OP_SET_GLOBAL, index, valueReg);
        
        // Also copy to the destination register.
        // Handles cases like: foo: some-global <- baz
        mExemplar->Write(OP_MOVE, valueReg, dest);
        
        ReleaseRegister();
    }
    
    void Compiler::CompileSetField(const String & name, const Expr & value, int dest)
    {
        int valueReg = ReserveRegister();
        value.Accept(*this, valueReg);
        
        int nameId = mEnvironment.Strings().Add(name);
        mExemplar->Write(OP_SET_FIELD, nameId, valueReg);
        
        // Also copy to the destination register.
        // Handles cases like: foo: _bar <- baz
        mExemplar->Write(OP_MOVE, valueReg, dest);
        
        ReleaseRegister();
    }

    void Compiler::CompileNestedBlock(const BlockExpr & block, int dest)
    {
        Compiler compiler(mEnvironment, this);
        compiler.Compile(block.Params(), *block.Body());
        
        int index = mExemplar->AddExemplar(compiler.mExemplar);
        
        mExemplar->Write(OP_BLOCK, index, dest);

        // Capture the upvalues.
        for (int i = 0; i < compiler.mUpvalues.Count(); i++)
        {
            Upvalue & upvalue = compiler.mUpvalues[i];
            if (upvalue.IsLocal())
            {
                // Closing over a local.
                mExemplar->Write(OP_CAPTURE_LOCAL, upvalue.Index());
            }
            else
            {
                // Closing over an upvalue.
                mExemplar->Write(OP_CAPTURE_UPVALUE, upvalue.Index());
            }
        }
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
            
            int value = ReserveRegister();

            if (definition.IsMethod()) {
                BlockExpr & body = static_cast<BlockExpr &>(
                    *definition.GetBody());
                
                // TODO(bob): Create a unique id for the method.
                CompileNestedBlock(body, value);
                
                // TODO(bob): Right now, we're only giving 8-bits to the name,
                // which will run out quickly.
                mExemplar->Write(OP_DEF_METHOD, name, value, dest);
            }
            else
            {
                // Compile the initializer.
                definition.GetBody()->Accept(*this, value);
                mExemplar->Write(OP_DEF_FIELD, name, value, dest);
            }
            
            ReleaseRegister();
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

