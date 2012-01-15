#include <iostream>

#include "ArrayExpr.h"
#include "BindExpr.h"
#include "BlockExpr.h"
#include "Compiler.h"
#include "Interpreter.h"
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
    int Compiler::sNextMethodId = 1;
    
    Ref<Block> Compiler::CompileTopLevel(Interpreter & interpreter, const Expr & expr)
    {
        Array<String> params;
        Compiler compiler(interpreter, NULL);
        compiler.Compile(Block::BLOCK_METHOD_ID, params, expr);
        
        /*
        // TODO(bob): Testing!
        compiler.mBlock->DebugDump(environment, "");
        */
        
        return compiler.mBlock;
    }
        
    Compiler::Compiler(Interpreter & interpreter, Compiler * parent)
    :   mInterpreter(interpreter),
        mParent(parent),
        mBlock(),
        mInUseRegisters(0),
        mLocals(),
        mObjectLiterals(),
        mHasReturn(false)
    {}

    void Compiler::Compile(int methodId, const Array<String> & params,
                           const Expr & expr)
    {
        mBlock = Ref<Block>(new Block(methodId, params));
        
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
        // END instruction.
        // Result register goes after params.
        int resultRegister = ReserveRegister();
        // TODO(bob): Hackish. Add a fake local for it so that the indices in
        // mLocals correctly map local names -> register.
        mLocals.Add("(return)");
        
        expr.Accept(*this, resultRegister);
        
        // TODO(bob): Not enabled yet because the VM doesn't support it.
        // Need to figure out how it's going to work with the register window
        // stuff for passing arguments.
        /*
        if (!mHasReturn) mBlock->MarkTailCall();
        */
        
        mBlock->Write(OP_END, resultRegister);
        
        // Now that all upvalues for this block are known (and its contained
        // blocks have also been compiled, which due to closure flattening may
        // upvalues to this block), we can store the number of upvalues.
        mBlock->SetNumUpvalues(mUpvalues.Count());
    }
    
    void Compiler::Visit(const ArrayExpr & expr, int dest)
    {
        // Create the empty array.
        // TODO(bob): Only allows arrays up to 255 elements. Should use two
        // registers for size.
        mBlock->Write(OP_ARRAY, expr.Elements().Count(), dest);
        
        // Write the instructions to add each item.
        int elementReg = ReserveRegister();
        for (int i = 0; i < expr.Elements().Count(); i++)
        {
            // Evaluate the element and store it in new register.
            expr.Elements()[i]->Accept(*this, elementReg);
            
            // Now add it to the array.
            mBlock->Write(OP_ARRAY_ELEMENT, elementReg, dest);
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
        CompileNestedBlock(Block::BLOCK_METHOD_ID, expr, dest);
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
            StringId messageId = mInterpreter.AddString(message.GetName());
            OpCode op = static_cast<OpCode>(OP_MESSAGE_0 +
                message.GetArguments().Count());
            
            mBlock->Write(op, messageId, receiverReg, dest);
            
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
            int index = mInterpreter.DefineGlobal(expr.Name());
            mBlock->Write(OP_GET_GLOBAL, index, dest);
        }
        else if (Expr::IsField(expr.Name()))
        {
            // Accessing a field.
            StringId index = mInterpreter.AddString(expr.Name());
            mBlock->Write(OP_GET_FIELD, index, dest);
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
                mBlock->Write(OP_MOVE, index, dest);
            }
            else if (resolvedUpvalue.IsValid())
            {
                // Load the upvalue into the destination register.
                mBlock->Write(OP_GET_UPVALUE, resolvedUpvalue.Slot(), dest);
            }
            else
            {
                // We couldn't find the name, so assume it's a global. This
                // allows for mutually recursive references to top-level names:
                // as long as the name is initialized before it's actually
                // accessed at runtime, this will work.
                int index = mInterpreter.DefineGlobal(expr.Name());
                mBlock->Write(OP_GET_GLOBAL, index, dest);
            }
        }
    }
        
    void Compiler::Visit(const NumberExpr & expr, int dest)
    {
        Ref<Object> number = mInterpreter.NewNumber(expr.GetValue());
        CompileConstant(number, dest);
    }
    
    void Compiler::Visit(const ObjectExpr & expr, int dest)
    {
        // Compile the parent. It will go into the same register that we'll
        // put the new object into.
        expr.Parent()->Accept(*this, dest);
        mBlock->Write(OP_OBJECT, dest);
        
        // Keep track of the fact that we're inside an object literal.
        mObjectLiterals.Push(dest);
        CompileDefinitions(expr, dest);
        mObjectLiterals.Pop();
    }
    
    void Compiler::Visit(const ReturnExpr & expr, int dest)
    {
        Compiler * method = GetEnclosingMethod();
        if (method == NULL)
        {
            // TODO(bob): Come up with real compile error reporting system.
            ASSERT(false, "Can't return out of a top-level block.");
        }
        
        // Compile the return value.
        expr.Result()->Accept(*this, dest);
        
        mBlock->Write(OP_RETURN, method->mBlock->MethodId(), dest);
        
        // Disable tail calls for the method.
        method->mHasReturn = true;
    }
    
    void Compiler::Visit(const SelfExpr & expr, int dest)
    {
        // If we're inside an object literal, `self` is statically bound to
        // the enclosing object.
        if (mObjectLiterals.Count() > 0)
        {
            int selfReg = mObjectLiterals.Peek();
            mBlock->Write(OP_MOVE, selfReg, dest);
        }
        else
        {
            // Do a normal dynamic lookup on `self`.
            mBlock->Write(OP_SELF, dest);
        }
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
                expr.Value()->Accept(*this, dest);
                
                // Store the upvalue.
                mBlock->Write(OP_SET_UPVALUE, resolvedUpvalue.Slot(), dest);
            }
            else
            {
                // Evaluate the value.
                expr.Value()->Accept(*this, dest);
                
                // See if a global with this name exists.
                int index = mInterpreter.FindGlobal(expr.Name());
                
                // TODO(bob): Report compile error for undefined global.
                if (index != -1)
                {
                    // We're compiling a top-level expression, so define it as a global.
                    mBlock->Write(OP_SET_GLOBAL, index, dest);
                }
            }
        }
    }
    
    void Compiler::Visit(const StringExpr & expr, int dest)
    {
        Ref<Object> string = mInterpreter.NewString(expr.GetValue());
        CompileConstant(string, dest);
    }
    
    void Compiler::Visit(const UndefineExpr & expr, int dest)
    {
        // TODO(bob): Hackish. Undefine expressions will probably go away. This
        // implementation doesn't totally work (for one thing, it doesn't
        // assign anything to dest, and it isn't clear what it *should* assign),
        // but it gets the test to pass.
        int local = mLocals.IndexOf(expr.Name());
        if (local != -1)
        {
            mLocals[local] = "";
        }
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
            mBlock->Write(OP_MOVE, local, dest);
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
        value.Accept(*this, dest);
        
        // We're compiling a top-level expression, so define it as a global.
        int index = mInterpreter.DefineGlobal(name);
        mBlock->Write(OP_SET_GLOBAL, index, dest);
    }
    
    void Compiler::CompileSetField(const String & name, const Expr & value, int dest)
    {
        value.Accept(*this, dest);
        
        StringId nameId = mInterpreter.AddString(name);
        mBlock->Write(OP_SET_FIELD, nameId, dest);
    }

    void Compiler::CompileNestedBlock(int methodId, const BlockExpr & block, int dest)
    {
        Compiler compiler(mInterpreter, this);
        compiler.Compile(methodId, block.Params(), *block.Body());
        
        int index = mBlock->AddBlock(compiler.mBlock);
        
        mBlock->Write(OP_BLOCK, index, dest);

        // Capture the upvalues.
        for (int i = 0; i < compiler.mUpvalues.Count(); i++)
        {
            Upvalue & upvalue = compiler.mUpvalues[i];
            if (upvalue.IsLocal())
            {
                // Closing over a local.
                mBlock->Write(OP_CAPTURE_LOCAL, upvalue.Index());
            }
            else
            {
                // Closing over an upvalue.
                mBlock->Write(OP_CAPTURE_UPVALUE, upvalue.Index());
            }
        }
    }
    
    void Compiler::CompileConstant(Ref<Object> constant, int dest)
    {
        // TODO(bob): Should check for duplicates. Only need one copy of any
        // given constant.
        int index = mBlock->AddConstant(Value::HackWrapRef(constant));
        mBlock->Write(OP_CONSTANT, index, dest);
    }
    
    void Compiler::CompileDefinitions(const DefineExpr & expr, int dest)
    {
        // Compile each of the definitions.
        int count = expr.Definitions().Count();
        for (int i = 0; i < count; i++)
        {
            const Definition & definition = expr.Definitions()[i];
            StringId name = mInterpreter.AddString(definition.GetName());
            
            int value = ReserveRegister();

            if (definition.IsMethod()) {
                BlockExpr & body = static_cast<BlockExpr &>(
                    *definition.GetBody());
                
                CompileNestedBlock(sNextMethodId++, body, value);
                
                // TODO(bob): Right now, we're only giving 8-bits to the name,
                // which will run out quickly.
                mBlock->Write(OP_DEF_METHOD, name, value, dest);
            }
            else
            {
                // Compile the initializer.
                definition.GetBody()->Accept(*this, value);
                mBlock->Write(OP_DEF_FIELD, name, value, dest);
            }
            
            ReleaseRegister();
        }
    }
    
    Compiler * Compiler::GetEnclosingMethod()
    {
        Compiler * compiler = this;
        while ((compiler != NULL) &&
            (compiler->mBlock->MethodId() == Block::BLOCK_METHOD_ID))
        {
            compiler = compiler->mParent;
        }
        
        return compiler;
    }
    
    int Compiler::ReserveRegister()
    {
        mInUseRegisters++;
        
        if (mBlock->NumRegisters() < mInUseRegisters)
        {
            mBlock->SetNumRegisters(mInUseRegisters);
        }
        
        return mInUseRegisters - 1;
    }
    
    void Compiler::ReleaseRegister()
    {
        mInUseRegisters--;
    }
}

