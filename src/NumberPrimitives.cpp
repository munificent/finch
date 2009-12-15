#include "NumberPrimitives.h"
#include "NumberObject.h"
#include "EvalContext.h"

namespace Finch
{
    Ref<Object> NumberPlus(Ref<Object> thisRef, EvalContext & context,
                           String message, const vector<Ref<Object> > & args)
    {
        return Object::New(thisRef->AsNumber() + args[0]->AsNumber());
    }

    Ref<Object> NumberMinus(Ref<Object> thisRef, EvalContext & context,
                            String message, const vector<Ref<Object> > & args)
    {
        return Object::New(thisRef->AsNumber() - args[0]->AsNumber());
    }
    
    Ref<Object> NumberMultiply(Ref<Object> thisRef, EvalContext & context,
                               String message, const vector<Ref<Object> > & args)
    {
        return Object::New(thisRef->AsNumber() * args[0]->AsNumber());
    }
    
    Ref<Object> NumberDivide(Ref<Object> thisRef, EvalContext & context,
                             String message, const vector<Ref<Object> > & args)
    {
        //### bob: need to check for div by zero
        return Object::New(thisRef->AsNumber() / args[0]->AsNumber());
    }    

    Ref<Object> NumberAbs(Ref<Object> thisRef, EvalContext & context,
                          String message, const vector<Ref<Object> > & args)
    {
        double number = thisRef->AsNumber();        
        double value = (number < 0) ? -number : number;
        return Object::New(value);
    }

    Ref<Object> NumberNeg(Ref<Object> thisRef, EvalContext & context,
                          String message, const vector<Ref<Object> > & args)
    {
        return Object::New(-thisRef->AsNumber());
    }

    Ref<Object> NumberEquals(Ref<Object> thisRef, EvalContext & context,
                             String message, const vector<Ref<Object> > & args)
    {
        bool result = thisRef->AsNumber() == args[0]->AsNumber();
        return result ? context.True() : context.False();
    }

    Ref<Object> NumberNotEquals(Ref<Object> thisRef, EvalContext & context,
                                String message, const vector<Ref<Object> > & args)
    {
        bool result = thisRef->AsNumber() != args[0]->AsNumber();
        return result ? context.True() : context.False();
    }
    
    Ref<Object> NumberLessThan(Ref<Object> thisRef, EvalContext & context,
                               String message, const vector<Ref<Object> > & args)
    {
        bool result = thisRef->AsNumber() < args[0]->AsNumber();
        return result ? context.True() : context.False();
    }
    
    Ref<Object> NumberGreaterThan(Ref<Object> thisRef, EvalContext & context,
                                  String message, const vector<Ref<Object> > & args)
    {
        bool result = thisRef->AsNumber() > args[0]->AsNumber();
        return result ? context.True() : context.False();
    }
    
    Ref<Object> NumberLessThanOrEqual(Ref<Object> thisRef, EvalContext & context,
                                      String message, const vector<Ref<Object> > & args)
    {
        bool result = thisRef->AsNumber() <= args[0]->AsNumber();
        return result ? context.True() : context.False();
    }
    
    Ref<Object> NumberGreaterThanOrEqual(Ref<Object> thisRef, EvalContext & context,
                                         String message, const vector<Ref<Object> > & args)
    {
        bool result = thisRef->AsNumber() >= args[0]->AsNumber();
        return result ? context.True() : context.False();
    }
}