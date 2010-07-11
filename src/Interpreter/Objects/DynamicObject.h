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
        
        virtual void Receive(Ref<Object> self, Process & process,
                             String message, const Array<Ref<Object> > & args);
        
        void AddMethod(Ref<Object> self, Process & process,
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

