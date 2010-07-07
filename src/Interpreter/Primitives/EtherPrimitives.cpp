#include <fstream>
#include <iostream>

#include "BlockObject.h"
#include "Environment.h"
#include "EtherPrimitives.h"
#include "Expr.h"
#include "FileLineReader.h"
#include "IInterpreterHost.h"
#include "Interpreter.h"
#include "Process.h"

namespace Finch
{
    using std::cerr;
    using std::cout;
    using std::endl;
    using std::ifstream;
    using std::ios;
    
    // Calls the given object if it's a block, otherwise just returns it.
    void CondtionallyEvaluate(Process & process, Ref<Object> object)
    {
        if (object->AsBlock() == NULL)
        {
            process.Push(object);
        }
        else
        {
            Array<Ref<Object> > noArgs;
            object->Receive(object, process, "call", noArgs);
        }
    }
    
    PRIMITIVE(EtherQuit)
    {
        process.StopRunning();
        process.PushNil();
    }
    
    PRIMITIVE(EtherDo)
    {
        CondtionallyEvaluate(process, args[0]);
    }
    
    PRIMITIVE(EtherIfThen)
    {
        if (args[0] == process.GetEnvironment().True())
        {
            CondtionallyEvaluate(process, args[1]);
        }
        else
        {
            process.PushNil();
        }
    }
    
    PRIMITIVE(EtherIfThenElse)
    {
        if (args[0] == process.GetEnvironment().True())
        {
            CondtionallyEvaluate(process, args[1]);
        }
        else
        {
            CondtionallyEvaluate(process, args[2]);
        }
    }
    
    PRIMITIVE(EtherWhileDo)
    {
        process.WhileLoop(args[0], args[1]);
    }
    
    PRIMITIVE(EtherWrite)
    {
        String text = args[0]->AsString();
        process.GetInterpreter().GetHost().Output(text);
        
        process.PushNil();
    }
    
    PRIMITIVE(EtherWriteLine)
    {
        String text = args[0]->AsString();
        process.GetInterpreter().GetHost().Output(text);
        process.GetInterpreter().GetHost().Output("\n");
        
        process.PushNil();
    }
    
    PRIMITIVE(EtherLoad)
    {
        String fileName = args[0]->AsString();
        process.GetInterpreter().EtherLoad(process, fileName);
    }
}

