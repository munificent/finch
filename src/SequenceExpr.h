#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    // AST node for a pair of sequential expressions: "a b . c d"
    class SequenceExpr : public Expr
        {
        public:
            SequenceExpr(Ref<Expr> first, Ref<Expr> second)
            :   mFirst(first),
                mSecond(second)
            {}
            
            virtual void Trace(std::ostream & stream) const;
            
        private:
            Ref<Expr> mFirst;
            Ref<Expr> mSecond;
        };
}