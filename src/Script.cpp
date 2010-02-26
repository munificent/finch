#include <iostream>
#include <string>

#include "Environment.h"
#include "FileLineReader.h"
#include "FinchParser.h"
#include "Interpreter.h"
#include "LineNormalizer.h"
#include "Object.h"
#include "Scope.h"
#include "Script.h"
#include "String.h"

namespace Finch
{
    void Script::Run(String fileName)
    {
        Environment environment;
        Interpreter interpreter(environment);

        return Run(fileName, interpreter);
    }
    
    void Script::Run(String fileName, Interpreter & interpreter)
    {
        FileLineReader reader(fileName);
        
        if (reader.EndOfLines())
        {
            // couldn't open
            std::cout << "Couldn't open file \"" << fileName << "\"" << std::endl;
            return;
        }
        
        Lexer           lexer(reader);
        LineNormalizer  normalizer(lexer);
        FinchParser     parser(normalizer);
        
        Ref<Expr> expr = parser.ParseFile();
        
        //### bob: need to report parse error
        if (!expr.IsNull())
        {
            int id = interpreter.GetEnvironment().Blocks().Add(Array<String>(), *expr, interpreter.GetEnvironment());
            const CodeBlock & code = interpreter.GetEnvironment().Blocks().Find(id);
            Ref<Object> block = Object::NewBlock(interpreter.GetEnvironment(), code, interpreter.GetEnvironment().Globals());
            
            Array<Ref<Object> > noArgs;
            interpreter.CallBlock(*(block->AsBlock()), noArgs);
        }
        else
        {
            //### bob: need better error-handling
            std::cout << "parse error loading " << fileName << std::endl;
        }
    }
}