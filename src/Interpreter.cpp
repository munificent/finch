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
#include "Fiber.h"

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
    
    void Interpreter::Interpret(ILineReader & reader, bool showResult)
    {
        Ref<Expr> expr = Parse(reader);
        
        // Bail if we failed to parse.
        if (expr.IsNull()) return;
        
        // Create a starting fiber for the expression.
        Ref<Object> block = mEnvironment.CreateBlock(expr);
        SwitchToFiber(Object::NewFiber(*this, block));
        
        // Run the interpreter.
        Ref<Object> result = Run();
        
        if (showResult)
        {
            std::stringstream text;
            text << *result << std::endl;
            mHost.Output(String(text.str().c_str()));
        }
    }
    
    Ref<Object> Interpreter::Run()
    {
        Ref<Object> result;

        while (!mCurrentFiber.IsNull())
        {
            FiberObject * fiber = mCurrentFiber->AsFiber();
            result = fiber->GetFiber().Execute();
            
            // if we finished the fiber, then switch back to the previous one
            if (fiber->GetFiber().IsDone())
            {
                // forget the old fiber completely
                mCurrentFiber = Ref<Object>();
                
                // and switch back to the previous one
                if (!mLastFiber.IsNull())
                {
                    // push the ending fiber's final value to the fiber we're
                    // switching to. this lets the dying fiber pass its result
                    // as the resuming fiber's return value from "run".
                    // TODO(bob): Figure out how to get this working with
                    // registers.
                    /*
                    mLastFiber->AsFiber()->GetFiber().Push(result);
                    */
                    
                    SwitchToFiber(mLastFiber);
                }
            }
        }

        ASSERT(!result.IsNull(), "The last fiber should have completed and returned a value.");
        
        // the last fiber's result is the result of the entire execution
        return result;
    }
    
    Ref<Expr> Interpreter::Parse(ILineReader & reader)
    {
        InterpreterErrorReporter errorReporter(*this);
        Lexer          lexer(reader);
        LineNormalizer normalizer(lexer);
        FinchParser    parser(normalizer, errorReporter);
        
        return parser.Parse();
    }
    
    void Interpreter::SwitchToFiber(Ref<Object> fiber)
    {
        mLastFiber = mCurrentFiber;
        
        // pause the current fiber
        if (!mCurrentFiber.IsNull())
        {
            FiberObject * oldFiber = mCurrentFiber->AsFiber();
            oldFiber->GetFiber().Pause();
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
        
        int globalIndex = mEnvironment.DefineGlobal(objectName);
        Ref<Object> object = mEnvironment.GetGlobal(globalIndex);
        ASSERT(!object.IsNull(), "Must be an existing global variable.");

        DynamicObject* dynamicObj = object->AsDynamic();
        ASSERT_NOT_NULL(dynamicObj);
        
        int messageId = mEnvironment.Strings().Add(message);
        dynamicObj->AddPrimitive(messageId, method);
    }
}
