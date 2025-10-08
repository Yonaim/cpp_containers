#ifndef EQUAL_H
#define EQUAL_H

// second가 더 짧은 경우: UB로 간주. 보호되지 않는다.

namespace ft
{
    template <class InputIt1, class InputIt2>
    bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
    {
        // while (first1 != last1)
        for (; first1 != last1; ++first1, (void)++first2)
        {
            if (*first1 != *first2)
                return false;
        }
        return true;
    }

    template <class InputIt1, class InputIt2, class BinaryPred>
    bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPred pred)
    {
        for (; first1 != last1; ++first1, (void)++first2)
        {
            if (!pred(*first1, *first2))
                return false;
        }
        return true;
    }
} // namespace ft

#endif
