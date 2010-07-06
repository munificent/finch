#include "FinchParser.h"
#include "Interpreter.h"
#include "Lexer.h"
#include "LineNormalizer.h"
#include "Process.h"

namespace Finch
{
    using std::cout;
    using std::endl;
    
    void Interpreter::Execute(ILineReader & reader, bool parseFile, bool callBlock)
    {
        // spin up a process
        Process process(*this, mEnvironment);
        
        Execute(process, reader, parseFile, callBlock);
    }
    
    void Interpreter::Execute(Process & process, ILineReader & reader,
                              bool parseFile, bool callBlock)
    {
        Lexer          lexer(reader);
        LineNormalizer normalizer(lexer);
        FinchParser    parser(normalizer);
        
        // ansi color: std::cout << "\033[0;32m";
        Ref<Expr> expr = parseFile ? parser.ParseFile() : parser.ParseLine();
        
        // bail if we failed to parse
        //### bob: need better error handling here
        if (expr.IsNull())
        {
            cout << "parse error" << endl;
            return;
        }
        
        // create a block for the expression
        int id = mEnvironment.Blocks().Add(Array<String>(), *expr, mEnvironment);
        const CodeBlock & code = mEnvironment.Blocks().Find(id);
        Ref<Object> block = Object::NewBlock(mEnvironment, code,
                                             mEnvironment.Globals(),
                                             mEnvironment.Nil());
        
        // and execute the code
        if (callBlock)
        {
            Array<Ref<Object> > noArgs;
            process.CallBlock(block, noArgs);
        }
        else
        {
            Ref<Object> result = process.Execute(block);
            
            // don't bother printing nil results
            if (result != mEnvironment.Nil())
            {
                //### bob: should go through host to print
                cout << *result << endl;
            }
        }
    }
}

