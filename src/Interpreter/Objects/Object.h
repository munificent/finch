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
        static Ref<Object> New(Ref<Object> prototype, double value);
        static Ref<Object> NewString(Ref<Object> prototype, String value);
        static Ref<Object> New(Ref<Object> prototype, Ref<Expr> value);
        
        virtual ~Object() {}
        
        //### bob: pass args by const&
        virtual Ref<Object> Receive(Ref<Object> thisRef, EvalContext & context,
                                    String message, const vector<Ref<Object> > & args);
        
        virtual double        AsNumber() const { return 0; }
        virtual String        AsString() const { return ""; }
        virtual BlockObject * AsBlock()        { return NULL; }
        
        virtual void Trace(std::ostream & stream) const = 0;
        
    protected:
        Object() : mPrototype(Ref<Object>()) {}
        Object(Ref<Object> prototype) : mPrototype(prototype) {}
        
        Ref<Object> Prototype() const { return mPrototype; }
        
    private:
        Ref<Object> mPrototype;
    };
    
    std::ostream& operator<<(std::ostream& cout, const Object & object);
}