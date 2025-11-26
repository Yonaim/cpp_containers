#ifndef FT_DISTANCE_H
#define FT_DISTANCE_H

#include "iterator_traits.h"
#include "iterator_tags.h"

/*
===============================================================================
    ft::distance
-------------------------------------------------------------------------------
  역할:
    - 반복자 구간[first, last]의 거리를 구한다.
    - iterator_category에 따라 서로 다른 알고리즘 사용 (Tag dispatch)

  동작 원리:
    1) Random Access Iterator
         → last - first  (O(1))
    2) 그 외 Input/Forward/Bidirectional Iterator
         → first != last까지 1씩 증가하면서 카운팅 (O(n))

  iterator_category 계층 구조:

        input_iterator_tag
            ├── forward_iterator_tag
            │       └── bidirectional_iterator_tag
            │               └── random_access_iterator_tag
        output_iterator_tag

  참고:
    - std::distance와 완전히 동일한 행동
    - RandomAccessIterator 여부는 iterator_category로 판단
===============================================================================
*/

/*
    tag dispatch시, 반환형이 아니라 파라미터를 이용해야 함!
    왜???

    -> c++에서 오버로드를 결정할 때 고려하는 건 '매개변수 타입'이기 때문.
        '반환형'은 고려하지 않는다!
        (즉, 매개변수 타입이 같고 반환형이 다른 함수들에 대해 오버로드 충돌 발생)
*/

/*
    알고리즘 함수의 매개변수: first, last -> 범위의 첫번째와 마지막
    container의 멤버 함수: begin(), end() -> 범위의 시작과 끝
*/

/*
    인자가 레퍼런스가 아닌 이유:
        1. 원본이 변경될 위험을 안고 굳이 레퍼런스로 받을 이유가 없음
        2. 어차피 복사 비용은 거의 0에 가까움 (memcpy)
*/

namespace ft
{
    // ------------------------------------------------------------------------
    // 1) Input Iterator 계열: 하나씩 증가하며 센다 (O(n))
    // ------------------------------------------------------------------------
    template <class InputIter>
    typename iterator_traits<InputIter>::difference_type distance(InputIter first, InputIter last,
                                                                  input_iterator_tag)
    {
        typedef typename iterator_traits<InputIter>::difference_type diff_type;
        diff_type                                                    d = 0;
        for (; first != last; ++first)
            ++d;
        return d;
    }

    // ------------------------------------------------------------------------
    // 2) Random Access Iterator: 단순 차 (O(1))
    // ------------------------------------------------------------------------
    template <class RandomAccessIter>
    typename iterator_traits<RandomAccessIter>::difference_type
    distance(RandomAccessIter first, RandomAccessIter last, random_access_iterator_tag)
    {
        return last - first;
    }

    // ------------------------------------------------------------------------
    // 3) public interface: tag dispatch
    // ------------------------------------------------------------------------
    template <class Iter>
    typename iterator_traits<Iter>::difference_type distance(Iter first, Iter last)
    {
        typedef typename iterator_traits<Iter>::iterator_category category;
        return distance(first, last, category());
    }
} // namespace ft

#endif // FT_DISTANCE_H
