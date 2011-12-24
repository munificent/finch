#include "ArrayTests.h"
#include "Array.h"

namespace Finch
{
    void ArrayTests::Run()
    {
        TestCtor();
        TestSubscript();
        TestRemove();
    }
    
    void ArrayTests::TestCtor()
    {
        {
            Array<int> array;
            
            EXPECT_EQUAL(0, array.Count());
            EXPECT_EQUAL(0, array.Capacity());
        }
        
        {
            Array<int> array(5);
            
            EXPECT_EQUAL(0, array.Count());
            EXPECT(array.Capacity() >= 5);
        }
    }
    
    void ArrayTests::TestSubscript()
    {
        Array<int> array;
        array.Add(1);
        array.Add(2);
        array.Add(3);
        
        EXPECT_EQUAL(1, array[0]);
        EXPECT_EQUAL(2, array[1]);
        EXPECT_EQUAL(3, array[2]);
        EXPECT_EQUAL(1, array[-3]);
        EXPECT_EQUAL(2, array[-2]);
        EXPECT_EQUAL(3, array[-1]);
    }
    
    void ArrayTests::TestRemove()
    {
        Array<char> array;
        array.Add('a');
        array.Add('b');
        array.Add('c');
        
        EXPECT_EQUAL(3, array.Count());
        
        array.Remove(1);

        EXPECT_EQUAL(2, array.Count());
        EXPECT_EQUAL('a', array[0]);
        EXPECT_EQUAL('c', array[1]);
        
        array.Remove(-2); // negative is from end
        
        EXPECT_EQUAL(1, array.Count());
        EXPECT_EQUAL('c', array[0]);
        
        array.Remove(0);
        
        EXPECT_EQUAL(0, array.Count());
    }
}

