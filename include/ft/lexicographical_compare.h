#ifndef LEXICOGRAPHIUCAL_H
#define LEXICOGRAPHIUCAL_H

// 두 시퀀스를 사전식(lexicographical)으로 비교한다.
// 첫번째 구간의 원소가 두번째 구간의 원소보다 작으면 true
// 두 시퀀스의 길이는 다를 수도 있다
// 앞의 것들이 다 똑같다면, 짧은 쪽이 더 작다
namespace ft
{
    template <class InputIt1, class InputIt2>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
    {
        /*
             전위 연산 vs 후위 연산
            - 후위 연산자가 포함된 식은 '원래 값'을 반환해야하는 규칙이 있음
            - 따라서 후위 연산자 사용시 '원래 값'을 복사하는 과정이 수반됨
            => 전위 연산이 더 효율적이다
        */
        // (void)를 붙이는 이유: 마지막 식의 결과값은 가장 마지막에 나오는 first2로 간주됨.
        // unused 경고 방지
        for (; (first1 != last1) && (first2 != last2); ++first1, (void)++first2)
        {
            if (*first1 < *first2)
                return true;
            else if (*first1 > *first2)
                return false;
        }
        // 첫번째 구간의 길이 < 두번째 구간의 길이
        return (first1 == last1) && (first2 != last2);
    }

    template <class InputIt1, class InputIt2, class Compare>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
                                 Compare comp)
    {
        for (; (first1 != last1) && (first2 != last2); ++first1, (void)++first2)
        {
            if (comp(*first1, *first2))
                return true;
            else if (comp(*first1, *first2))
                return false;
        }
        return (first1 == last1) && (first2 != last2);
    }
} // namespace ft

#endif
