#include <fstream>
#include <iostream>

#include "BlockObject.h"
#include "Environment.h"
#include "EtherPrimitives.h"
#include "Expr.h"
#include "Script.h"

namespace Finch
{
    using std::cerr;
    using std::cout;
    using std::endl;
    using std::ifstream;
    using std::ios;
    
    Ref<Object> EtherQuit(Ref<Object> thisRef, Environment & env,
                                String message, const vector<Ref<Object> > & args)
    {
        env.StopRunning();
        return Ref<Object>();
    }
    
    Ref<Object> EtherIfThen(Ref<Object> thisRef, Environment & env,
                          String message, const vector<Ref<Object> > & args)
    {
        vector<Ref<Object> > noArgs;
        
        if (args[0] == env.True())
        {
            return args[1]->Receive(args[1], env, "call", noArgs);
        }
        
        return Ref<Object>();
    }
    
    Ref<Object> EtherIfThenElse(Ref<Object> thisRef, Environment & env,
                              String message, const vector<Ref<Object> > & args)
    {
        vector<Ref<Object> > noArgs;
        
        if (args[0] == env.True())
        {
            return args[1]->Receive(args[1], env, "call", noArgs);
        }
        else
        {
            return args[2]->Receive(args[2], env, "call", noArgs);
        }
    }
    
    Ref<Object> EtherWhileDo(Ref<Object> thisRef, Environment & env,
                                   String message, const vector<Ref<Object> > & args)
    {
        BlockObject * condition = args[0]->AsBlock();
        
        // do nothing if the condition isn't a block
        //### bob: report error here?
        if (condition == NULL) return env.Nil();
        
        vector<Ref<Object> > noArgs;
        
        while (env.EvaluateBlock(condition, noArgs) == env.True())
        {
            args[1]->Receive(args[1], env, "value", noArgs);
        }
        
        return Ref<Object>();
    }
    
    Ref<Object> EtherWrite(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args)
    {
        String text = args[0]->AsString();
        cout << text;
        
        return Ref<Object>();
    }
    
    Ref<Object> EtherWriteLine(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args)
    {
        String text = args[0]->AsString();
        cout << text << endl;
        
        return Ref<Object>();
    }
    
    Ref<Object> EtherLoad(Ref<Object> thisRef, Environment & env,
                                 String message, const vector<Ref<Object> > & args)
    {
        String fileName = args[0]->AsString();
        
        return Script::Run(fileName, env);
        
        /*
        FileLineReader reader(fileName);
        
        if (reader.EndOfLines())
        {
            // couldn't open
            cout << "Couldn't open file \"" << fileName << "\"" << endl;
            //### bob: need error reporting
            return Ref<Object>();
        }
        
        Lexer           lexer(&reader);
        LineNormalizer  normalizer(&lexer);
        FinchParser     parser(&normalizer);
        Evaluator       evaluator(env);

        Ref<Object> result;
        Ref<Expr> expr = parser.ParseFile();

        if (!expr.IsNull())
        {
            result = evaluator.Evaluate(expr);
        }
        return result;
         */
    }
}
