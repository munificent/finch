#include <iostream>
#include <string>

#include "Compiler.h"
#include "Environment.h"
#include "FinchParser.h"
#include "LineNormalizer.h"
#include "Process.h"
#include "Repl.h"
#include "ReplLineReader.h"
#include "Scope.h"
#include "Script.h"
#include "FinchString.h"

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
        
        Process        process(env);
        
        cout << "Finch 0.0.0d" << endl;
        cout << "------------" << endl;
        
        //### bob: hard-coded path here is a total hack
        // load the base library
        #ifdef HACK_ROOT_BASE_PATH
            const char* baseLibPath = "base/main.fin";
        #else
            const char* baseLibPath = "../../base/main.fin";
        #endif
        Script::Execute(baseLibPath, process);
        
        while (process.IsRunning())
        {
            // ansi color: std::cout << "\033[0;32m";
            reader.Reset();
            Ref<Expr> expr = parser.ParseLine();
            
            if (expr.IsNull())
            {
                lexer.Reset();
                parser.Reset();
                continue;
            }
            
            //cout << "parsed \"" << *expr << "\"" << endl;
            
            // create a block for the expression
            int id = env.Blocks().Add(Array<String>(), *expr, env);
            const CodeBlock & code = env.Blocks().Find(id);
            Ref<Object> block = Object::NewBlock(env, code, env.Globals(), env.Nil());
            
            // and execute it
            Ref<Object> result = process.Execute(block);

            // don't bother printing nil results
            if (result != env.Nil())
            {
                cout << *result << endl;
            }
        }
    }
}

