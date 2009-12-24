#include <iostream>

#ifdef UNIT_TESTS

#include "QueueTests.h"
#include "RefTests.h"
#include "TestMain.h"
#include "TokenTests.h"

namespace Finch
{
    void TestMain::Run()
    {
        std::cout << "running unit tests" << std::endl;
        
        QueueTests::Run();
        TokenTests::Run();
        RefTests::Run();
    }

}

#endif