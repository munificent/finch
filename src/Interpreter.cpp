#include "FileLineReader.h"
#include "FinchParser.h"
#include "Interpreter.h"
#include "Lexer.h"
#include "LineNormalizer.h"
#include "Process.h"

namespace Finch
{
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
        //### bob: need better error handling here
        if (expr.IsNull())
        {
            std::cout << "parse error" << std::endl;
            return;
        }
        
        // create a block for the expression
        Ref<Object> block = mEnvironment.CreateBlock(expr);
        
        // and execute the code
        Array<Ref<Object> > noArgs;
        process.CallBlock(block, noArgs);
    }
        
    Ref<Expr> Interpreter::Parse(ILineReader & reader)
    {
        Lexer          lexer(reader);
        LineNormalizer normalizer(lexer);
        FinchParser    parser(normalizer);
        
        return parser.Parse();
    }
    
    bool Interpreter::Execute(Ref<Expr> expr)
    {        
        // bail if we failed to parse
        //### bob: need better error handling here
        if (expr.IsNull())
        {
            std::cout << "parse error" << std::endl;
            return false;
        }
        
        // create a block for the expression
        Ref<Object> block = mEnvironment.CreateBlock(expr);
        
        Process process(*this, mEnvironment);
        Ref<Object> result = process.Execute(block);
        
        // don't bother printing nil results
        if (result != mEnvironment.Nil())
        {
            //### bob: should go through host to print
            std::cout << *result << std::endl;
        }
        
        return true;
    }
}