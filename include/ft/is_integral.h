#ifndef IS_INTEGRAL_H
#define IS_INTEGRAL_H

#include "bool_type.h" // 컴파일 타임 평가
#include "remove_cv.h"

// template specialization 이용
/*
필요한 작업
1. const, volatile이 붙어있는 경우 그것을 떼 base로 변경하여 알맞은 구조체로 스위칭 (fallback)
2. base 타입으로 확인했는데도 불구하고 존재하지 않으면 false로 fallback
그러나, fallback은 하나의 specialization에서 한번만 가능
    => 내부 구현체와 API를 분리하여 각각의 fallback을 이용
*/
namespace ft
{
    // ========================== implementation ===============================
    template <class T>
    struct __is_integral_impl : false_type // fallback
    {
    };
    template <>
    struct __is_integral_impl<bool> : true_type
    {
    };
    template <>
    struct __is_integral_impl<char> : true_type
    {
    };
    template <>
    struct __is_integral_impl<signed char> : true_type
    {
    };
    template <>
    struct __is_integral_impl<unsigned char> : true_type
    {
    };
    template <>
    struct __is_integral_impl<wchar_t> : true_type
    {
    };
    template <>
    struct __is_integral_impl<short> : true_type
    {
    };
    template <>
    struct __is_integral_impl<unsigned short> : true_type
    {
    };
    template <>
    struct __is_integral_impl<int> : true_type
    {
    };
    template <>
    struct __is_integral_impl<unsigned int> : true_type
    {
    };
    template <>
    struct __is_integral_impl<long> : true_type
    {
    };
    template <>
    struct __is_integral_impl<unsigned long> : true_type
    {
    };
    template <>
    struct __is_integral_impl<long long> : true_type
    {
    };
    template <>
    struct __is_integral_impl<unsigned long long> : true_type
    {
    };

    // ================================ API ====================================
    // 상속 구조를 쓰는 이유: static 멤버 변수인 value를 갖다 쓰려고
    template <class T>
    struct is_integral : __is_integral_impl<typename remove_cv<T>::type>
    {
    };

} // namespace ft

#endif
