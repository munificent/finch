#pragma once

#include <iostream>
#include <map>

#include "Macros.h"
#include "String.h"
#include "Value.h"

namespace Finch
{
    using std::map;
    
    class Object
    {
    public:
        
    private:
        map<String, Value> mDataSlots;
    };
}