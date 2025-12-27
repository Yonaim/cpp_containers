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
    즉, rbegin()은 끝 원소의 뒤쪽을 가리키고 rend()는 맨 처음 앞쪽을 가리킨다.
*/

/*
!!!! reverse_iterator는 단지 wrapper임 !!!
- 별도의 const_reverse_iterator 타입을 따로 만들지 않음.
- 원본 Iter의 타입에 따라 const인지 아닌지 결정됨.
- 즉 reverse_iterator 자체가 템플릿.

ex)
    typedef reverse_iterator<iterator>             reverse_iterator;
    typedef reverse_iterator<const_iterator>       const_reverse_iterator;
*/

namespace ft
{
    // Iter: iterator 혹은 const_iterator 타입만 허용!
    // 이외 타입이 오는 경우, 아래 type alias에서 컴파일 오류
    template <class Iter>
    class reverse_iterator
    {
      protected:
        Iter current; // 내부에서 사용하는 정방향 iterator
        // 사용자가 직접 접근하지는 못하지만, 상속 클래스는 접근 가능하도록 protected로

      public:
        typedef Iter iterator_type;
        // iterator_traits를 사용하는 이유: Iter 타입에서 타입 정보 갖고오는 정책을 일관화
        typedef typename iterator_traits<Iter>::value_type        value_type;
        typedef typename iterator_traits<Iter>::difference_type   difference_type;
        typedef typename iterator_traits<Iter>::pointer           pointer;
        typedef typename iterator_traits<Iter>::reference         reference;
        typedef typename iterator_traits<Iter>::iterator_category iterator_category;

        // ====================== constructor ======================
        reverse_iterator() : current() {}
        explicit reverse_iterator(iterator_type it) : current(it) {}

        /*
        conversion constructor (변환 생성자) (타입 U -> T)
            - 절대 원시 타입 변환을 위한 변환이 아님!! 오직 iterator용
            - 타입 U: iterator -> 타입 T: const_iterator
            - reverse_const_iterator -> const_reverse_iterator 변환 지원을 위함
            - 그냥 iterator에서 const_reverse_iterator로 바로 변환은 불가
        */
        template <class U>
        reverse_iterator(const reverse_iterator<U> &other) : current(other.base())
        {
        }

        // ====================== base() ======================
        iterator_type base() const { return current; }

        // ====================== operators ======================

        // * : 참조 반환
        reference operator*() const
        {
            Iter tmp = current;
            --tmp;
            return *tmp;
        }

        // -> : 포인터 반환 (체이닝 o)
        /*
            it->value를 해석하면,
            it.operator->()->value가 됨

            1) iterator의 연산자 오버로딩 ->에 의해 포인터가 반환
            2) pointer의 연산자(기본) ->에 의해 value에 접근

            포인터를 반환하는 이유: 기본 포인터의 역할(체이닝 + 접근 가능)을 그대로 수행하기 위함
        */
        pointer operator->() const { return &(operator*()); }

        // iterator의 증감 연산자는 이터레이터 그 자체를 반환함 (체이닝 가능)
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
        // difference_type: 두 iterator 사이의 disrance를 나타내는 표준화된 정수 타입
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

    /*
    비교 함수가 멤버 함수가 아닌 이유:
        - Iter의 타입이 일치하지 않아도 비교 연산이 가능하도록 하기 위해
        - Iter 타입이 일치하지 않을시, conversion constructor (T -> U)가 호출됨
            - 멤버 함수일 경우 이런 과정은 일어날 수 없음
                (멤버 함수일 경우 무조건 타입 T는 일치해야하기 때문)
        - 둘 중 lhs가 먼저오니 rhs가 lhs의 타입에 맞게 변환될 수 있는지 우선적으로 확인
            - 안되면 lhs가 rhs 타입으로 변환 가능한지 확인
    */
    template <class Iter>
    bool operator==(const reverse_iterator<Iter> &lhs, const reverse_iterator<Iter> &rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <class Iter>
    bool operator!=(const reverse_iterator<Iter> &lhs, const reverse_iterator<Iter> &rhs)
    {
        return lhs.base() != rhs.base();
    }

    template <class Iter>
    bool operator<(const reverse_iterator<Iter> &lhs, const reverse_iterator<Iter> &rhs)
    {
        return lhs.base() > rhs.base();
    }

    template <class Iter>
    bool operator>(const reverse_iterator<Iter> &lhs, const reverse_iterator<Iter> &rhs)
    {
        return lhs.base() < rhs.base();
    }

    template <class Iter>
    bool operator<=(const reverse_iterator<Iter> &lhs, const reverse_iterator<Iter> &rhs)
    {
        return lhs.base() >= rhs.base();
    }

    template <class Iter>
    bool operator>=(const reverse_iterator<Iter> &lhs, const reverse_iterator<Iter> &rhs)
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
