#pragma once

#include <iostream>

#include "Array.h"
#include "ArgReader.h"
#include "Macros.h"
#include "Ref.h"
#include "FinchString.h"

#define PRIMITIVE(name)                                                     \
        Value name(Fiber & fiber, const Value & self, const ArgReader & args)

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
                                     const ArgReader & args);

    class Value
    {
    public:
        // Constructs a new null value.
        Value()
        :   mObj(NULL)
        {}
        
        explicit Value(Object * obj)
        :   mObj(obj)
        {
            // Don't increment refcount because Object's constructor initializes
            // it to 1.
        }
        
        // Copies a value. If the copied value is a reference type, both values
        // will point to the same object.
        Value(const Value & other);
        
        ~Value() { Clear(); }
        
        Value GetField(int name) const;
        void SetField(int name, const Value & value) const;
        
        Value SendMessage(Fiber & fiber, StringId messageId, const ArgReader & args) const;

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
        
        Value & operator =(const Value & other);
        
        // Gets whether or not this value is nil.
        bool IsNull() const { return mObj == NULL; }
        
        // Clears the reference. If this was the last reference to the referred
        // object, it will be deallocated.
        void Clear();
        
        const Value & Parent() const;
        
        void Trace(ostream & cout) const;
        
        double          AsNumber() const;
        String          AsString() const;
        ArrayObject *   AsArray() const;
        BlockObject *   AsBlock() const;
        DynamicObject * AsDynamic() const;
        FiberObject *   AsFiber() const;
        
    private:
        Object * mObj;
    };
    
    ostream & operator<<(ostream & cout, const Value & value);

    // Base class for an object in Finch. All values in Finch inherit from this.
    class Object
    {
        friend class Value;
        
    public:
        virtual ~Object() {}

        virtual double          AsNumber() const { return 0; }
        virtual String          AsString() const { return ""; }
        virtual ArrayObject *   AsArray()        { return NULL; }
        virtual BlockObject *   AsBlock()        { return NULL; }
        virtual DynamicObject * AsDynamic()      { return NULL; }
        virtual FiberObject *   AsFiber()        { return NULL; }

        const Value & Parent() const { return mParent; }

        virtual void Trace(ostream & stream) const = 0;

    protected:
        Object(const Value & parent) : mParent(parent), mRefCount(1) {}

    private:
        Value mParent;
        int   mRefCount;
    };
}

