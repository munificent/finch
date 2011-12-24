#include <iostream>

#include "ArrayTests.h"
#include "LexerTests.h"
#include "QueueTests.h"
#include "RefTests.h"
#include "StackTests.h"
#include "StringTests.h"
#include "TokenTests.h"


int main (int argc, char * const argv[])
{
    using namespace Finch;
    
    ArrayTests::Run();
    LexerTests::Run();
    QueueTests::Run();
    RefTests::Run();
    StackTests::Run();
    StringTests::Run();
    TokenTests::Run();
    
    Test::ShowResults();
    return 0;
}
