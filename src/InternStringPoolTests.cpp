#include "InternStringPoolTests.h"
#include "InternStringPool.h"

namespace Finch
{
    void InternStringPoolTests::Run()
    {
        TestIntern();
    }
    
    void InternStringPoolTests::TestIntern()
    {
        InternStringPool pool;
        
        InternString a = pool.Intern("foo");
        InternString b = pool.Intern("bar");
        InternString c = pool.Intern("foo");
        
        EXPECT_NOT_EQUAL(a, b);
        EXPECT_NOT_EQUAL(a.CString(), b.CString());
        
        EXPECT_EQUAL(a, c);
        EXPECT_EQUAL(a.CString(), c.CString());
    }
}
