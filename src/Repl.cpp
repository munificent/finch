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
        Evaluator   evaluator(env);
        
        ReplLineReader reader;
        Lexer          lexer(reader);
        LineNormalizer normalizer(lexer);
        FinchParser    parser(normalizer);
        
        Interpreter    interpreter;

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
                
                //### bob: run the old interpreter
                Ref<Object> result = evaluator.Evaluate(expr);
                
                if (!result.IsNull())
                {
                    // don't bother printing nil results
                    if (result != env.Nil())
                    {
                        cout << "old " << *result << endl;
                    }
                }
                else
                {
                    cout << "old no result" << endl;
                }

                //### bob: new hotness!
                Ref<CodeBlock> compiled = Compiler::Compile(env, *expr);
                result = interpreter.Execute(env, *compiled);

                if (!result.IsNull())
                {
                    // don't bother printing nil results
                    if (result != env.Nil())
                    {
                        cout << "new " << *result << endl;
                    }
                }
                else
                {
                    cout << "new no result" << endl;
                }
            }
            else
            {
                cout << "parse error" << endl;
            }
        }
    }
}