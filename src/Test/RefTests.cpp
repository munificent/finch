#include "RefTests.h"
#include "Ref.h"

namespace Finch
{
    class DestructorTester
    {
    public:
        DestructorTester()
        {
            sDestructed = false;
        }
        ~DestructorTester()
        {
            sDestructed = true;
        }
        
        static bool Destructed() { return sDestructed; }
    private:
        static bool sDestructed;
    };

    bool DestructorTester::sDestructed = 0;
    
    void RefTests::Run()
    {
        // dereferencing
        {
            Ref<int> r(new int(1234));
            
            // get the value back out
            EXPECT_EQUAL(1234, *r);
        }
        
        // nested reference
        {
            {
                Ref<DestructorTester> r1(new DestructorTester());
                
                {
                    Ref<DestructorTester> r2 = r1;
                    Ref<DestructorTester> r3 = r1;
                    Ref<DestructorTester> r4 = r2;
                }
                
                EXPECT_EQUAL(false, DestructorTester::Destructed());
            }
            
            EXPECT_EQUAL(true, DestructorTester::Destructed());
        }
        
        // chained reference
        {
            {
                Ref<DestructorTester> r1;
                
                {
                    Ref<DestructorTester> r2(new DestructorTester());
                    r1 = r2;
                }
                
                EXPECT_EQUAL(false, DestructorTester::Destructed());
            }
            
            EXPECT_EQUAL(true, DestructorTester::Destructed());
        }
        
        // setting to null
        {
            Ref<DestructorTester> r1(new DestructorTester());
            
            EXPECT_EQUAL(false, DestructorTester::Destructed());
            
            r1 = Ref<DestructorTester>();
            
            EXPECT_EQUAL(true, DestructorTester::Destructed());
        }
    }
}
