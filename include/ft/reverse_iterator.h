#ifndef FT_REVERSE_ITERATOR_H
#define FT_REVERSE_ITERATOR_H

#include "iterator_traits.h"

/*
기존 iterator를 래핑(wrapping) 하는 클래스, 단지 역방향으로 작동하게 만드는 adapter

iterator와 대칭적 구조:
- iterator: begin() ~ end() (처음 → 끝)
- reverse_iterator: rbegin() ~ rend() (끝 → 처음)

rbegin() = end()
rend() = begin()
즉, rbegin()은 끝 원소의 **뒤쪽**을 가리키고 rend()는 맨 처음 **앞쪽**을 가리킨다.
*/

namespace ft
{
    template <class Iter>
    class reverse_iterator
    {
      protected:
        Iter current; // 내부에서 사용하는 정방향 iterator
                      // 사용자가 직접 접근하지는 못하지만, 상속 클래스는 접근 가능하도록
                      // protected로 둠

      public:
        typedef Iter                                              iterator_type;
        typedef typename iterator_traits<Iter>::value_type        value_type;
        typedef typename iterator_traits<Iter>::difference_type   difference_type;
        typedef typename iterator_traits<Iter>::pointer           pointer;
        typedef typename iterator_traits<Iter>::reference         reference;
        typedef typename iterator_traits<Iter>::iterator_category iterator_category;

        // ====================== constructor ======================
        reverse_iterator() : current() {}
        explicit reverse_iterator(iterator_type it) : current(it) {}
        template <class U>
        reverse_iterator(const reverse_iterator<U> &other) : current(other.base())
        {
        }

        // ====================== base() ======================
        iterator_type base() const { return current; }

        // ====================== operators ======================
        reference operator*() const
        {
            Iter tmp = current;
            --tmp;
            return *tmp;
        }

        pointer operator->() const { return &(operator*()); }

        reverse_iterator &operator++() // prefix
        {
            --current;
            return *this;
        }

        reverse_iterator operator++(int) // postfix
        {
            reverse_iterator tmp(*this);
            --current;
            return tmp;
        }

        reverse_iterator &operator--() // prefix
        {
            ++current;
            return *this;
        }

        reverse_iterator operator--(int) // postfix
        {
            reverse_iterator tmp(*this);
            ++current;
            return tmp;
        }

        // index operator
        reference operator[](difference_type n) const { return *(*this + n); }

        // arithmetic operators
        reverse_iterator operator+(difference_type n) const
        {
            return reverse_iterator(current - n);
        }

        reverse_iterator operator-(difference_type n) const
        {
            return reverse_iterator(current + n);
        }

        reverse_iterator &operator+=(difference_type n)
        {
            current -= n;
            return *this;
        }

        reverse_iterator &operator-=(difference_type n)
        {
            current += n;
            return *this;
        }
    };

    template <class Iterator>
    bool operator==(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <class Iterator>
    bool operator!=(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
    {
        return lhs.base() != rhs.base();
    }

    template <class Iterator>
    bool operator<(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
    {
        return lhs.base() > rhs.base();
    }

    template <class Iterator>
    bool operator>(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
    {
        return lhs.base() < rhs.base();
    }

    template <class Iterator>
    bool operator<=(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
    {
        return lhs.base() >= rhs.base();
    }

    template <class Iterator>
    bool operator>=(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator> &rhs)
    {
        return lhs.base() <= rhs.base();
    }

    // difference between two reverse_iterators
    template <class Iter>
    typename iterator_traits<Iter>::difference_type operator-(const reverse_iterator<Iter> &lhs,
                                                              const reverse_iterator<Iter> &rhs)
    {
        return rhs.base() - lhs.base();
    }
} // namespace ft

#endif
