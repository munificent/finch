#include <iostream>
#include <string>

#include "Evaluator.h"
#include "Environment.h"
#include "Scope.h"
#include "String.h"
#include "Repl.h"
#include "LineNormalizer.h"
#include "FinchParser.h"
#include "ReplLineReader.h"

namespace Finch
{
    using std::cin;
    using std::cout;
    using std::endl;
    
    void Repl::Run()
    {
        Environment env;
        Evaluator   evaluator(env);
        
        ReplLineReader reader;
        Lexer          lexer(&reader);
        LineNormalizer normalizer(&lexer);
        FinchParser    parser(&normalizer);
                
        cout << "Finch 0.0.0d" << endl;
        cout << "------------" << endl;
        
        while (env.Running())
        {
            // ansi color: std::cout << "\033[0;32m";
            reader.Reset();
            Ref<Expr> expr = parser.ParseLine();
            if (!expr.IsNull())
            {
                //cout << "parsed \"" << *expr << "\"" << endl;
                
                Ref<Object> result = evaluator.Evaluate(expr);
                
                if (!result.IsNull())
                {
                    // don't bother printing nil results
                    if (result != env.Nil())
                    {
                        cout << *result << endl;
                    }
                }
                else
                {
                    cout << "no result" << endl;
                }
            }
            else
            {
                cout << "parse error" << endl;
            }
        }
    }
}