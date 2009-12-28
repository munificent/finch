#include <iostream>

#include "LexerTests.h"
#include "QueueTests.h"
#include "RefTests.h"
#include "StringTests.h"
#include "TestMain.h"
#include "TokenTests.h"

namespace Finch
{
    void TestMain::Run()
    {
        std::cout << "Running unit tests:" << std::endl;
        
        LexerTests::Run();
        QueueTests::Run();
        RefTests::Run();
        StringTests::Run();
        TokenTests::Run();

        std::cout << "Finished unit tests." << std::endl;
    }

}
