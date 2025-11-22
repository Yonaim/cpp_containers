// Reference: https://en.cppreference.com/w/cpp/utility/functional/unary_function

#ifndef FT_UNARY_FUNCTION_H
#define FT_UNARY_FUNCTION_H

/**
 * @brief Provides the typedefs for unary function objects.
 *
 * std::unary_function was a base class template for creating function objects 
 * with one argument. It provides two types:
 *  - argument_type
 *  - result_type
 *
 * Derived classes are expected to implement operator().
 * (Note: Removed in C++17; kept only for STL-like compatibility in this project.)
 */


namespace ft
{
    template <class ArgumentType, class ResultType>
    struct unary_function
    {
        typedef ArgumentType argument_type;
        typedef ResultType result_type;
    };
}

#endif
