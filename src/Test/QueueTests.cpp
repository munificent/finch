#include "QueueTests.h"
#include "Queue.h"

namespace Finch
{
    void QueueTests::Run()
    {
        TestEnqueueDequeue();
        TestSerialEnqueue();
        TestMultipleEnqueue();
        TestCount();
        TestSubscript();
    }
    
    void QueueTests::TestEnqueueDequeue()
    {
        Queue<int, 3> queue;
        
        EXPECT_EQUAL(0, queue.Count());
        EXPECT_EQUAL(3, queue.Capacity());
        
        queue.Enqueue(2);
        
        EXPECT_EQUAL(1, queue.Count());
        EXPECT_EQUAL(2, queue[0]);
        
        int result = queue.Dequeue();
        
        EXPECT_EQUAL(0, queue.Count());
        EXPECT_EQUAL(2, result);
    }
    
    void QueueTests::TestSerialEnqueue()
    {
        Queue<int, 3> queue;
        
        for (int i = 0; i < 6; i++)
        {
            EXPECT_EQUAL(0, queue.Count());
            
            queue.Enqueue(i);
            int result = queue.Dequeue();
            
            EXPECT_EQUAL(i, result);
        }
    }
    
    void QueueTests::TestMultipleEnqueue()
    {
        Queue<int, 3> queue;
        
        queue.Enqueue(5);
        queue.Enqueue(6);
        
        EXPECT_EQUAL(2, queue.Count());
        EXPECT_EQUAL(5, queue.Dequeue());
        EXPECT_EQUAL(1, queue.Count());
        EXPECT_EQUAL(6, queue.Dequeue());
        EXPECT_EQUAL(0, queue.Count());

        queue.Enqueue(7);
        queue.Enqueue(8);
        
        EXPECT_EQUAL(2, queue.Count());
        EXPECT_EQUAL(7, queue.Dequeue());
        EXPECT_EQUAL(1, queue.Count());
        EXPECT_EQUAL(8, queue.Dequeue());
        EXPECT_EQUAL(0, queue.Count());
    }
    
    void QueueTests::TestCount()
    {
        Queue<int, 3> queue;
        
        EXPECT_EQUAL(0, queue.Count());
        queue.Enqueue(1);
        EXPECT_EQUAL(1, queue.Count());
        queue.Enqueue(1);
        EXPECT_EQUAL(2, queue.Count());
        queue.Enqueue(1);
        EXPECT_EQUAL(3, queue.Count());
        queue.Dequeue();
        EXPECT_EQUAL(2, queue.Count());
        queue.Enqueue(1);
        EXPECT_EQUAL(3, queue.Count());
        queue.Dequeue();
        EXPECT_EQUAL(2, queue.Count());
        queue.Dequeue();
        EXPECT_EQUAL(1, queue.Count());
        queue.Dequeue();
        EXPECT_EQUAL(0, queue.Count());
    }
    
    void QueueTests::TestSubscript()
    {
        Queue<int, 3> queue;
        
        queue.Enqueue(5);
        queue.Enqueue(6);

        EXPECT_EQUAL(5, queue[0]);
        EXPECT_EQUAL(6, queue[1]);
        
        queue.Dequeue();
        
        EXPECT_EQUAL(6, queue[0]);
        
        queue.Enqueue(7);
        queue.Enqueue(8);

        EXPECT_EQUAL(6, queue[0]);
        EXPECT_EQUAL(7, queue[1]);
        EXPECT_EQUAL(8, queue[2]);
    }
}

