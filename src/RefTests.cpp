#include "RefTests.h"
#include "Ref.h"

namespace Finch
{
    class DestructorTester
    {
    public:
        ~DestructorTester()
        {
            sDestructed++;
        }
        
        static int Destructed() { return sDestructed; }
    private:
        static int sDestructed;
    };

    int DestructorTester::sDestructed = 0;
    
    void RefTests::Run()
    {
        // test ref-counting and assignment
        {
            Ref<int> r1(new int(1234));
            
            // get the value back out
            EXPECT_EQUAL(1234, *r1);
            
            {
                Ref<int> r2 = r1;
                
                // get the value back out
                EXPECT_EQUAL(1234, *r2);
            }
            
            // still get the value back out
            EXPECT_EQUAL(1234, *r1);
        }
        
        // test that object is actually freed when count hits zero
        {
            EXPECT_EQUAL(0, DestructorTester::Destructed());
            
            Ref<DestructorTester> r(new DestructorTester());
        }
        
        EXPECT_EQUAL(1, DestructorTester::Destructed());
    }
}