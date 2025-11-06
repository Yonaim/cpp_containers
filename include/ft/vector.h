#ifndef FT_VECTOR_H
#define FT_VECTOR_H

// Dummy implementation for linking only.
// Provides minimal definitions to avoid linker errors.

#include <cstddef>
#include <memory>
#include "equal.h"
#include "iterator_traits.h"
#include "lexicographical_compare.h"
#include "reverse_iterator.h"

namespace ft
{
    template <class T, class Alloc = std::allocator<T> >
    class vector
    {
      public:
        typedef T                                        value_type;
        typedef Alloc                                    allocator_type;
        typedef typename allocator_type::reference       reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;
        typedef std::ptrdiff_t                           difference_type;
        typedef std::size_t                              size_type;

        typedef pointer                              iterator;
        typedef const_pointer                        const_iterator;
        typedef ft::reverse_iterator<iterator>       reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

        // -------------------- constructors -------------------- //
        explicit vector(const allocator_type & = allocator_type()) {}
        explicit vector(size_type, const value_type & = value_type(),
                        const allocator_type & = allocator_type())
        {
        }
        template <class InputIterator>
        vector(InputIterator, InputIterator, const allocator_type & = allocator_type())
        {
        }
        vector(const vector &) {}
        ~vector() {}
        vector &operator=(const vector &) { return *this; }

        // -------------------- iterators -------------------- //
        iterator               begin() { return iterator(); }
        const_iterator         begin() const { return const_iterator(); }
        iterator               end() { return iterator(); }
        const_iterator         end() const { return const_iterator(); }
        reverse_iterator       rbegin() { return reverse_iterator(); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(); }
        reverse_iterator       rend() { return reverse_iterator(); }
        const_reverse_iterator rend() const { return const_reverse_iterator(); }

        // -------------------- capacity -------------------- //
        size_type size() const { return 0; }
        size_type max_size() const { return 0; }
        void      resize(size_type, value_type = value_type()) {}
        size_type capacity() const { return 0; }
        bool      empty() const { return true; }
        void      reserve(size_type) {}

        // -------------------- element access -------------------- //
        reference operator[](size_type)
        {
            static value_type tmp = value_type();
            return tmp;
        }
        const_reference operator[](size_type) const
        {
            static value_type tmp = value_type();
            return tmp;
        }
        reference at(size_type)
        {
            static value_type tmp = value_type();
            return tmp;
        }
        const_reference at(size_type) const
        {
            static value_type tmp = value_type();
            return tmp;
        }
        reference front()
        {
            static value_type tmp = value_type();
            return tmp;
        }
        const_reference front() const
        {
            static value_type tmp = value_type();
            return tmp;
        }
        reference back()
        {
            static value_type tmp = value_type();
            return tmp;
        }
        const_reference back() const
        {
            static value_type tmp = value_type();
            return tmp;
        }

        // -------------------- modifiers -------------------- //
        template <class InputIterator>
        void assign(InputIterator, InputIterator)
        {
        }
        void assign(size_type, const value_type &) {}

        void push_back(const value_type &) {}
        void pop_back() {}

        iterator insert(iterator, const value_type &) { return iterator(); }
        void     insert(iterator, size_type, const value_type &) {}
        template <class InputIterator>
        void insert(iterator, InputIterator, InputIterator)
        {
        }

        iterator erase(iterator) { return iterator(); }
        iterator erase(iterator, iterator) { return iterator(); }
        void     swap(vector &) {}
        void     clear() {}

        // -------------------- allocator -------------------- //
        allocator_type get_allocator() const { return allocator_type(); }
    };
} // namespace ft

namespace ft
{
    // ================== relational operators ================== //
    template <class T, class Alloc>
    bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class T, class Alloc>
    bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(lhs == rhs);
    }

    template <class T, class Alloc>
    bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class T, class Alloc>
    bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(rhs < lhs);
    }

    template <class T, class Alloc>
    bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return rhs < lhs;
    }

    template <class T, class Alloc>
    bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(lhs < rhs);
    }

    // ================== swap specialization ================== //
    template <class T, class Alloc>
    void swap(vector<T, Alloc> &lhs, vector<T, Alloc> &rhs)
    {
        lhs.swap(rhs);
    }
} // namespace ft

#endif // FT_VECTOR_H
