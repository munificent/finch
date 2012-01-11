#include "ArrayTests.h"
#include "Array.h"

namespace Finch
{
    void ArrayTests::Run()
    {
        TestCtor();
        TestSubscript();
        TestRemoveAt();
        TestTruncate();
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
    
    void ArrayTests::TestRemoveAt()
    {
        Array<char> array;
        array.Add('a');
        array.Add('b');
        array.Add('c');
        
        EXPECT_EQUAL(3, array.Count());
        
        array.RemoveAt(1);

        EXPECT_EQUAL(2, array.Count());
        EXPECT_EQUAL('a', array[0]);
        EXPECT_EQUAL('c', array[1]);
        
        array.RemoveAt(-2); // negative is from end
        
        EXPECT_EQUAL(1, array.Count());
        EXPECT_EQUAL('c', array[0]);
        
        array.RemoveAt(0);
        
        EXPECT_EQUAL(0, array.Count());
    }
    
    void ArrayTests::TestTruncate()
    {
        Array<char> array;
        array.Add('a');
        array.Add('b');
        array.Add('c');
        array.Add('d');
        array.Add('e');
        
        EXPECT_EQUAL(5, array.Count());
        
        // Truncate to greater size does nothing.
        array.Truncate(7);
        EXPECT_EQUAL(5, array.Count());
        
        // Truncate to same size does nothing.
        array.Truncate(5);
        EXPECT_EQUAL(5, array.Count());
        
        array.Truncate(3);
        EXPECT_EQUAL(3, array.Count());
        EXPECT_EQUAL('a', array[0]);
        EXPECT_EQUAL('b', array[1]);
        EXPECT_EQUAL('c', array[2]);

        array.Truncate(0);
        EXPECT_EQUAL(0, array.Count());
    }
}

