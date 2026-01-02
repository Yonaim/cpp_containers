#ifndef FT_UNINITIALIZED_COPY_H
#define FT_UNINITIALIZED_COPY_H

#include "construct_guard.h"

/*
    - 초기화되지 않은 메모리 영역에 주어진 구간의 값을 copy
    - 메모리 순회 방식을 추상화 -> iterator 필요
    - 초기화되지 않은 메모리는 iterator로 나타낼 수 없기 때문에 value_type 포인터
    - 초기화되지 않은 메모리로 간주하므로 별도의 destroy 과정 거치지 않음
*/

namespace ft
{
    // value가 아닌 Allocator 타입이 템플릿 인자인 이유?
    // -> 후자에서 전자를 유도 가능, 전자에서 후자 유도는 불가능 (policy 객체이기 때문에)
    template <class _InputIt, class _Alloc>
    typename _Alloc::value_type *uninitialized_copy(_InputIt first, _InputIt last,
                                                    typename _Alloc::value_type *dest,
                                                    _Alloc                      &alloc)
    {
        typename _Alloc::value_type *cur = dest;
        construct_guard<_Alloc>      guard(alloc, dest);

        for (; first != last; ++first, ++cur)
        {
            alloc.construct(cur, *first);
            guard.bump();
        }
        // 만약 construct 중 실패시 c++ 예외 규칙에 의해 그 즉시 guard의 소멸자가 호출됨 (RAII)
        guard.release();
        return cur;
    }
} // namespace ft

#endif
