#include <iostream>

#include "QueueTests.h"
#include "RefTests.h"
#include "StringTests.h"
#include "TestMain.h"
#include "TokenTests.h"

namespace Finch
{
    void TestMain::Run()
    {
        std::cout << "running unit tests" << std::endl;
        
        QueueTests::Run();
        RefTests::Run();
        StringTests::Run();
        TokenTests::Run();
    }

}
