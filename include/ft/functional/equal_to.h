#ifndef FT_EQUAL_TO_H
#define FT_EQUAL_TO_H

#include "binary_function.h"

namespace ft
{

    template <typename T>
    struct equal_to : public binary_function<T, T, bool>
    {
        bool operator()(const T &lhs, const T &rhs) const { return lhs == rhs; }
    };

} // namespace ft

#endif
