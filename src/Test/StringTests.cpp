#include "StringTests.h"
#include "String.h"

namespace Finch
{
    void StringTests::Run()
    {
        TestEmpty();
        TestLength();
        TestSubscript();
        TestAddition();
        TestAssignment();
        TestCompoundAssignment();
        TestComparison();
    }
    
    void StringTests::TestEmpty()
    {
        String a;
        
        EXPECT_EQUAL(0, a.Length());
        EXPECT_EQUAL('\0', *a.CString());
    }
    
    void StringTests::TestLength()
    {
        String a = String("");
        EXPECT_EQUAL(0, a.Length());

        String b = String("a");
        EXPECT_EQUAL(1, b.Length());
        
        String c = String("some string");
        EXPECT_EQUAL(11, c.Length());
    }
    
    void StringTests::TestSubscript()
    {
        String a = String("abcd");
        EXPECT_EQUAL('a', a[0]);
        EXPECT_EQUAL('b', a[1]);
        EXPECT_EQUAL('c', a[2]);
        EXPECT_EQUAL('d', a[3]);
    }
    
    void StringTests::TestAddition()
    {
        {
            String a = String("left");
            String b = String("right");
            
            String c = a + b;
            EXPECT_EQUAL("leftright", c);
        }
        
        // left is null
        {
            String a;
            String b = String("right");
            
            String c = a + b;
            EXPECT_EQUAL("right", c);
        }

        // right is null
        {
            String a = String("left");
            String b;
            
            String c = a + b;
            EXPECT_EQUAL("left", c);
        }
        
        // both null
        {
            String a;
            String b;
            
            String c = a + b;
            EXPECT_EQUAL(0, c.Length());
        }
    }
    
    void StringTests::TestAssignment()
    {
        String a = String("abc");
        String b = a;
        
        EXPECT_EQUAL("abc", b);
    }
    
    void StringTests::TestCompoundAssignment()
    {
        String a = String("abc");
        a += String("def");
        
        EXPECT_EQUAL("abcdef", a);
    }
    
    void StringTests::TestComparison()
    {
        String a1 = String("abc");
        String a2 = String("abc");
        String b  = String("abd");

        EXPECT_EQUAL(false, a1 <  a2);
        EXPECT_EQUAL(true,  a1 <= a2);
        EXPECT_EQUAL(false, a1 >  a2);
        EXPECT_EQUAL(true,  a1 >= a2);
        EXPECT_EQUAL(true,  a1 == a2);
        EXPECT_EQUAL(false, a1 != a2);
        
        EXPECT_EQUAL(true,  a1 <  b);
        EXPECT_EQUAL(true,  a1 <= b);
        EXPECT_EQUAL(false, a1 >  b);
        EXPECT_EQUAL(false, a1 >= b);
        EXPECT_EQUAL(false, a1 == b);
        EXPECT_EQUAL(true, a1 != b);
    }
}
