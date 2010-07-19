#pragma once

#include <iostream>

#include "Array.h"
#include "Macros.h"
#include "Ref.h"
#include "FinchString.h"
#include "Scope.h"

#define PRIMITIVE(name)                                             \
        void name(Ref<Object> self, Fiber & fiber,              \
             String message, const Array<Ref<Object> > & args)

namespace Finch
{
    using std::ostream;
    
    class Expr;
    class ArrayObject;
    class BlockObject;
    class CodeBlock;
    class DynamicObject;
    class Environment;
    class FiberObject;
    class Interpreter;
    class Object;
    class Fiber;
    
    // Function pointer type for a primitive Finch method implemented in C++.
    typedef void (*PrimitiveMethod)(Ref<Object> self, Fiber & fiber,
                                    String message, const Array<Ref<Object> > & args);
    
    // Base class for an object in Finch. All values in Finch inherit from this.
    class Object
    {
    public:
        // virtual constructors
        static Ref<Object> NewObject(Ref<Object> parent, String name);
        static Ref<Object> NewObject(Ref<Object> parent);
        static Ref<Object> NewNumber(Environment & env, double value);
        static Ref<Object> NewString(Environment & env, String value);
        static Ref<Object> NewArray(Environment & env, int length);
        static Ref<Object> NewBlock(Environment & env, const CodeBlock & code, 
                                    Ref<Scope> closure, Ref<Object> self);
        static Ref<Object> NewFiber(Interpreter & interpreter, Ref<Object> block);
        
        virtual ~Object() {}
        
        virtual void Receive(Ref<Object> self, Fiber & fiber,
                             String message, const Array<Ref<Object> > & args);
        
        virtual double          AsNumber() const { return 0; }
        virtual String          AsString() const { return ""; }
        virtual ArrayObject *   AsArray()        { return NULL; }
        virtual BlockObject *   AsBlock()        { return NULL; }
        virtual DynamicObject * AsDynamic()      { return NULL; }
        virtual FiberObject *   AsFiber()        { return NULL; }
        
        Ref<Scope> ObjectScope();
        
        Ref<Object> GetParent() { return mParent; }
        void        SetParent(Ref<Object> parent) { mParent = parent; }
        
        virtual void Trace(ostream & stream) const = 0;
        
    protected:
        Object() : mParent(Ref<Object>()) {}
        Object(Ref<Object> parent) : mParent(parent) {}
        
        Ref<Object> Parent() const { return mParent; }
        
    private:
        Ref<Object> mParent;
        Ref<Scope>  mScope;
    };
    
    ostream & operator<<(ostream & cout, const Object & object);
}

