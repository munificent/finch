#pragma once

#include <iostream>
#include <cstdlib>

#ifdef DEBUG

#define ASSERT(condition, message)                      \
    if (!(condition)) {                                 \
        std::cout << "ASSERTION FAILED " << __FILE__    \
                  << ":" << __LINE__ << " - "           \
                  << message << std::endl;              \
        abort();                                        \
    }

#define ASSERT_NOT_NULL(value)                          \
    ASSERT(value != NULL, "Expression " #value " cannot be null.")

#define ASSERT_RANGE(value, max)                        \
    if (((value) < 0) || ((value) >= max)) {            \
        std::cout << "ASSERTION FAILED " << __FILE__    \
                  << ":" << __LINE__ << " - "           \
                  << "Value " << value                  \
                  << " was out of range [0, " << max    \
                  << ")." << std::endl;                 \
        abort();                                        \
    }

#define ASSERT_STRING_NOT_EMPTY(value)                  \
    ASSERT(value.Length() > 0, "String " #value " cannot be empty.")

#else

#define ASSERT(condition, message)     ;
#define ASSERT_NOT_NULL(value)         ;
#define ASSERT_INDEX(index, max)       ;
#define ASSERT_STRING_NOT_EMPTY(value) ;

#endif

// Use this inside a class declaration to prevent the compiler from creating
// the default copy constructor and assignment operators for the class. Note
// that starts a private section, so you should either use this at the end of
// the declaration or before a privacy declaration.
#define NO_COPY(className)                          \
    private:                                        \
        className(const className &);               \
        className& operator=(const className &)

