#include <iostream>

#include "ArrayObject.h"
#include "BlockObject.h"
#include "CodeBlock.h"
#include "Environment.h"
#include "Process.h"

namespace Finch
{
    using std::cout;
    using std::endl;
    
    Process::Process(Interpreter & interpreter, Environment & environment)
    :   mIsRunning(true),
        mInterpreter(interpreter),
        mEnvironment(environment),
        mLoopCode(Array<String>()),
        mDiscardCode(Array<String>())
    {
        // build the special "while loop" chunk of bytecode
        mLoopCode.Write(OP_LOOP_1);
        mLoopCode.Write(OP_LOOP_2);
        mLoopCode.Write(OP_LOOP_3);
        mLoopCode.Write(OP_LOOP_4);
        mLoopCode.Write(OP_END_BLOCK);
        
        mDiscardCode.Write(OP_POP);
        mDiscardCode.Write(OP_END_BLOCK);
    }

    Ref<Object> Process::Execute(Ref<Object> block)
    {
        // push the starting block
        mCallStack.Push(CallFrame(mEnvironment.Globals(), block));
        
        // continue processing bytecode until the entire callstack has
        // completed
        while (mCallStack.Count() > 0)
        {
            CallFrame & frame = mCallStack.Peek();
            const Instruction & instruction = frame.Block().GetCode()[frame.address];
            
            // advance past the instruction
            frame.address++;
            
            switch (instruction.op)
            {
                case OP_NOTHING:
                    // do nothing
                    break;
                    
                case OP_NUMBER_LITERAL:
                    PushOperand(Object::NewNumber(mEnvironment, instruction.arg.number));
                    break;
                    
                case OP_STRING_LITERAL:
                    {
                        String string = mEnvironment.Strings().Find(instruction.arg.id);
                        PushOperand(Object::NewString(mEnvironment, string));
                    }
                    break;
                    
                case OP_BLOCK_LITERAL:
                    {
                        // capture the current scope
                        Ref<Scope> closure = frame.scope;
                        Ref<Object> self   = frame.Block().Self();
                        
                        const CodeBlock & code = mEnvironment.Blocks().Find(instruction.arg.id);
                        Ref<Object> block = Object::NewBlock(mEnvironment, code, closure, self);
                        PushOperand(block);
                    }
                    break;
                    
                case OP_CREATE_ARRAY:
                    {
                        // create the array
                        Ref<Object> object = Object::NewArray(mEnvironment, 0);
                        ArrayObject * array = object->AsArray();
                        
                        // pop the elements
                        Array<Ref<Object> > elements;
                        for (int i = 0; i < instruction.arg.id; i++)
                        {
                            array->Elements().Add(PopOperand());
                        }
                        
                        // reverse them since the stack has them in order (so
                        // that elements are evaluated from left to right) and
                        // popping reverses the order
                        array->Elements().Reverse();
                        
                        // return the array
                        Push(object);
                    }
                    break;
                    
                case OP_POP:
                    PopOperand();
                    break;
                    
                case OP_DEF_GLOBAL:
                    {
                        // def returns the defined value, so instead of popping
                        // and then pushing the value back on the stack, we'll
                        // just peek
                        Ref<Object> value = mOperands.Peek();
                        //### bob: if we get strings fully interned (i.e. no dupes in
                        // string table), then the global name scope doesn't need the
                        // actual string at all, just the id in the string table
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        mEnvironment.Globals()->Define(name, value);
                    }
                    break;
                    
                case OP_DEF_OBJECT:
                    {
                        // def returns the defined value, so instead of popping
                        // and then pushing the value back on the stack, we'll
                        // just peek
                        Ref<Object> value = mOperands.Peek();
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        if (!Self().IsNull())
                        {
                            Self()->ObjectScope()->Define(name, value);
                        }
                    }
                    break;
                    
                case OP_DEF_LOCAL:
                    {
                        // def returns the defined value, so instead of popping
                        // and then pushing the value back on the stack, we'll
                        // just peek
                        Ref<Object> value = mOperands.Peek();
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        CurrentScope()->Define(name, value);
                    }
                    break;
                    
                case OP_UNDEF_GLOBAL:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        mEnvironment.Globals()->Undefine(name);
                        PushNil();
                    }
                    break;
                    
                case OP_UNDEF_OBJECT:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        if (!Self().IsNull())
                        {
                            Self()->ObjectScope()->Undefine(name);
                        }
                        PushNil();
                    }
                    break;
                    
                case OP_UNDEF_LOCAL:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        CurrentScope()->Undefine(name);
                        PushNil();
                    }
                    break;
                    
                case OP_SET_LOCAL:
                    {
                        // set returns the defined value, so instead of popping
                        // and then pushing the value back on the stack, we'll
                        // just peek
                        Ref<Object> value = mOperands.Peek();
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        CurrentScope()->Set(name, value);
                    }
                    break;
                    
                case OP_LOAD_GLOBAL:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        Ref<Object> value = mEnvironment.Globals()->LookUp(name);
                        PushOperand(value.IsNull() ? mEnvironment.Nil() : value);
                    }
                    break;
                    
                case OP_LOAD_OBJECT:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        if (!Self().IsNull())
                        {
                            Ref<Object> value = Self()->ObjectScope()->LookUp(name);
                            PushOperand(value.IsNull() ? mEnvironment.Nil() : value);
                        }
                        else
                        {
                            PushOperand(Ref<Object>());
                        }
                    }
                    break;
                    
                case OP_LOAD_LOCAL:
                    {
                        String name = mEnvironment.Strings().Find(instruction.arg.id);
                        Ref<Object> value = CurrentScope()->LookUp(name);
                        PushOperand(value.IsNull() ? mEnvironment.Nil() : value);
                    }
                    break;
                    
                case OP_LOAD_SELF:
                    PushOperand(Self());
                    break;
                    
                case OP_MESSAGE_0:
                case OP_MESSAGE_1:
                case OP_MESSAGE_2:
                case OP_MESSAGE_3:
                case OP_MESSAGE_4:
                case OP_MESSAGE_5:
                case OP_MESSAGE_6:
                case OP_MESSAGE_7:
                case OP_MESSAGE_8:
                case OP_MESSAGE_9:
                case OP_MESSAGE_10:
                    {
                        // pop the arguments
                        Array<Ref<Object> > args;
                        for (int i = 0; i < instruction.op - OP_MESSAGE_0; i++)
                        {
                            args.Add(PopOperand());
                        }
                        
                        // reverse them since the stack has them in order (so
                        // that arguments are evaluated from left to right) and
                        // popping reverses the order
                        args.Reverse();
                        
                        // send the message
                        String string = mEnvironment.Strings().Find(instruction.arg.id);
                        Ref<Object> receiver = PopOperand();
                        
                        receiver->Receive(receiver, *this, string, args);
                    }
                    break;
                    
                    // these next four opcodes handle the one built-in loop
                    // construct: "while". because a while loop must wait for
                    // the condition to be evaluated, and then later the body,
                    // it proceeds in stages, with an opcode for each stage.
                    //
                    // OP_LOOP_1 begins evaluating the condition expression
                    // OP_LOOP_2 checks the result of that and either ends the
                    //           loop or continues
                    // OP_LOOP_3 begins evaluating the body
                    // OP_LOOP_4 discards the result of that and loops back to
                    //           the beginning by explicitly changing the
                    //           instruction pointer
                    //
                    // note that all of this is initiated by a call to
                    // WhileLoop on the process. that pushes a special static
                    // CodeBlock that contains this sequence of opcodes. we do
                    // this, instead of compiling a while loop directly into
                    // the bytecode where it appears so that it's still
                    // possible to overload while:do: at runtime.
                    
                case OP_LOOP_1:
                    {
                        // evaluate the conditional (while leaving it on the stack)
                        Ref<Object> condition = mOperands.Peek();
                        condition->Receive(condition, *this, "call", Array<Ref<Object> >());
                    }
                    break;

                case OP_LOOP_2:
                    // if the condition is false, end the loop
                    if (PopOperand() != mEnvironment.True())
                    {
                        // pop the condition and body blocks
                        PopOperand();
                        PopOperand();
                        
                        // end the loop
                        mCallStack.Pop();
                        
                        // every expression must return something
                        PushNil();
                    }
                    break;
                    
                case OP_LOOP_3:
                    {
                        // evaluate the body
                        Ref<Object> body = mOperands[1];
                        body->Receive(body, *this, "call", Array<Ref<Object> >());
                    }
                    break;
                    
                case OP_LOOP_4:
                    // discard the body's return value
                    PopOperand();
                    
                    // restart the loop
                    mCallStack.Peek().address = 0;
                    break;
                    
                case OP_END_BLOCK:
                    mCallStack.Pop();
                    break;
                    
                default:
                    ASSERT(false, "Unknown op code.");
            }
        }
        
        // there should be one object left on the stack: the final return
        return PopOperand();
    }
    
    Ref<Object> Process::Self()
    {
        return mCallStack.Peek().Block().Self();
    }

    void Process::Push(Ref<Object> object)
    {
        PushOperand(object);
    }
    
    void Process::PushNil()
    {
        Push(mEnvironment.Nil());
    }

    void Process::PushBool(bool value)
    {
        PushOperand(value ? mEnvironment.True() : mEnvironment.False());
    }

    void Process::PushNumber(double value)
    {
        Push(Object::NewNumber(mEnvironment, value));
    }

    void Process::PushString(const String & value)
    {
        Push(Object::NewString(mEnvironment, value));
    }

    void Process::CallMethod(Ref<Object> self,
                                 Ref<Object> blockObj,
                                 const Array<Ref<Object> > & args)
    {
        BlockObject & block = *(blockObj->AsBlock());
        
        // make sure we have at least as many arguments as the block expects
        //### bob: could change to pad missing ones with
        // nil if we want to be "looser" about calling convention
        if (block.Params().Count() > args.Count())
        {
            RuntimeError(String::Format("Block expects at least %d arguments, but was passed %d.",
                                        block.Params().Count(), args.Count()));
            PushNil();
            return;
        }
        
        // create a new local scope for the block
        Ref<Scope> scope = Ref<Scope>(new Scope(block.Closure()));
        
        // bind the arguments
        for (int i = 0; i < block.Params().Count(); i++)
        {
            scope->Define(block.Params()[i], args[i]);
        }
        
        //### bob: there's something fishy here. this *should* cause a bug, but
        //    i can't seem to get it to. when a method invokes, this binds the
        //    block to the current receiver, but the previous receiver is never
        //    restored after this call is done. that should mean that if you
        //    call a method from one receiver, and within that, call the *same*
        //    method from a *different* receiver, when that second call returns
        //    the first call should still see the second receiver.
        //    but, in my tests, that doesn't seem to happen. :(
        block.RebindSelf(self);
        
        // push the call onto the stack
        mCallStack.Push(CallFrame(scope, blockObj));
    }
    
    void Process::CallBlock(Ref<Object> blockObj,
                                const Array<Ref<Object> > & args)
    {
        BlockObject & block = *(blockObj->AsBlock());
        CallMethod(block.Self(), blockObj, args);
    }
    
    void Process::WhileLoop(Ref<Object> condition, Ref<Object> body)
    {
        // push the arguments onto the stack
        Push(body);
        Push(condition);
        
        Ref<Object> block = Object::NewBlock(mEnvironment, mLoopCode,
                                             mCallStack.Peek().scope,
                                             mCallStack.Peek().Block().Self());
        
        // call our special loop "function"
        mCallStack.Push(CallFrame(mCallStack.Peek().scope, block));
    }
    
    void Process::DiscardReturn()
    {
        Ref<Object> block = Object::NewBlock(mEnvironment, mDiscardCode,
                                             mCallStack.Peek().scope,
                                             mCallStack.Peek().Block().Self());
        
        // call our special pop "function"
        mCallStack.Push(CallFrame(mCallStack.Peek().scope, block));
    }

    void Process::RuntimeError(const String & message)
    {
        //### bob: ideally, this should be programmatically configurable from
        // within Finch
        cout << "Runtime error: " << message << endl;
    }
    
    void Process::PushOperand(Ref<Object> object)
    {
        ASSERT(!object.IsNull(), "Cannot push a null object. (Should be Nil instead.)");
        
        //std::cout << "push " << object << std::endl;
        mOperands.Push(object);
    }
    
    Ref<Object> Process::PopOperand()
    {
        Ref<Object> object = mOperands.Pop();
        
        //std::cout << "pop  " << object << std::endl;
        return object;
    }
}

