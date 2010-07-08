#include <sstream>

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

    bool Interpreter::InterpretFile(String fileName)
    {
        FileLineReader reader(fileName);
        
        if (reader.EndOfLines())
        {
            // couldn't open
            std::cout << "Couldn't open file \"" << fileName << "\"" << std::endl;
            return false;
        }
        
        return Execute(Parse(reader));
    }
    
    void Interpreter::InterpretSource(ILineReader & reader)
    {
        Execute(Parse(reader));
    }
    
    void Interpreter::EtherLoad(Process & process, String filePath)
    {
        FileLineReader reader(filePath);
        
        if (reader.EndOfLines())
        {
            // couldn't open
            std::cout << "Couldn't open file \"" << filePath << "\"" << std::endl;
            return;
        }
        
        Ref<Expr> expr = Parse(reader);
        
        // bail if we failed to parse
        if (expr.IsNull()) return;
        
        // create a block for the expression
        Ref<Object> block = mEnvironment.CreateBlock(expr);
        
        // and execute the code
        Array<Ref<Object> > noArgs;
        process.CallBlock(block, noArgs);
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
        
        Process process(*this, mEnvironment);
        Ref<Object> result = process.Execute(block);
        
        // don't bother printing nil results
        if (result != mEnvironment.Nil())
        {
            std::stringstream text;
            text << *result << std::endl;
            mHost.Output(String(text.str().c_str()));
        }
        
        return true;
    }
}