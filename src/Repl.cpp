#include <iostream>
#include <string>

#include "Compiler.h"
#include "Evaluator.h"
#include "Environment.h"
#include "FinchParser.h"
#include "Interpreter.h"
#include "LineNormalizer.h"
#include "Repl.h"
#include "ReplLineReader.h"
#include "Scope.h"
#include "String.h"

namespace Finch
{
    using std::cin;
    using std::cout;
    using std::endl;
    
    void Repl::Run()
    {
        Environment env;
        
        ReplLineReader reader;
        Lexer          lexer(reader);
        LineNormalizer normalizer(lexer);
        FinchParser    parser(normalizer);
        
        Interpreter    interpreter(env);

        cout << "Finch 0.0.0d" << endl;
        cout << "------------" << endl;
        
        while (interpreter.Running())
        {
            // ansi color: std::cout << "\033[0;32m";
            reader.Reset();
            Ref<Expr> expr = parser.ParseLine();
            if (!expr.IsNull())
            {
                //cout << "parsed \"" << *expr << "\"" << endl;
                
                int id = env.Blocks().Add(vector<String>(), *expr, env);
                const CodeBlock & code = env.Blocks().Find(id);
                
                Ref<Object> result = interpreter.Execute(code);

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