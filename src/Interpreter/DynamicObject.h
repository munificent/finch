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
        DynamicObject() {}
        
        DynamicObject(Ref<Object> prototype)
        :   mPrototype(prototype)
        {}
        
        virtual void Trace(std::ostream & stream) const;
        
        virtual Ref<Object> Receive(Ref<Object> thisRef, String message, vector<Ref<Object> > args);
        
    private:
        Ref<Object>               mPrototype;
        map<String, Ref<Object> > mFields;
    };    
}