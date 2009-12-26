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
        String a = "";
        EXPECT_EQUAL(0, a.Length());

        String b = "b";
        EXPECT_EQUAL(1, b.Length());
        
        String c = "some string";
        EXPECT_EQUAL(11, c.Length());
    }
    
    void StringTests::TestSubscript()
    {
        String a = "abcd";
        EXPECT_EQUAL('a', a[0]);
        EXPECT_EQUAL('b', a[1]);
        EXPECT_EQUAL('c', a[2]);
        EXPECT_EQUAL('d', a[3]);
    }
    
    void StringTests::TestAddition()
    {
        {
            String a = "left";
            String b = "right";
            
            String c = a + b;
            EXPECT_EQUAL("leftright", c);
        }
        
        // left is null
        {
            String a;
            String b = "right";
            
            String c = a + b;
            EXPECT_EQUAL("right", c);
        }

        // right is null
        {
            String a = "left";
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
        String a = "abc";
        String b = a;
        
        EXPECT_EQUAL("abc", b);
    }
    
    void StringTests::TestCompoundAssignment()
    {
        String a = "abc";
        a += String("def");
        
        EXPECT_EQUAL("abcdef", a);
    }
    
    void StringTests::TestComparison()
    {
        String a1 = "abc";
        String a2 = "abc";
        String b  = "abd";

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
