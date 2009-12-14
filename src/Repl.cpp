#include <iostream>
#include <string>

#include "Evaluator.h"
#include "EvalContext.h"
#include "Scope.h"
#include "String.h"
#include "Repl.h"

namespace Finch
{
    void Repl::Run()
    {
        bool   running = true;
        String line;
        
        EvalContext context;
        Evaluator evaluator(context);
        
        std::cout << "finch 0.0.0d\n";
        
        while (running)
        {
            std::cout << "> ";
            getline(std::cin, line);
            
            if (line == "quit")
            {
                running = false;
            }
            else
            {
                Ref<Expr> expr = mParser.ParseLine(line.c_str());
                if (!expr.IsNull())
                {
                    //std::cout << "parsed \"" << *expr << "\"" << std::endl;
                    
                    Ref<Object> result = evaluator.Evaluate(expr);
                    
                    if (!result.IsNull())
                    {
                        std::cout << "< " << *result << std::endl;
                    }
                    else
                    {
                        std::cout << "< no result" << std::endl;
                    }
                }
                else
                {
                    std::cout << "Parse error" << std::endl;
                }
            }
        }
    }
}