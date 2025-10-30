#ifndef IS_INTEGRAL_H
#define IS_INTEGRAL_H

#include "bool_type.h" // 컴파일 타임 평가

// 간이 구현임: 오직 'int'에서만 true로 평가하도록 간단히 작성.
// template specialization 이용
namespace ft
{
    template <class T>
    struct is_integral : false_type
    {
    };

    template <>
    struct is_integral<int> : true_type
    {
    };
} // namespace ft

#endif
