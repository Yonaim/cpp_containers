#ifndef SWAP_H
#define SWAP_H

#include <cstddef> // size_t
namespace ft
{
    // Swaps the values a and b
    template <class T>
    inline void swap(T &a, T &b)
    {
        if (&a == &b)
            return;
        T tmp = a;
        a = b;
        b = tmp;
    }

    // Swaps the arrays a and b
    // 인자는 '배열의 참조'
    // 참조 타입의 배열이 아닌 배열 그 자체의 참조임을 표현하기 위해 []보다 &를 괄호로 우선
    template <class T, size_t N>
    inline void swap(T (&a)[N], T (&b)[N])
    {
        for (size_t i = 0; i < N; ++i)
            swap(a[i], b[i]);
    }
} // namespace ft

#endif
