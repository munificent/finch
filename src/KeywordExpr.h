#pragma once

#include <iostream>
#include <vector>

#include "Macros.h"
#include "Expr.h"
#include "ExprVisitor.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::vector;
    
    // AST node for a keyword message send: "obj do: thing with: other"
    class KeywordExpr : public Expr
    {
    public:
        KeywordExpr(Ref<Expr> receiver, vector<String> keywords,
                    vector<Ref<Expr> > args)
        :   mReceiver(receiver),
            mKeywords(keywords),
            mArgs(args)
        {
            ASSERT(mKeywords.size() == mArgs.size(),
                   "Must have same number of keywords and arguments.");
        }
        
        Ref<Expr>                  Receiver()  const { return mReceiver; }
        
        const vector<String> &     Keywords()  const { return mKeywords; }
        const vector<Ref<Expr> > & Arguments() const { return mArgs; }
        
        virtual void Trace(std::ostream & stream) const;
        
        EXPRESSION_VISITOR

    private:
        // the object receiving the message
        Ref<Expr> mReceiver;
        
        // the names of the keywords
        vector<String> mKeywords;
        
        // the arguments being passed
        vector<Ref<Expr> > mArgs;
    };
}