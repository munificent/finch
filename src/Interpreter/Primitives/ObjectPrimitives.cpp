#include <iostream>

#include "BlockObject.h"
#include "ObjectPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Interpreter.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(ObjectToString)
    {
        interpreter.PushString(thisRef->AsString());
    }
    
    PRIMITIVE(ObjectEquals)
    {
        // by default, objects compare using reference equality
        interpreter.PushBool(thisRef == args[0]);
    }
    
    PRIMITIVE(ObjectNotEquals)
    {
        // by default, objects compare using reference equality
        interpreter.PushBool(thisRef != args[0]);
    }
    
    PRIMITIVE(ObjectCopy)
    {
        interpreter.Push(Object::NewObject(thisRef));
    }
    
    PRIMITIVE(ObjectCopyWith)
    {
        // create the object
        Ref<Object> copy = Object::NewObject(thisRef);
        
        // run the initialization block
        BlockObject * originalBlock = args[0]->AsBlock();
        if (originalBlock == NULL)
        {
            interpreter.RuntimeError("copyWith: must be passed a block argument.");
            interpreter.PushNil();
        }
        else
        {
            // push the new object. this is what copyWith: will ultimately
            // return
            interpreter.Push(copy);
            
            // tell the interpreter to discard the return value of the next
            // called method. this will ditch the value returned by the
            // copyWith: block
            interpreter.DiscardReturn();
            
            // clone the block and rebind it's self to be the copied object.
            // this way, within the initialization block, it seems we're a
            // method on the block
            Ref<Object> blockCopy = Object::NewBlock(interpreter.GetEnvironment(),
                                                     originalBlock->GetCode(),
                                                     originalBlock->Closure(),
                                                     originalBlock->Self());
            BlockObject * block = blockCopy->AsBlock();
            block->RebindSelf(copy);
            
            //### bob: note, we have to clone the block here because, in theory,
            // you could reuse the block after passing it to copyWith:, like:
            //
            // b <- { writeLine: "self is " + self }
            // c <- Object copyWith: b
            // b call
            // d <- Object copyWith: b
            // b call
            // since binding self mutates it, we have to copy it first
            
            interpreter.CallBlock(blockCopy, Array<Ref<Object> >());
        }
    }
    
    PRIMITIVE(ObjectAddMethodBody)
    {
        DynamicObject * object = thisRef->AsDynamic();
        ASSERT_NOT_NULL(object);
        
        String      name  = args[0]->AsString();
        Ref<Object> value = args[1];
        
        object->AddMethod(thisRef, interpreter, name, value);
        interpreter.PushNil();
    }
    
    PRIMITIVE(ObjectGetParent)
    {
        interpreter.Push(thisRef->GetParent());
    }
    
    PRIMITIVE(ObjectSetParent)
    {
        thisRef->SetParent(args[0]);
        interpreter.PushNil();
    }
}

