#pragma once

#include <iostream>
#include <memory>

#include "Macros.h"
#include "Expr.h"
#include "String.h"

namespace Finch
{
    using std::auto_ptr;
    
    // AST node for a keyword message send: "obj do: thing with: other"
    class KeywordExpr : public Expr
    {
    public:
        KeywordExpr(auto_ptr<Expr> object, String keywords[],
                    auto_ptr<Expr> args[], int numKeywords)
        :   mObject(object),
            mNumKeywords(numKeywords)
        {
            for (int i = 0; i < numKeywords; i++)
            {
                mKeywords[i] = keywords[i];
                mArgs[i]     = args[i];
            }
        }
        
        virtual void Trace(std::ostream & stream) const;

    private:
        static const int MAX_KEYWORDS = 10;
        
        // the object receiving the message
        auto_ptr<Expr> mObject;
        
        int mNumKeywords;
        
        // the names of the keywords
        String mKeywords[MAX_KEYWORDS];
        
        // the arguments being passed
        auto_ptr<Expr> mArgs[MAX_KEYWORDS];
    };
}