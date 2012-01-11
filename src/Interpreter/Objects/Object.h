#pragma once

#include <iostream>

#include "Array.h"
#include "Macros.h"
#include "Ref.h"
#include "FinchString.h"
#include "Scope.h"

#define PRIMITIVE(name)                                                     \
        Ref<Object> name(Fiber & fiber, Ref<Object> self, ArgReader & args)

namespace Finch
{
    using std::ostream;
    
    class Expr;
    class ArrayObject;
    class BlockExemplar;
    class BlockObject;
    class DynamicObject;
    class Environment;
    class Fiber;
    class FiberObject;
    class Interpreter;
    class Object;

    // TODO(bob): Move to other file.
    class ArgReader
    {
    public:
        ArgReader(Array<Ref<Object> > & stack, int firstArg, int numArgs)
        :   mStack(stack),
            mFirstArg(firstArg),
            mNumArgs(numArgs)
        {}
        
        int GetStackStart() const { return mFirstArg; }
        int GetNumArgs() const { return mNumArgs; }
        
        Ref<Object> & operator[] (int index)
        {
            ASSERT_RANGE(index, mNumArgs);
            return mStack[mFirstArg + index];
        }
        
    private:
        int mFirstArg;
        int mNumArgs;
        Array<Ref<Object> > & mStack;
    };
    
    typedef Ref<Object> (*PrimitiveMethod)(Fiber & fiber, Ref<Object> self,
                                           ArgReader & args);
    
    // Base class for an object in Finch. All values in Finch inherit from this.
    class Object
    {
    public:
        // virtual constructors
        static Ref<Object> NewObject(Ref<Object> parent, String name);
        static Ref<Object> NewObject(Ref<Object> parent);
        static Ref<Object> NewNumber(Environment & env, double value);
        static Ref<Object> NewString(Environment & env, String value);
        static Ref<Object> NewArray(Environment & env, int capacity);
        static Ref<Object> NewBlock(Environment & env, Ref<BlockExemplar> block, 
                                    Ref<Object> self);
        static Ref<Object> NewFiber(Interpreter & interpreter, Ref<Object> block);
        
        virtual ~Object() {}
        
        // TODO(bob): It seems weird that these are on Object even though only
        // DynamicObject supports them.
        // TODO(bob): Should make a typedef for string table IDs.
        virtual Ref<Object>     FindMethod(int name) { return Ref<Object>(); }
        virtual PrimitiveMethod FindPrimitive(int name) { return NULL; }
        
        virtual Ref<Object>     GetField(int name) { return Ref<Object>(); }
        virtual void            SetField(int name, Ref<Object> value) {}

        virtual double          AsNumber() const { return 0; }
        virtual String          AsString() const { return ""; }
        virtual ArrayObject *   AsArray()        { return NULL; }
        virtual BlockObject *   AsBlock()        { return NULL; }
        virtual DynamicObject * AsDynamic()      { return NULL; }
        virtual FiberObject *   AsFiber()        { return NULL; }
        
        Ref<Object> Parent() { return mParent; }
        // TODO(bob): Only used to set Object's parent to itself. Can we get
        // rid of this?
        void        SetParent(Ref<Object> parent) { mParent = parent; }
        
        virtual void Trace(ostream & stream) const = 0;
        
    protected:
        Object(Ref<Object> parent) : mParent(parent) {}
        
        Ref<Object> Parent() const { return mParent; }
        
    private:
        Ref<Object> mParent;
    };
    
    ostream & operator<<(ostream & cout, const Object & object);
}

