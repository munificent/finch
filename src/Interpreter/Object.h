#pragma once

#include <iostream>
#include <vector>

#include "Macros.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::vector;
    
    class Expr;
    class Scope;
    class BlockObject;
    class EvalContext;
    
    class Object
    {
    public:
        // virtual constructors
        static Ref<Object> New(Ref<Object> prototype, String name);
        static Ref<Object> New(Ref<Object> prototype);
        static Ref<Object> New(double value);
        static Ref<Object> New(String value);
        static Ref<Object> New(Ref<Expr> value);
        
        virtual ~Object() {}
        
        virtual Ref<Object> Receive(Ref<Object> thisRef, EvalContext & context,
                                    String message, vector<Ref<Object> > args) = 0;
        
        virtual double        AsNumber() const { return 0; }
        virtual String        AsString() const { return ""; }
        virtual BlockObject * AsBlock()        { return NULL; }
        
        virtual void Trace(std::ostream & stream) const = 0;
    };
    
    std::ostream& operator<<(std::ostream& cout, const Object & object);
}