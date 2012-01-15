#pragma once

#include <iostream>

#include "Array.h"
#include "ArgReader.h"
#include "Macros.h"
#include "Ref.h"
#include "FinchString.h"

#define PRIMITIVE(name)                                                     \
        Ref<Object> name(Fiber & fiber, Ref<Object> self, ArgReader & args)

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

    typedef Ref<Object> (*PrimitiveMethod)(Fiber & fiber, Ref<Object> self,
                                           ArgReader & args);

    class Value
    {
    public:
        // TODO(bob): Temp until everything is using Value.
        static Value HackWrapRef(Ref<Object> obj)
        {
            return Value(obj);
        }
        
        static const Value NIL;
        
        Value(const Value & other)
        :   mObj(other.mObj)
        {}
        
        explicit Value(Object * obj)
        :   mObj(obj)
        {}
        
        Value()
        :   mObj()
        {}
        
        // Discards the currently referred to object and assigns the given
        // reference to this one.
        inline Value & operator =(const Value & other)
        {
            if (&other != this)
            {
                mObj = other.mObj;
            }
            
            return *this;
        }
        
        inline bool IsNull() const { return mObj.IsNull(); }
        
        double          AsNumber() const;
        String          AsString() const;
        ArrayObject *   AsArray();
        BlockObject *   AsBlock() const;
        DynamicObject * AsDynamic();
        FiberObject *   AsFiber();
        
        inline Ref<Object> Obj() const { return mObj; }
        
    private:
        // TODO(bob): Temp until everything is using Value.
        Value(Ref<Object> obj)
        :   mObj(obj)
        {}
        
        Ref<Object> mObj;
    };
    
    // Base class for an object in Finch. All values in Finch inherit from this.
    class Object
    {
    public:
        virtual ~Object() {}

        // TODO(bob): It seems weird that these are on Object even though only
        // DynamicObject supports them.
        virtual Ref<Object>     FindMethod(StringId name) { return Ref<Object>(); }
        virtual PrimitiveMethod FindPrimitive(StringId name) { return NULL; }

        virtual Value GetField(int name) { return Value(); }
        virtual void  SetField(int name, const Value & value) {}

        virtual double          AsNumber() const { return 0; }
        virtual String          AsString() const { return ""; }
        virtual ArrayObject *   AsArray()        { return NULL; }
        virtual BlockObject *   AsBlock()        { return NULL; }
        virtual DynamicObject * AsDynamic()      { return NULL; }
        virtual FiberObject *   AsFiber()        { return NULL; }

        const Value & Parent() { return mParent; }
        // TODO(bob): Only used to set Object's parent to itself. Can we get
        // rid of this?
        void        SetParent(const Value & parent) { mParent = parent; }

        virtual void Trace(ostream & stream) const = 0;

    protected:
        Object(const Value & parent) : mParent(parent) {}

    private:
        Value mParent;
    };

    ostream & operator<<(ostream & cout, const Object & object);
}

