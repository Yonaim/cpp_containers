#ifndef FT_VECTOR_3PTR_H
#define FT_VECTOR_3PTR_H

#include "iterator.h"
#include "memory.h"
#include "type_traits.h"

/*
    - 포인터 버전
*/

namespace ft
{
    // =============================== _Vector_alloc_base =================================

    /*
        base 클래스를 중간에 두는 이유:
        1. allocator의 stateless 유무에 따른 분기 처리
        2. exception-safe의 RAII 방식 처리
    */

    /*
        SGI/GCC 구현에서는 포인터 3종이 alloc_base 타입 상에 존재하는데,
        옛날 컴파일러에서는 간접적인 상속에 EBO가 적용되지 않는 경우가 있었기 때문
    */

    template <class _Tp, class _Alloc, bool _instanceless>
    class _Vector_alloc_base;

    // allcator의 instance가 없어도 OK
    // Empty class -> EBO 적용하여 0바이트
    template <class _Tp, class _Alloc>
    class _Vector_alloc_base<_Tp, _Alloc, true>
    {
      public:
        // TODO: Alloc_traits를 이용하는게 정석임
        // _Alloc이 _Tp 타입을 템플릿 인자로 받은 allocator 타입이라고 가정
        typedef _Alloc allocator_type;

        allocator_type get_allocator() const { return allocator_type(); }
        _Vector_alloc_base(const allocator_type &a) {}

      protected:
        _Tp *_allocate(size_t n) { return _Alloc::allocate(n); }
        void _deallocate(_Tp *p, size_t n)
        {
            if (p)
                _Alloc::deallocate(p, n);
        }
    };

    // allcator의 instance가 있어야 함
    template <class _Tp, class _Alloc>
    class _Vector_alloc_base<_Tp, _Alloc, false>
    {
        // _Alloc이 _Tp 타입을 템플릿 인자로 받은 allocator 타입이라고 가정
        typedef _Alloc allocator_type;

        allocator_type get_allocator() const { return allocator_type(); }
        _Vector_alloc_base(const allocator_type &a) : _data_allocator(a) {}

      protected:
        allocator_type _data_allocator;

        _Tp *_allocate(size_t n) { return _data_allocator.allocate(n); }
        void _deallocate(_Tp *p, size_t n)
        {
            if (p)
                _data_allocator.deallocate(p, n);
        }
    };

    // =============================== _Vector_base =================================

    // RAII로 메모리 관리
    template <class _Tp, class _Alloc>
    class _Vector_base : protected _Vector_alloc_base<_Tp, _Alloc, ft::is_empty<_Alloc>::value>
    {
      protected:
        _Tp *_start;
        _Tp *_finish;
        _Tp *_end_of_storage;

      private:
        typedef _Vector_alloc_base<_Tp, _Alloc, ft::is_empty<_Alloc>::value> _Base;
        typedef typename _Base::allocator_type                               allocator_type;

        _Vector_base(const allocator_type &a) : _Base(a) {}
        _Vector_base(size_t n, const allocator_type &a) : _Base(a)
        {
            _start = _allocate(n);
            _finish = _start;
            _end_of_storage = _start + n;
        }
        ~_Vector_base() { _deallocate(_start, _end_of_storage - _start); }
    };

    template <class _Tp, class _Alloc = ft::allocator<_Tp>>
    class vector
    {
      public:
        typedef _Tp                                         value_type;
        typedef value_type                                 *pointer;
        typedef const value_type                           *const_pointer;
        typedef normal_iterator<pointer, vector_type>       iterator;
        typedef normal_iterator<const_pointer, vector_type> const_iterator;
        typedef value_type                                 &reference;
        typedef const value_type                           &const_reference;
        typedef size_t                                      size_type;
        typedef ptrdiff_t                                   difference_type;

        typedef typename _Base::allocator_type allocator_type;
        using _Base::get_allocator();

      public:
        // iterator
        iterator       begin() { return iterator(_start); }
        const_iterator begin() const { return const_iterator(_start); }
        iterator       end() { return iterator(_finish); }
        const_iterator end() const { return const_iterator(_finish); }
        // reverse-iterator
        reverse_iterator       rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        reverse_iterator       rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

        // -------------------- Capacity -------------------- //
        size_type size() const { return size_type(end() - begin()); }
        size_type max_size() const {}
        size_type capacity() const { return const_iterator(_end_of_storage) - begin(); }
        bool      empty() const { return begin() == end(); }

        // -------------------- Element access -------------------- //

        // operator[] : 빠른 버전 (범위 밖이면 UB)
        // at() : 안전 버전 (범위 밖이면 예외 던짐)
        reference       operator[](size_type n) { return *(begin() + n); }
        const_reference operator[](size_type n) const { return *(begin() + n); }
        reference       at(size_type n)
        {
            _range_check(n);
            return operator[](n);
            // return (*this)[n];과 동일한 결과
        }
        const_reference at(size_type n) const
        {
            _range_check(n);
            return operator[](n);
        }

        // -------------------- Constructors -------------------- //
        // 인자가 하나인 생성자들에 대해서는 implicit conversion을 막아둔다

        explicit vector(const allocator_type &a = allocator_type());
        // Fill constructor (with specified value)
        vector(size_type n, const _Tp &value, const allocator_type &a = allocator_type());
        explicit vector(size_type n);
        // Copy constructor
        vector(const vector<_Tp, _Alloc> &x);
        template <class InputIt>
        vector(InputIt first, InputIt last, const Allocator &alloc = Allocator(),
               typename ft::enable_if<!ft::is_integral<InputIt>::value, void>::type * = 0);

        template <class _InputIterator>
        vector(_InputIterator first, _InputIterator last,
               const allocator_type &a = allocator_type());

        // Destructor
        ~vector();

        // Copy assignment operator
        vector<_Tp, _Alloc> &operator=(const vector<_Tp, _Alloc> &x);
        void                 reserve(size_type n);

        // -------------------- Element access -------------------- //
        reference       front() { return *begin(); }
        const_reference front() const { return *begin(); }
        reference       back() { return *(end() - 1); }
        const_reference back() const { return *(end() - 1); }

        // -------------------- Modifiers -------------------- //
        void push_back(const _Tp &x);
        void push_back();
        void pop_back();
        void swap(vector<_Tp, _Alloc> &x);

        iterator insert(iterator position, const _Tp &x);
        iterator insert(iterator position);
        template <class _InputIterator>
        void insert(iterator pos, _InputIterator first, _InputIterator last);
        void insert(iterator pos, size_type n, const _Tp &x);

        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);

        void resize(size_type new_size, const _Tp &x);
        void resize(size_type new_size) { resize(new_size, _Tp()); }

        void clear() { erase(begin(), end()); }

      private:
        /* ------------------ Internal private methods ------------------  */
        void _range_check(size_type n) const;
    };

    // -------------------- Non-member operators -------------------- //

    template <class _Tp, class _Alloc>
    bool operator==(const vector<_Tp, _Alloc> &lhs, const vector<_Tp, _Alloc> &rhs)
    {
        if (lhs.size() != rhs.size())
            return false;
        return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class _Tp, class _Alloc>
    bool operator!=(const vector<_Tp, _Alloc> &lhs, const vector<_Tp, _Alloc> &rhs)
    {
        return !(lhs == rhs);
    }

    template <class _Tp, class _Alloc>
    bool operator<(const vector<_Tp, _Alloc> &lhs, const vector<_Tp, _Alloc> &rhs)
    {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class _Tp, class _Alloc>
    bool operator<=(const vector<_Tp, _Alloc> &lhs, const vector<_Tp, _Alloc> &rhs)
    {
        return !(rhs < lhs);
    }

    template <class _Tp, class _Alloc>
    bool operator>(const vector<_Tp, _Alloc> &lhs, const vector<_Tp, _Alloc> &rhs)
    {
        return (rhs < lhs);
    }

    template <class _Tp, class _Alloc>
    bool operator>=(const vector<_Tp, _Alloc> &lhs, const vector<_Tp, _Alloc> &rhs)
    {
        return !(lhs < rhs);
    }
} // namespace ft

#include "vector_3ptr_impl.h"

#endif
