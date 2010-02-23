#include <fstream>
#include <iostream>

#include "BlockObject.h"
#include "Environment.h"
#include "EtherPrimitives.h"
#include "Expr.h"
#include "Interpreter.h"
#include "Script.h"

namespace Finch
{
    using std::cerr;
    using std::cout;
    using std::endl;
    using std::ifstream;
    using std::ios;
    
    // Calls the given object if it's a block, otherwise just returns it.
    void CondtionallyEvaluate(Interpreter & interpreter, Ref<Object> object)
    {
        // evaluate the arg if it's a block, otherwise just return it
        if (object->AsBlock() == NULL)
        {
            interpreter.Push(object);
        }
        else
        {
            vector<Ref<Object> > noArgs;
            object->Receive(object, interpreter, "call", noArgs);
        }
    }
    
    PRIMITIVE(EtherQuit)
    {
        interpreter.StopRunning();
        interpreter.PushNil();
    }
    
    PRIMITIVE(EtherDo)
    {
        CondtionallyEvaluate(interpreter, args[0]);
    }
    
    PRIMITIVE(EtherIfThen)
    {
        vector<Ref<Object> > noArgs;
        
        if (args[0] == interpreter.GetEnvironment().True())
        {
            CondtionallyEvaluate(interpreter, args[1]);
        }
        else
        {
            interpreter.PushNil();
        }
    }
    
    PRIMITIVE(EtherIfThenElse)
    {
        vector<Ref<Object> > noArgs;
        
        if (args[0] == interpreter.GetEnvironment().True())
        {
            CondtionallyEvaluate(interpreter, args[1]);
        }
        else
        {
            CondtionallyEvaluate(interpreter, args[2]);
        }
    }
    
    //### bob: need to figure out how this is going to work...
    /*
    PRIMITIVE(EtherWhileDo)
    {
        BlockObject * condition = args[0]->AsBlock();
        
        if (condition == NULL)
        {
            interpreter.GetEnvironment().RuntimeError("First argument to while:do: must be a block.");
            return Ref<Object>();
        }
        
        vector<Ref<Object> > noArgs;
        
        while (interpreter.EvaluateBlock(*condition, noArgs) ==
               interpreter.GetEnvironment().True())
        {
            args[1]->Receive(args[1], interpreter, "call", noArgs);
        }
        
        return Ref<Object>();
    }
    */
    
    PRIMITIVE(EtherWrite)
    {
        String text = args[0]->AsString();
        cout << text;
        
        interpreter.PushNil();
    }
    
    PRIMITIVE(EtherWriteLine)
    {
        String text = args[0]->AsString();
        cout << text << endl;
        
        interpreter.PushNil();
    }
    
    PRIMITIVE(EtherLoad)
    {
        String fileName = args[0]->AsString();
        Script::Run(fileName, interpreter);
    }
}
