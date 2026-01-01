#ifndef UNINITIALIZED_FILL_H
#define UNINITIALIZED_FILL_H

#include "construct_guard.h"

/*
    - 초기화되지 않은 메모리 영역에 주어진 값을 n번 채운다
*/

namespace ft
{
    template <class _InputIt, class _Alloc>
    void uninitialized_fill(typename _Alloc::value_type *dest, typename _Alloc::value_type value,
                            size_t n, _Alloc &alloc)
    {
        typename Alloc::value_type *cur = dest;
        _construct_guard<_Alloc>    guard(alloc, dest);

        for (size_t i = 0; i < n; ++i)
        {
            alloc.construct(cur + i, value);
            guard.bump();
        }
        // 만약 construct 중 실패시 c++ 예외 규칙에 의해 그 즉시 guard의 소멸자가 호출됨 (RAII)
        guard.release();
        return cur;
    }
} // namespace ft

#endif
