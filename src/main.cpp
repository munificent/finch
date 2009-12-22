#include <iostream>

#include "Ref.h"
#include "Repl.h"
#include "Script.h"

using namespace Finch;

int main (int argc, char * const argv[])
{
    if (argc == 1)
    {
        // with no arguments (arg zero is app), run in interactive mode
        Repl repl;    
        repl.Run();
    }
    else if (argc == 2)
    {
        // one argument, load and execute the given script
        String fileName = argv[1];
        
        Script::Run(fileName);
        /*
        FileLineReader reader(fileName);
        
        if (reader.EndOfLines())
        {
            // couldn't open
            std::cout << "Couldn't open file \"" << fileName << "\"" << std::endl;
            //### bob: what should this error code be?
            return 0;
        }
        
        Lexer           lexer(&reader);
        LineNormalizer  normalizer(&lexer);
        FinchParser     parser(&normalizer);

        Environment     env;
        Evaluator       evaluator(env);
        
        Ref<Object> result;
        Ref<Expr> expr = parser.ParseFile();
        
        //### bob: need to report parse error
        if (!expr.IsNull())
        {
            evaluator.Evaluate(expr);
        }
         */
    }
    
    return 0;
}
