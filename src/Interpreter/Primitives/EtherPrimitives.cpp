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
    
    PRIMITIVE(EtherWhileDo)
    {
        interpreter.WhileLoop(args[0], args[1]);
    }
    
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
