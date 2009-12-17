#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::map;
    using std::vector;
    
    typedef Ref<Object> (*PrimitiveMethod)(Ref<Object> thisRef, Environment & env,
                                           String message, const vector<Ref<Object> > & args);

    class DynamicObject : public Object
    {
    public:
        DynamicObject(Ref<Object> prototype, String name)
        :   Object(prototype),
            mName(name)
        {}
        
        DynamicObject(Ref<Object> prototype)
        :   Object(prototype),
            mName("object")
        {}
        
        virtual void Trace(std::ostream & stream) const;
        
        virtual String AsString() const     { return mName; }
        virtual DynamicObject * AsDynamic() { return this; }

        virtual Ref<Object> Receive(Ref<Object> thisRef, Environment & env,
                                    String message, const vector<Ref<Object> > & args);
        
        Ref<Object> AddField(String name, Ref<Object> value);
        Ref<Object> AddMethod(String name, Ref<Object> body);
        
        void RegisterPrimitive(String message, PrimitiveMethod method);
        
    private:
        String                    mName; //### bob: hack temp
        map<String, Ref<Object> > mFields;
        map<String, Ref<Object> > mMethods;
        map<String, PrimitiveMethod > mPrimitives;
    };    
}