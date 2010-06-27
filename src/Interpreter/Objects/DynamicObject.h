#pragma once

#include <iostream>

#include "Dictionary.h"
#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "Scope.h"
#include "String.h"

namespace Finch
{
    using std::ostream;
    
    class Interpreter;
    
    // Function pointer type for a primitive Finch method implemented in C++.
    typedef void (*PrimitiveMethod)(Ref<Object> thisRef, Interpreter & interpreter,
                                    String message, const Array<Ref<Object> > & args);

    // Object class for a "normal" full-featured object. Supports user-defined
    // fields and methods as well as primitive methods.
    class DynamicObject : public Object
    {
    public:
        DynamicObject(Ref<Object> parent, String name)
        :   Object(parent),
            mName(name)
        {
            InitializeScope();
        }
        
        DynamicObject(Ref<Object> parent)
        :   Object(parent),
            mName("object")
        {
            InitializeScope();
        }
        
        virtual void Trace(ostream & stream) const;
        
        virtual String AsString() const     { return mName; }
        virtual DynamicObject * AsDynamic() { return this; }
        
        virtual Ref<Scope> ObjectScope() const { return mScope; }
        
        virtual void Receive(Ref<Object> thisRef, Interpreter & interpreter,
                             String message, const Array<Ref<Object> > & args);
        
        void AddMethod(Ref<Object> thisRef, Interpreter & interpreter,
                       String name, Ref<Object> body);
        
        void RegisterPrimitive(String message, PrimitiveMethod method);
        
    private:
        void InitializeScope();
        
        String                              mName; //### bob: hack temp
        Dictionary<String, Ref<Object> >    mMethods;
        Dictionary<String, PrimitiveMethod> mPrimitives;
        
        Ref<Scope> mScope;
    };    
}

