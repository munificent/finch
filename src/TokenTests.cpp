#include "TokenTests.h"
#include "Token.h"

namespace Finch
{
    void TokenTests::Run()
    {
        Ref<Token> token = Token::New(TOKEN_NAME, "name");
        
        EXPECT_EQUAL(TOKEN_NAME, token->Type());
        EXPECT_EQUAL("name", token->Text());
    }
}