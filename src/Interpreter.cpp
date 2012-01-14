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
        Ref<Object> fiber = Object::NewFiber(*this, block);
        
        // Run the interpreter.
        Ref<Object> result = fiber->AsFiber()->GetFiber().Execute();
        
        if (showResult)
        {
            std::stringstream text;
            text << *result << std::endl;
            mHost.Output(String(text.str().c_str()));
        }
    }
        
    Ref<Expr> Interpreter::Parse(ILineReader & reader)
    {
        InterpreterErrorReporter errorReporter(*this);
        Lexer          lexer(reader);
        LineNormalizer normalizer(lexer);
        FinchParser    parser(normalizer, errorReporter);
        
        return parser.Parse();
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
        
        StringId messageId = mEnvironment.Strings().Add(message);
        dynamicObj->AddPrimitive(messageId, method);
    }
}
