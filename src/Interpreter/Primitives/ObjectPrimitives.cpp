#include <iostream>

#include "BlockObject.h"
#include "ObjectPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Process.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(ObjectGetPrototype)
    {
        process.Push(process.GetEnvironment().ObjectPrototype());
    }
    
    PRIMITIVE(ObjectToString)
    {
        process.PushString(thisRef->AsString());
    }
    
    PRIMITIVE(ObjectEquals)
    {
        // by default, objects compare using reference equality
        process.PushBool(thisRef == args[0]);
    }
    
    PRIMITIVE(ObjectNotEquals)
    {
        // by default, objects compare using reference equality
        process.PushBool(thisRef != args[0]);
    }
    
    PRIMITIVE(ObjectCopy)
    {
        process.Push(Object::NewObject(thisRef));
    }
    
    PRIMITIVE(ObjectCopyWith)
    {
        // create the object
        Ref<Object> copy = Object::NewObject(thisRef);
        
        // run the initialization block
        BlockObject * originalBlock = args[0]->AsBlock();
        if (originalBlock == NULL)
        {
            process.Error("copyWith: must be passed a block argument.");
            process.PushNil();
        }
        else
        {
            // push the new object. this is what copyWith: will ultimately
            // return
            process.Push(copy);
            
            // tell the process to discard the return value of the next called
            // method. this will ditch the value returned by the copyWith: block
            process.DiscardReturn();
            
            // clone the block and rebind it's self to be the copied object.
            // this way, within the initialization block, it seems we're a
            // method on the block
            Ref<Object> blockCopy = Object::NewBlock(process.GetEnvironment(),
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
            
            process.CallBlock(blockCopy, Array<Ref<Object> >());
        }
    }
    
    PRIMITIVE(ObjectAddMethodBody)
    {
        DynamicObject * object = thisRef->AsDynamic();
        ASSERT_NOT_NULL(object);
        
        String      name  = args[0]->AsString();
        Ref<Object> value = args[1];
        
        object->AddMethod(thisRef, process, name, value);
        process.PushNil();
    }
    
    PRIMITIVE(ObjectGetParent)
    {
        process.Push(thisRef->GetParent());
    }
    
    PRIMITIVE(ObjectSetParent)
    {
        thisRef->SetParent(args[0]);
        process.PushNil();
    }
}

