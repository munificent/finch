#pragma once

#include <iostream>

#include "Environment.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    // Object class for a dynamically-resizable array.
    class ArrayObject : public Object
    {
    public:
        ArrayObject(Environment & environment, Ref<Object> parent, int length)
        :   Object(parent),
            mElements(length, environment.Nil())
        {
        }
        
        Array<Ref<Object> > & Elements() { return mElements; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << AsString();
        }
        
        virtual ArrayObject * AsArray() { return this; }
        
        virtual String AsString() const
        {
            String text = "#[";
            
            if (mElements.Count() > 0) text += mElements[0]->AsString();
            for (int i = 1; i < mElements.Count(); i++)
            {
                text += String(", ") + mElements[i]->AsString();
            }
            text += "]";
            
            return text;
        }
        
    private:
        Array<Ref<Object> > mElements;
    };
}

