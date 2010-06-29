#pragma once

#include "Macros.h"
#include "Ref.h"
#include "Token.h"
#include "ITokenSource.h"

namespace Finch
{
    // Takes in a raw token stream including line tokens, and normalizes it by
    // discarding newlines that should be ignored. The rules are:
    //
    // - Any leading newlines appearing at the beginning of the token stream are
    //   discarded.
    //
    // - After a newline is emitted, any immediately following ones are
    //   discarded. This collapses duplicate newlines to a single one.
    //
    // - Any newlines after a token that cannot end an expression are discarded.
    //   Any newlines after a keyword, operator, "|", "<-", or "<--" will be
    //   ignored.
    //
    // - Any newlines after an opening group are discarded. This means newlines
    //   following a "(", "[", or "{" will be ignored.
    //
    // Note that because newlines are significant, it's important to pay
    // attention to how they appear in expressions that use multiple blocks.
    // The following:
    //
    //   if: False {
    //      write: "true"
    //   } else: {
    //      write: "false"
    //   }
    //
    // will correctly print "false". However, this will not:
    //
    //   if: False
    //   {
    //      write: "true"
    //   }
    //   else:
    //   {
    //      write: "false"
    //   }
    //
    // The newline after the "}" ends the "if:" expression, and the following
    // "else:" is considered the beginning of a new expression.
    class LineNormalizer : public ITokenSource
    {
    public:
        LineNormalizer(ITokenSource & tokens)
        :   mTokens(tokens),
            mEatNewlines(true) // strip all leading newlines
        {}
        
        virtual Token ReadToken();
        
    private:
        ITokenSource & mTokens;
        bool           mEatNewlines;
        
        NO_COPY(LineNormalizer);
    };
}

