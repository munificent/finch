#pragma once

#include <iostream>

#include "Array.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "Macros.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for a keyword message send: "obj do: thing with: other"
    class KeywordExpr : public Expr
    {
    public:
        KeywordExpr(Ref<Expr> receiver, const Array<String> & keywords,
                    const Array<Ref<Expr> > & args)
        :   mReceiver(receiver),
            mKeywords(keywords),
            mArgs(args)
        {
            ASSERT(mKeywords.Count() == mArgs.Count(),
                   "Must have same number of keywords and arguments.");
        }
        
        Ref<Expr>                 Receiver()  const { return mReceiver; }
        
        const Array<String> &     Keywords()  const { return mKeywords; }
        const Array<Ref<Expr> > & Arguments() const { return mArgs; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << mReceiver;
            
            for (int i = 0; i < mKeywords.Count(); i++)
            {
                stream << " " << mKeywords[i] << " " << mArgs[i];
            }
        }
            
        EXPRESSION_VISITOR

    private:
        // the object receiving the message
        Ref<Expr> mReceiver;
        
        // the names of the keywords
        Array<String> mKeywords;
        
        // the arguments being passed
        Array<Ref<Expr> > mArgs;
    };
}

