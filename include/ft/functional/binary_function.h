// Reference: https://en.cppreference.com/w/cpp/utility/functional/binary_function

#ifndef FT_BINARY_FUNCTION_H
#define FT_BINARY_FUNCTION_H

/**
 * @brief Provides the typedefs for binary function objects.
 *
 * std::binary_function was a base class template for creating
 * function objects with two arguments. It provides three types:
 *  - first_argument_type
 *  - second_argument_type
 *  - result_type
 *
 * Derived classes are expected to implement operator().
 * (Note: Removed in C++17; kept only for STL-like compatibility in this project.)
 */

namespace ft
{
    template <class Arg1, class Arg2, class Result>
    struct binary_function
    {
        typedef Arg1   first_argument_type;
        typedef Arg2   second_argument_type;
        typedef Result result_type;
    };
} // namespace ft

#endif
