#include "NumberPrimitives.h"
#include "NumberObject.h"
#include "Environment.h"

namespace Finch
{
    Ref<Object> NumberAdd(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args)
    {
        return Object::NewNumber(env, thisRef->AsNumber() + args[0]->AsNumber());
    }

    Ref<Object> NumberSubtract(Ref<Object> thisRef, Environment & env,
                            String message, const vector<Ref<Object> > & args)
    {
        return Object::NewNumber(env, thisRef->AsNumber() - args[0]->AsNumber());
    }
    
    Ref<Object> NumberMultiply(Ref<Object> thisRef, Environment & env,
                               String message, const vector<Ref<Object> > & args)
    {
        return Object::NewNumber(env, thisRef->AsNumber() * args[0]->AsNumber());
    }
    
    Ref<Object> NumberDivide(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args)
    {
        //### bob: need to check for div by zero
        return Object::NewNumber(env, thisRef->AsNumber() / args[0]->AsNumber());
    }    

    Ref<Object> NumberAbs(Ref<Object> thisRef, Environment & env,
                          String message, const vector<Ref<Object> > & args)
    {
        double number = thisRef->AsNumber();        
        double value = (number < 0) ? -number : number;
        return Object::NewNumber(env, value);
    }

    Ref<Object> NumberNeg(Ref<Object> thisRef, Environment & env,
                          String message, const vector<Ref<Object> > & args)
    {
        return Object::NewNumber(env, -thisRef->AsNumber());
    }

    Ref<Object> NumberEquals(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args)
    {
        bool result = thisRef->AsNumber() == args[0]->AsNumber();
        return result ? env.True() : env.False();
    }

    Ref<Object> NumberNotEquals(Ref<Object> thisRef, Environment & env,
                                String message, const vector<Ref<Object> > & args)
    {
        bool result = thisRef->AsNumber() != args[0]->AsNumber();
        return result ? env.True() : env.False();
    }
    
    Ref<Object> NumberLessThan(Ref<Object> thisRef, Environment & env,
                               String message, const vector<Ref<Object> > & args)
    {
        bool result = thisRef->AsNumber() < args[0]->AsNumber();
        return result ? env.True() : env.False();
    }
    
    Ref<Object> NumberGreaterThan(Ref<Object> thisRef, Environment & env,
                                  String message, const vector<Ref<Object> > & args)
    {
        bool result = thisRef->AsNumber() > args[0]->AsNumber();
        return result ? env.True() : env.False();
    }
    
    Ref<Object> NumberLessThanOrEqual(Ref<Object> thisRef, Environment & env,
                                      String message, const vector<Ref<Object> > & args)
    {
        bool result = thisRef->AsNumber() <= args[0]->AsNumber();
        return result ? env.True() : env.False();
    }
    
    Ref<Object> NumberGreaterThanOrEqual(Ref<Object> thisRef, Environment & env,
                                         String message, const vector<Ref<Object> > & args)
    {
        bool result = thisRef->AsNumber() >= args[0]->AsNumber();
        return result ? env.True() : env.False();
    }
}