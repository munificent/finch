#define ASSERT_NOT_NULL(value)                      \
    ASSERT(value != NULL, "Expression " #value " cannot be null.")

#define ASSERT(condition, message)                      \
    if (!(condition)) {                                 \
        std::cout << "ASSERTION FAILED " << __FILE__    \
                  << ":" << __LINE__ << " - "           \
                  << message << std::endl;              \
        abort();                                        \
    }

// Use this inside a class declaration to prevent the compiler from creating
// the default copy constructor and assignment operators for the class. Note
// that starts a private section, so you should either use this at the end of
// the declaration or before a privacy declaration.
#define NO_COPY(className)                          \
    private:                                        \
        className(const className &);               \
        className& operator=(const className &);
