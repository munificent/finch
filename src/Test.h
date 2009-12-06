#pragma once

#include <iostream>

#include "String.h"

#define EXPECT(condition, message) \
_Expect(__FILE__, __LINE__, condition, message)

#define EXPECT_EQUAL(expected, actual) \
_ExpectEqual(__FILE__, __LINE__, expected, actual)

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
                                        const Left & expected, const Right & actual)
        {
            String message;
            
            message += "Expected ";
            message += expected;
            message += ", but was ";
            message += actual;
            
            _Expect(file, line, expected == actual, message.c_str());
        }
        
    private:
        static const int MESSAGE_LENGTH = 256;
    };

}