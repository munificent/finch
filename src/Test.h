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
    protected:
        static inline void _Expect(const char * file, int line,
                                   bool condition, const char * message)
        {
            if (!condition)
            {
                std::cout << "FAIL " << file << ":" << line << " - "
                     << message << std::endl;
            }
        }

        template <typename Left, typename Right>
        static inline void _ExpectEqual(const char* file, int line,
                                        const char* actualExpression,
                                        const Left & expected, const Right & actual)
        {
            if (expected != actual)
            {
                std::cout << "FAIL " << file << ":" << line << " - "
                << "Expected " << actualExpression << " to be "
                << expected << ", but was " << actual << "." << std::endl;
            }
        }
        
    private:
        static const int MESSAGE_LENGTH = 256;
    };

}