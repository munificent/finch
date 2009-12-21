#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "Dictionary.h"
#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::map;
    using std::vector;
    
    // Function pointer type for a primitive Finch method implemented in C++.
    typedef Ref<Object> (*PrimitiveMethod)(Ref<Object> thisRef, Environment & env,
                                           String message, const vector<Ref<Object> > & args);

    // Object class for a "normal" full-featured object. Supports user-defined
    // fields and methods as well as primitive methods.
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
        String                     mName; //### bob: hack temp
        Dictionary<String, Object> mFields;
        Dictionary<String, Object> mMethods;
        map<String, PrimitiveMethod > mPrimitives;
    };    
}