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
        if (sFailed == 0)
        {
            cout << "SUCCESS: All " << sTests << " tests passed." << endl;
        }
        else
        {
            cout << endl;
            cout << "FAILURE: " << (sTests - sFailed) <<
                " tests passed out of " << sTests << "." << endl;
        }
    }
}

