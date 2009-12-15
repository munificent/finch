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
    
    typedef Ref<Object> (*PrimitiveMethod)(Ref<Object> thisRef, EvalContext & context,
                                           String message, const vector<Ref<Object> > & args);

    class DynamicObject : public Object
    {
    public:
        DynamicObject(Ref<Object> prototype, String name)
        :   mPrototype(prototype),
            mName(name)
        {}
        
        DynamicObject(Ref<Object> prototype)
        :   mPrototype(prototype),
            mName("object")
        {}
        
        virtual void Trace(std::ostream & stream) const;
        
        virtual String AsString() const { return mName; }

        virtual Ref<Object> Receive(Ref<Object> thisRef, EvalContext & context,
                                    String message, const vector<Ref<Object> > & args);
        
        void RegisterPrimitive(String message, PrimitiveMethod method);
        
    private:
        Ref<Object>               mPrototype;
        String                    mName; //### bob: hack temp
        map<String, Ref<Object> > mFields;
        map<String, Ref<Object> > mMethods;
        map<String, PrimitiveMethod > mPrimitives;
    };    
}