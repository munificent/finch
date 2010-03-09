#pragma once

#include <iostream>

#include "Array.h"
#include "Macros.h"
#include "Ref.h"
#include "String.h"

#define PRIMITIVE(name)                                             \
        void name(Ref<Object> thisRef, Interpreter & interpreter,   \
             String message, const Array<Ref<Object> > & args)

namespace Finch
{
    using std::ostream;
    
    class Expr;
    class Scope;
    class ArrayObject;
    class BlockObject;
    class CodeBlock;
    class DynamicObject;
    class Environment;
    class Interpreter;
    
    // Base class for an object in Finch. All values in Finch inherit from this.
    class Object
    {
    public:
        // virtual constructors
        static Ref<Object> NewObject(Ref<Object> prototype, String name);
        static Ref<Object> NewObject(Ref<Object> prototype);
        static Ref<Object> NewNumber(Environment & env, double value);
        static Ref<Object> NewString(Environment & env, String value);
        static Ref<Object> NewArray(Environment & env, int length);
        static Ref<Object> NewBlock(Environment & env, const CodeBlock & code, 
                                    Ref<Scope> closure, Ref<Object> self);
        
        virtual ~Object() {}
        
        virtual void Receive(Ref<Object> thisRef, Interpreter & interpreter,
                             String message, const Array<Ref<Object> > & args);
        
        virtual double          AsNumber() const { return 0; }
        virtual String          AsString() const { return ""; }
        virtual ArrayObject *   AsArray()        { return NULL; }
        virtual BlockObject *   AsBlock()        { return NULL; }
        virtual DynamicObject * AsDynamic()      { return NULL; }
        
        virtual Ref<Scope> ObjectScope() const;
        
        Ref<Object> GetPrototype() { return mPrototype; }
        void        SetPrototype(Ref<Object> prototype) { mPrototype = prototype; }
        
        virtual void Trace(ostream & stream) const = 0;
        
    protected:
        Object() : mPrototype(Ref<Object>()) {}
        Object(Ref<Object> prototype) : mPrototype(prototype) {}
        
        Ref<Object> Prototype() const { return mPrototype; }
        
    private:
        Ref<Object> mPrototype;
    };
    
    ostream & operator<<(ostream & cout, const Object & object);
}