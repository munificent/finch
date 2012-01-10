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
        
        virtual Ref<Object>     FindMethod(int messageId);
        virtual PrimitiveMethod FindPrimitive(int messageId);

        virtual Ref<Object>     GetField(int name);
        virtual void            SetField(int name, Ref<Object> value);

        void AddMethod(int messageId, Ref<Object> method);
        void AddPrimitive(int messageId, PrimitiveMethod method);
        
    private:
        void InitializeScope();
        
        String                      mName; //### bob: hack temp
        IdTable<Ref<Object> >       mFields;
        IdTable<Ref<Object> >       mMethods;
        IdTable<PrimitiveMethod>    mPrimitives;
    };    
}

