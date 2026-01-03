#ifndef FT_VECTOR_3PTR_IMPL_H
#define FT_VECTOR_3PTR_IMPL_H

#include "vector_3ptr.h"

namespace ft
{
    // -------------------- Constructors -------------------- //

    template <class _Tp, class _Alloc>
    vector<_Tp, _Alloc>::vector(const allocator_type &a = allocator_type())

    {
    }

    // Fill constructor (with specified value)
    template <class _Tp, class _Alloc>
    vector<_Tp, _Alloc>::vector(size_type n, const _Tp &value,
                                const allocator_type &a = allocator_type())
    {
    }

    template <class _Tp, class _Alloc>
    vector<_Tp, _Alloc>::vector(size_type n)
    {
    }

    // Copy constructor
    template <class _Tp, class _Alloc>
    vector<_Tp, _Alloc>::vector(const vector<_Tp, _Alloc> &x)
    {
    }

    template <class _Tp, class _Alloc, class _InputIterator>
    vector<_Tp, _Alloc>::vector(
        InputIt first, InputIt last, const Allocator &alloc = Allocator(),
        typename ft::enable_if<!ft::is_integral<InputIt>::value, void>::type * = 0)
    {
    }

    template <class _Tp, class _Alloc, class _InputIterator>
    vector<_Tp, _Alloc>::vector(_InputIterator first, _InputIterator last,
                                const allocator_type &a = allocator_type())
    {
    }

    // Destructor
    template <class _Tp, class _Alloc>
    vector<_Tp, _Alloc>::~vector<_Tp, _Alloc>::vector()
    {
    }

    // Copy assignment operator
    vector<_Tp, _Alloc> &vector::operator=(const vector<_Tp, _Alloc> &x) {}
    void                 vector::reserve(size_type n) {}

    // -------------------- Modifiers -------------------- //

    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::push_back(const _Tp &x)
    {
    }

    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::push_back()
    {
    }

    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::pop_back()
    {
    }

    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::swap(vector<_Tp, _Alloc> &x)
    {
    }

    template <class _Tp, class _Alloc>
    iterator vector<_Tp, _Alloc>::insert(iterator position, const _Tp &x)
    {
    }

    template <class _Tp, class _Alloc>
    iterator vector<_Tp, _Alloc>::insert(iterator position)
    {
    }

    template <class _Tp, class _Alloc, class _InputIterator>
    void vector<_Tp, _Alloc>::insert(iterator pos, _InputIterator first, _InputIterator last)
    {
    }

    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::insert(iterator pos, size_type n, const _Tp &x)
    {
    }

    template <class _Tp, class _Alloc>
    iterator vector<_Tp, _Alloc>::erase(iterator position)
    {
    }

    template <class _Tp, class _Alloc>
    iterator vector<_Tp, _Alloc>::erase(iterator first, iterator last)
    {
    }

    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::resize(size_type new_size, const _Tp &x)
    {
    }

    template <class _Tp, class _Alloc>
    void clear()
    {
        erase(begin(), end());
    }

    /* ------------------ Internal private methods ------------------  */
    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::_range_check(size_type n) const
    {
    }

} // namespace ft

#endif
