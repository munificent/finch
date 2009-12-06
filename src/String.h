#include <string>

namespace Finch
{
    // Alias to the standard string. This lets us use "String" within the Snow
    // namespace without having to do "std::string" or "using std::string". It
    // also lets us change the type of this later (for example, to use
    // basic_string with a custom allocator) without breaking existing code.
    typedef std::string String;
}