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
                                    String message, vector<Ref<Object> > args);
        
    private:
        Ref<Object>               mPrototype;
        String                    mName; //### bob: hack temp
        map<String, Ref<Object> > mFields;
        map<String, Ref<Object> > mMethods;
    };    
}