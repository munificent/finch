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
        
        virtual Value FindMethod(StringId messageId);
        virtual PrimitiveMethod FindPrimitive(StringId messageId);

        virtual Value GetField(StringId name);
        virtual void  SetField(StringId name, const Value & value);

        void AddMethod(StringId messageId, const Value & method);
        void AddPrimitive(StringId messageId, PrimitiveMethod method);
        
    private:
        void InitializeScope();
        
        String                      mName; //### bob: hack temp
        IdTable<Value>              mFields;
        IdTable<Value>              mMethods;
        IdTable<PrimitiveMethod>    mPrimitives;
    };    
}

