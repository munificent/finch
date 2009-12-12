#include <iostream>
#include <string>

#include "Scope.h"
#include "String.h"
#include "Repl.h"

namespace Finch
{
    void Repl::Run()
    {
        bool   running = true;
        String line;
        
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
                    
                    Ref<Object> result = mEvaluator.Evaluate(expr);
                    
                    if (!result.IsNull())
                    {
                        std::cout << "< " << *result << std::endl;
                    }
                    else
                    {
                        std::cout << "< ERROR NULL RESULT OBJECT" << std::endl;
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