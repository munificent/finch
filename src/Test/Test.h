#pragma once

#include <iostream>

#include "String.h"

#define EXPECT(condition, message) \
_Expect(__FILE__, __LINE__, condition, message)

#define EXPECT_EQUAL(expected, actual) \
_ExpectEqual(__FILE__, __LINE__, #actual, expected, actual)

namespace Finch
{
    // Base class for test classes.
    class Test
    {
    public:
        static void ShowResults();
        
    protected:
        static inline void _Expect(const char * file, int line,
                                   bool condition, const char * message)
        {
            sTests++;
            if (!condition)
            {
                std::cout << "FAIL " << sTests << " " << file << ":" << line << " - "
                     << message << std::endl;
                sFailed++;
            }
        }

        template <typename Left, typename Right>
        static inline void _ExpectEqual(const char* file, int line,
                                        const char* actualExpression,
                                        const Left & expected, const Right & actual)
        {
            sTests++;
            if (expected != actual)
            {
                std::cout << "FAIL " << sTests << " " << file << ":" << line << " - "
                << "Expected " << actualExpression << " to be "
                << expected << ", but was " << actual << "." << std::endl;
                sFailed++;
            }
        }
        
    private:
        static int sTests;
        static int sFailed;
    };

}

