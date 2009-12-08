#include <iostream>

#include "Ref.h"
#include "Repl.h"

//#define UNIT_TEST

#ifdef UNIT_TEST
    #include "TestMain.h"
#endif

using namespace Finch;

int main (int argc, char * const argv[])
{
    
#ifdef UNIT_TEST
    
    TestMain::Run();
    
#else

    Repl repl;    
    repl.Run();
    
#endif
    
    return 0;
}
