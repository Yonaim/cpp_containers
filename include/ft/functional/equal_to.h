#ifndef FT_EQUAL_TO_H
#define FT_EQUAL_TO_H

#include "binary_function.h"

namespace ft
{
    // 함수 객체
    // binary_function을 상속 받았으므로 내부에 정의되어있는 typedef들을 갖다쓸 수 있다
    template <typename T>
    struct equal_to : public binary_function<T, T, bool>
    // Arg1 = T, Arg2 = T, Result = bool
    {
        bool operator()(const T &lhs, const T &rhs) const { return lhs == rhs; }
    };
} // namespace ft

#endif
