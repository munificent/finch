#include "TokenTests.h"
#include "Token.h"

namespace Finch
{
    void TokenTests::Run()
    {
        Token token(TOKEN_NAME, "name");
        
        EXPECT_EQUAL(TOKEN_NAME, token.Type());
        EXPECT_EQUAL("name", token.Text());
    }
}

