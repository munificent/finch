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
    // - After a newline is emitted, any immediately following ones are
    //   discarded. This collapses duplicate newlines to a single one.
    //
    // - Any newlines after a token that cannot end an expression are discarded.
    //   Any newlines after a keyword, operator, "|", "def", or "set" will
    //   be ignored.
    //
    // - Any newlines after an opening group are discarded. This means newlines
    //   following a "(", "[", or "{" will be ignored.
    //
    // - Exactly one newline, if present, following a "}" on its own line will
    //   be discarded. This allows a single keyword expression to span multiple
    //   lines:
    //
    //   if: 1 < 2 then:
    //   {
    //       write: "true"
    //   }
    //   else: ' this rule causes the newline here to be discarded
    //   {
    //       write: "false"
    //   }
    //
    //   The "on its own line" ensures that single-line blocks work as expected:
    //
    //   if: foo then: { write: "foo" } ' newline will not be discarded here
    //   if: bar then: { write: "bar" }
    class LineNormalizer : public ITokenSource
    {
    public:
        LineNormalizer(ITokenSource * tokens)
        :   mTokens(tokens),
            mState(NORMALIZE_DEFAULT),
            mNewLine(true)
        {}
        
        virtual Ref<Token> ReadToken();
        
    private:
        enum State
        {
            NORMALIZE_DEFAULT,
            NORMALIZE_DISCARD,
            NORMALIZE_DISCARD_ONE
        };
        
        ITokenSource * mTokens;
        State          mState;
        bool           mNewLine;
        
        NO_COPY(LineNormalizer)
    };
}
