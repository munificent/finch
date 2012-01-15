#pragma once

#include <iostream>

#include "Dictionary.h"
#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
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
        DynamicObject(const Value & parent, String name)
        :   Object(parent),
            mName(name)
        {
        }
        
        DynamicObject(const Value & parent)
        :   Object(parent),
            mName("object")
        {
        }
        
        virtual void Trace(ostream & stream) const;
        
        virtual String AsString() const     { return mName; }
        virtual DynamicObject * AsDynamic() { return this; }
        
        virtual Ref<Object>     FindMethod(StringId messageId);
        virtual PrimitiveMethod FindPrimitive(StringId messageId);

        virtual Ref<Object>     GetField(StringId name);
        virtual void            SetField(StringId name, Ref<Object> value);

        void AddMethod(StringId messageId, Ref<Object> method);
        void AddPrimitive(StringId messageId, PrimitiveMethod method);
        
    private:
        void InitializeScope();
        
        String                      mName; //### bob: hack temp
        IdTable<Ref<Object> >       mFields;
        IdTable<Ref<Object> >       mMethods;
        IdTable<PrimitiveMethod>    mPrimitives;
    };    
}

