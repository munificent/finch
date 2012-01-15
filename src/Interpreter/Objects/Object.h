#pragma once

#include <iostream>

#include "Array.h"
#include "ArgReader.h"
#include "Macros.h"
#include "Ref.h"
#include "FinchString.h"

#define PRIMITIVE(name)                                                     \
        Value name(Fiber & fiber, const Value & self, ArgReader & args)

namespace Finch
{
    using std::ostream;

    class Expr;
    class ArrayObject;
    class Block;
    class BlockObject;
    class DynamicObject;
    class Environment;
    class Fiber;
    class FiberObject;
    class Interpreter;
    class Object;

    typedef Value (*PrimitiveMethod)(Fiber & fiber, const Value & self,
                                           ArgReader & args);

    class Value
    {
    public:
        // Constructs a new null value.
        Value()
        :   mObj(NULL),
            mPrev(this),
            mNext(this)
        {}
        
        explicit Value(Object * obj)
        :   mObj(obj),
            mPrev(this),
            mNext(this)
        {}
        
        // Copies a value. If the copied value is a reference type, both values
        // will point to the same object.
        Value(const Value & other)
        :   mObj(NULL),
            mPrev(this),
            mNext(this)
        {
            if (&other != this)
            {
                Link(other);
            }
        }
        
        ~Value() { Clear(); }
        
        Object & operator *() const { return *mObj; }
        Object * operator ->() const { return mObj; }
        
        // Compares two values.
        bool operator ==(const Value & other) const
        {
            return mObj == other.mObj;
        }
        
        // Compares two values.
        bool operator !=(const Value & other) const
        {
            return mObj != other.mObj;
        }
        
        Value & operator =(const Value & other)
        {
            if (&other != this)
            {
                Clear();
                Link(other);
            }
            
            return *this;
        }
        
        // Gets whether or not this value is nil.
        bool IsNull() const { return mObj == NULL; }
        
        // Clears the reference. If this was the last reference to the referred
        // object, it will be deallocated.
        void Clear();
        
        const Value & Parent() const;
        
        double          AsNumber() const;
        String          AsString() const;
        ArrayObject *   AsArray() const;
        BlockObject *   AsBlock() const;
        DynamicObject * AsDynamic() const;
        FiberObject *   AsFiber() const;
        
    private:
        void Link(const Value & other);
        
        Object * mObj;

        mutable const Value * mPrev;
        mutable const Value * mNext;
    };
    
    // Base class for an object in Finch. All values in Finch inherit from this.
    class Object
    {
        friend class Value;
        
    public:
        virtual ~Object() {}

        // TODO(bob): It seems weird that these are on Object even though only
        // DynamicObject supports them.
        virtual Value FindMethod(StringId name) { return Value(); }
        virtual PrimitiveMethod FindPrimitive(StringId name) { return NULL; }

        virtual Value GetField(int name) { return Value(); }
        virtual void  SetField(int name, const Value & value) {}

        virtual double          AsNumber() const { return 0; }
        virtual String          AsString() const { return ""; }
        virtual ArrayObject *   AsArray()        { return NULL; }
        virtual BlockObject *   AsBlock()        { return NULL; }
        virtual DynamicObject * AsDynamic()      { return NULL; }
        virtual FiberObject *   AsFiber()        { return NULL; }

        const Value & Parent() const { return mParent; }
        // TODO(bob): Only used to set Object's parent to itself. Can we get
        // rid of this?
        void        SetParent(const Value & parent) { mParent = parent; }

        virtual void Trace(ostream & stream) const = 0;

    protected:
        Object(const Value & parent) : mParent(parent), mRefCount(1) {}

    private:
        Value mParent;
        int   mRefCount;
    };

    ostream & operator<<(ostream & cout, const Object & object);
}

