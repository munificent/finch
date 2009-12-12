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
        virtual void Trace(std::ostream & stream) const;
        
        virtual Ref<Object> Receive(String message, vector<Ref<Object> > args);
        
    private:
        map<String, Ref<Object> > mFields;
    };    
}