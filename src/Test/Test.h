#pragma once

#include <iostream>

#include "String.h"

#define EXPECT(condition) \
_Expect(__FILE__, __LINE__, #condition, condition)

#define EXPECT_MSG(condition, message) \
_ExpectMsg(__FILE__, __LINE__, condition, message)

#define EXPECT_EQUAL(expected, actual) \
_ExpectEqual(__FILE__, __LINE__, #actual, expected, actual)

namespace Finch
{
    using std::cout;
    using std::endl;
        
    // Base class for test classes.
    class Test
    {
    public:
        static void ShowResults();
        
    protected:
        static void _Expect(const char * file, int line,
                            const char * expression,
                            bool condition)
        {
            sTests++;
            if (!condition)
            {
                cout << "FAIL: " << expression << " was false." << endl;
                cout << "      " << file << ":" << line << endl;
                sFailed++;
            }
        }
        
        static void _ExpectMsg(const char * file, int line,
                            bool condition, const char * message)
        {
            sTests++;
            if (!condition)
            {
                cout << "FAIL: " << message << endl;
                cout << "      " << file << ":" << line << endl;
                sFailed++;
            }
        }

        template <typename Left, typename Right>
        static void _ExpectEqual(const char* file, int line,
                                 const char* actualExpression,
                                 const Left & expected, const Right & actual)
        {
            sTests++;
            if (expected != actual)
            {
                cout << "FAIL: Expected " << actualExpression << " to be " <<
                    expected << ", but was " << actual << "." << endl;
                cout << "      " << file << ":" << line << endl;
                sFailed++;
            }
        }
        
    private:
        static int sTests;
        static int sFailed;
    };

}

