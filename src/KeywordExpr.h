#pragma once

#include <iostream>
#include <vector>

#include "Macros.h"
#include "Expr.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::vector;
    
    // AST node for a keyword message send: "obj do: thing with: other"
    class KeywordExpr : public Expr
    {
    public:
        KeywordExpr(Ref<Expr> object, vector<String> keywords,
                    vector<Ref<Expr> > args)
        :   mObject(object),
            mKeywords(keywords),
            mArgs(args)
        {
            ASSERT(mKeywords.size() == mArgs.size(),
                   "Must have same number of keywords and arguments.");
        }
        
        virtual void Trace(std::ostream & stream) const;

    private:
        // the object receiving the message
        Ref<Expr> mObject;
        
        int mNumKeywords;
        
        // the names of the keywords
        vector<String> mKeywords;
        
        // the arguments being passed
        vector<Ref<Expr> > mArgs;
    };
}