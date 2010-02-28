#include <iostream>
#include <string>

#include "Compiler.h"
#include "Environment.h"
#include "FinchParser.h"
#include "Interpreter.h"
#include "LineNormalizer.h"
#include "Repl.h"
#include "ReplLineReader.h"
#include "Scope.h"
#include "Script.h"
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
        
        //### bob: hard-coded path here is a total hack
        // load the base library
        Script::Execute("../../base/main.fin", interpreter);
        
        while (interpreter.IsRunning())
        {
            // ansi color: std::cout << "\033[0;32m";
            reader.Reset();
            Ref<Expr> expr = parser.ParseLine();
            
            if (expr.IsNull())
            {
                cout << "parse error" << endl;
                continue;
            }
            
            //cout << "parsed \"" << *expr << "\"" << endl;
            
            // create a block for the expression
            int id = env.Blocks().Add(Array<String>(), *expr, env);
            const CodeBlock & code = env.Blocks().Find(id);
            
            // and execute it
            Ref<Object> result = interpreter.Execute(code);

            // don't bother printing nil results
            if (result != env.Nil())
            {
                cout << *result << endl;
            }
        }
    }
}