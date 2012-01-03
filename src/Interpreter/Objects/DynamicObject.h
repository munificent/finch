#pragma once

#include <iostream>

#include "Dictionary.h"
#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "Scope.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    class Interpreter;

    // Object class for a "normal" full-featured object. Supports user-defined
    // fields and methods as well as primitive methods.
    class DynamicObject : public Object
    {
    public:
        DynamicObject(Ref<Object> parent, String name)
        :   Object(parent),
            mName(name)
        {
        }
        
        DynamicObject(Ref<Object> parent)
        :   Object(parent),
            mName("object")
        {
        }
        
        virtual void Trace(ostream & stream) const;
        
        virtual String AsString() const     { return mName; }
        virtual DynamicObject * AsDynamic() { return this; }
        /*
        virtual void Receive(Ref<Object> self, Fiber & fiber,
                             String message, const Array<Ref<Object> > & args);
        
        void AddMethod(Ref<Object> self, Fiber & fiber, String name,
                       Ref<Scope> closure, Ref<CodeBlock> code);
        
        void AddPrimitive(String message, PrimitiveMethod method);
         */
    private:
        void InitializeScope();
        
        String                              mName; //### bob: hack temp
        Dictionary<String, Ref<Object> >    mMethods;
        /*
        Dictionary<String, PrimitiveMethod> mPrimitives;
         */
    };    
}

