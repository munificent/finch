#include <iostream>

#include "String.h"
#include "TestMain.h"
#include "TokenTests.h"
#include "RefTests.h"

namespace Finch
{
    void TestMain::Run()
    {
        std::cout << "running unit tests" << std::endl;
        
        TokenTests::Run();
        RefTests::Run();
    }

}