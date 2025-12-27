#ifndef FT_REMOVE_CV_H
#define FT_REMOVE_CV_H

// const, volatile이 붙어있을 때 떼어내는 용도로 사용한다.

namespace ft
{
    template <typename T>
    struct remove_cv // basic type (no CV)
    {
        typedef T type;
    };
    template <typename T>
    struct remove_cv<const T> // const type
    {
        typedef T type;
    };
    template <typename T>
    struct remove_cv<volatile T> // volatile type
    {
        typedef T type;
    };
    template <typename T>
    struct remove_cv<const volatile T> // const volatile type
    {
        typedef T type;
    };
} // namespace ft

#endif
