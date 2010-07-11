#include <sstream>

#include "DynamicObject.h"
#include "FiberObject.h"
#include "FileLineReader.h"
#include "FinchParser.h"
#include "IErrorReporter.h"
#include "IInterpreterHost.h"
#include "Interpreter.h"
#include "Lexer.h"
#include "LineNormalizer.h"
#include "Process.h"

namespace Finch
{
    // Error reporter passed to the parser so that parse errors can be routed
    // through the interpreter host.
    class InterpreterErrorReporter : public IErrorReporter
    {
    public:
        InterpreterErrorReporter(Interpreter & interpreter)
        :   mInterpreter(interpreter)
        {}
        
        virtual void Error(String message)
        {
            mInterpreter.GetHost().Error(message);
        }
        
    private:
        Interpreter & mInterpreter;
    };
    
    void Interpreter::Interpret(ILineReader & reader)
    {
        Execute(Parse(reader));
    }
    
    void Interpreter::Interpret(ILineReader & reader, Process & process)
    {
        Ref<Expr> expr = Parse(reader);
        
        // bail if we failed to parse
        if (expr.IsNull()) return;
        
        // create a block for the expression
        Ref<Object> block = mEnvironment.CreateBlock(expr);
        
        // and execute the code
        Array<Ref<Object> > noArgs;
        process.CallBlock(block, noArgs);
    }
    
    Ref<Object> Interpreter::Run()
    {
        Ref<Object> result;
        
        while (!mCurrentFiber.IsNull())
        {
            FiberObject * fiber = mCurrentFiber->AsFiber();
            result = fiber->GetProcess().Execute();
        }
        
        ASSERT(!result.IsNull(), "The last fiber should have completed and returned a value.");
        
        // the last fiber's result is the result of the entire execution
        return result;
    }
    
    Ref<Expr> Interpreter::Parse(ILineReader & reader)
    {
        InterpreterErrorReporter errorReporter(*this);
        Lexer          lexer(reader, errorReporter);
        LineNormalizer normalizer(lexer);
        FinchParser    parser(normalizer, errorReporter);
        
        return parser.Parse();
    }
    
    bool Interpreter::Execute(Ref<Expr> expr)
    {        
        // bail if we failed to parse
        if (expr.IsNull()) return false;
        
        // create a block for the expression
        Ref<Object> block = mEnvironment.CreateBlock(expr);
        
        mCurrentFiber = Object::NewFiber(*this, block);
        /*
        Process process(*this, block);
        process.Execute();
        Ref<Object> result = process.GetResult();
        */
        Ref<Object> result = Run();
        
        // don't bother printing nil results
        if (result != mEnvironment.Nil())
        {
            std::stringstream text;
            text << *result << std::endl;
            mHost.Output(String(text.str().c_str()));
        }
        
        return true;
    }
    
    void Interpreter::SwitchToFiber(Ref<Object> fiber)
    {
        // pause the current fiber
        if (!mCurrentFiber.IsNull())
        {
            FiberObject * oldFiber = mCurrentFiber->AsFiber();
            oldFiber->GetProcess().Pause();
        }
        
        // jump to the new one
        mCurrentFiber = fiber;
    }

    void Interpreter::BindMethod(String objectName, String message,
                                 PrimitiveMethod method)
    {
        ASSERT_STRING_NOT_EMPTY(objectName);
        ASSERT_STRING_NOT_EMPTY(message);
        ASSERT_NOT_NULL(method);
        
        Ref<Object> object = mEnvironment.Globals()->LookUp(objectName);
        ASSERT(!object.IsNull(), "Must be an existing global variable.");

        DynamicObject* dynamicObj = object->AsDynamic();
        ASSERT_NOT_NULL(dynamicObj);
        
        dynamicObj->RegisterPrimitive(message, method);
    }
}