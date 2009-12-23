#include <iostream>
#include <string>

#include "Evaluator.h"
#include "Environment.h"
#include "FileLineReader.h"
#include "FinchParser.h"
#include "LineNormalizer.h"
#include "Object.h"
#include "Scope.h"
#include "Script.h"
#include "String.h"

namespace Finch
{
    Ref<Object> Script::Run(String fileName)
    {
        Environment env;

        return Run(fileName, env);
    }

    Ref<Object> Script::Run(String fileName, Environment & env)
    {
        FileLineReader reader(fileName);
        
        Ref<Object> result;
        
        if (reader.EndOfLines())
        {
            // couldn't open
            std::cout << "Couldn't open file \"" << fileName << "\"" << std::endl;
            //### bob: what should this error code be?
            return result;
        }
        
        Lexer           lexer(&reader);
        LineNormalizer  normalizer(&lexer);
        FinchParser     parser(&normalizer);
        
        Evaluator       evaluator(env);
        
        Ref<Expr> expr = parser.ParseFile();
        
        //### bob: need to report parse error
        if (!expr.IsNull())
        {
            result = evaluator.Evaluate(expr);
        }
        
        return result;
    }
}