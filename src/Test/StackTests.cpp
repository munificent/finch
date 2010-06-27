#include "StackTests.h"
#include "Stack.h"

namespace Finch
{
    void StackTests::Run()
    {
        TestPushPop();
        TestCount();
        TestSubscript();
    }
    
    void StackTests::TestPushPop()
    {
        Stack<int> stack;
        
        EXPECT_EQUAL(0, stack.Count());
        
        stack.Push(2);
        
        EXPECT_EQUAL(1, stack.Count());
        EXPECT_EQUAL(2, stack[0]);
        
        int result = stack.Pop();
        
        EXPECT_EQUAL(0, stack.Count());
        EXPECT_EQUAL(2, result);
    }
    
    void StackTests::TestCount()
    {
        Stack<int> stack;
        
        EXPECT_EQUAL(0, stack.Count());
        stack.Push(1);
        EXPECT_EQUAL(1, stack.Count());
        stack.Push(1);
        EXPECT_EQUAL(2, stack.Count());
        stack.Push(1);
        EXPECT_EQUAL(3, stack.Count());
        stack.Pop();
        EXPECT_EQUAL(2, stack.Count());
        stack.Push(1);
        EXPECT_EQUAL(3, stack.Count());
        stack.Pop();
        EXPECT_EQUAL(2, stack.Count());
        stack.Pop();
        EXPECT_EQUAL(1, stack.Count());
        stack.Pop();
        EXPECT_EQUAL(0, stack.Count());
    }
    
    void StackTests::TestSubscript()
    {
        Stack<int> stack;
        
        stack.Push(5);
        stack.Push(6);
        
        EXPECT_EQUAL(6, stack[0]);
        EXPECT_EQUAL(5, stack[1]);
        
        stack.Pop();
        
        EXPECT_EQUAL(5, stack[0]);
        
        stack.Push(7);
        stack.Push(8);
        
        EXPECT_EQUAL(8, stack[0]);
        EXPECT_EQUAL(7, stack[1]);
        EXPECT_EQUAL(5, stack[2]);
    }
}

