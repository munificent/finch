#include <iostream>

#include "InternStringPoolTests.h"
#include "LexerTests.h"
#include "QueueTests.h"
#include "RefTests.h"
#include "StackTests.h"
#include "StringTests.h"
#include "TestMain.h"
#include "TokenTests.h"

namespace Finch
{
    bool TestMain::Run()
    {
        std::cout << "Running unit tests:" << std::endl;
        
        InternStringPoolTests::Run();
        LexerTests::Run();
        QueueTests::Run();
        RefTests::Run();
        StackTests::Run();
        StringTests::Run();
        TokenTests::Run();

        Test::ShowResults();
        
        return Test::AllPassed();
    }
}
