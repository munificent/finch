#include <iostream>
#include <string>

#include "Environment.h"
#include "FileLineReader.h"
#include "FinchParser.h"
#include "LineNormalizer.h"
#include "Object.h"
#include "Process.h"
#include "Scope.h"
#include "Script.h"
#include "FinchString.h"

namespace Finch
{
    void Script::Run(String fileName)
    {
        Environment environment;
        Process process(environment);
        Execute(fileName, process);
    }
    
    //### bob: these two functions are almost identical. merge:
    
    void Script::Execute(String fileName, Process & process)
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
            int id = process.GetEnvironment().Blocks().Add(Array<String>(), *expr, process.GetEnvironment());
            const CodeBlock & code = process.GetEnvironment().Blocks().Find(id);
            
            Ref<Object> block = Object::NewBlock(
                    process.GetEnvironment(),
                    code, process.GetEnvironment().Globals(), 
                    process.GetEnvironment().Nil());
            
            process.Execute(block);
        }
        else
        {
            //### bob: need better error-handling
            std::cout << "parse error loading " << fileName << std::endl;
        }
    }
    
    void Script::Run(String fileName, Process & process)
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
            int id = process.GetEnvironment().Blocks().Add(Array<String>(), *expr, process.GetEnvironment());
            const CodeBlock & code = process.GetEnvironment().Blocks().Find(id);
            
            Ref<Object> block = Object::NewBlock(process.GetEnvironment(), code,
                process.GetEnvironment().Globals(), process.GetEnvironment().Nil());
            
            Array<Ref<Object> > noArgs;
            process.CallBlock(block, noArgs);
        }
        else
        {
            //### bob: need better error-handling
            std::cout << "parse error loading " << fileName << std::endl;
        }
    }
}

