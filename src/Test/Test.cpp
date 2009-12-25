#include <iostream>

#include "Test.h"

namespace Finch
{
    using std::cout;
    using std::endl;
    
    int Test::sTests = 0;
    int Test::sFailed = 0;

    void Test::ShowResults()
    {
        cout << endl;
        cout << (sTests - sFailed) << " tests passed out of " << sTests << "." << endl;
    }
}