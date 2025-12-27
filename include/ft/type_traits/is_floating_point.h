#ifndef FT_IS_FLOATING_POINT_H
#define FT_IS_FLOATING_POINT_H

#include "bool_type.h" // 컴파일 타임 평가

// 간이 구현임: 오직 'float'에서만 true로 평가하도록 간단히 작성.
// template specialization 이용
namespace ft
{
    template <class T>
    struct is_floating_point : false_type // fallback
    {
    };

    template <>
    struct is_floating_point<float> : true_type
    {
    };
} // namespace ft

#endif
