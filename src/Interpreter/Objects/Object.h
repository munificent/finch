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

    // Base class for an object in Finch. All values in Finch inherit from this.
    class Object
    {
    public:
        virtual ~Object() {}

        // TODO(bob): It seems weird that these are on Object even though only
        // DynamicObject supports them.
        virtual Ref<Object>     FindMethod(StringId name) { return Ref<Object>(); }
        virtual PrimitiveMethod FindPrimitive(StringId name) { return NULL; }

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

