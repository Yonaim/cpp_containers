// https://github.com/gcc-mirror/gcc/blob/releases/gcc-3.0/libstdc%2B%2B-v3/include/bits/stl_map.h
#ifndef FT_MAP_H
#define FT_MAP_H

#include <allocator.h> // TODO
#include <cstddef>
#include <functional>
#include "pair.h"
#include "stl_tree.h"

/*
- A sorted associative container that contains key-value pairs with 'unique' keys.
    - associative: key를 기준으로 데이터를 저장하고 찾음
- Keys are sorted by using the comparison function 'Compare'
- Search, removal, and insertion operations have logarithmic complexity
    - logarithmic complexity: 로그 복잡도, O(logN)
- Maps are usually implemented as Red–black trees.
*/

/*
map
 ├─ compare
 ├─ allocator
 └─ rb_tree
       ├─ header
       │    ├─ root
       │    ├─ leftmost (begin)
       │    ├─ rightmost (max)
       │    ├─ nil (sentinel)
       │    └─ node_count
       └─ tree algorithms (insert, erase, rotate, fixup, find...)
*/

namespace ft
{
    // map은 tree의 thin wrapper
    template <class Key, class T, class Compare = std::less<Key>,
              class Allocator = std::allocator<std::pair<const Key, T>>>
    class map
    {
      private:
        // red-black tree representing map
        typedef _Rb_tree <
            typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc>
                  _Rep_type;
        _Rep_type _tree;

      public:
        typedef Key                     key_type;
        typedef T                       mapped_type;
        typedef std::pair<const Key, T> value_type;
        typedef Compare                 key_compare;
        typedef Allocator               allocator_type;

        // value의 key(first)끼리 비교하는 함수 객체 타입
        // binary_function: 인자 2개인 함수 객체 (functor) 어댑터
        class value_compare : std::binary_function<value_type, value_type, bool>
        {
          public:
            /*
            C++98 STL functor 대부분은 binary_function이 제공하는 typedef를 그대로 사용한다.
            따라서 functor 내부에 typedef를 직접 작성할 필요는 없다.

            하지만 map::value_compare처럼 표준 문서에서 typedef가 명시된
            'public API용 functor'의 경우에는 typedef를 functor 내부에 직접 선언해야 한다.
            */
            typedef bool       result_type;
            typedef value_type first_argument_type;
            typedef value_type second_argument_type;

          protected:
            Compare comp;
            // Initializes the internal instance of the comparator to c.
            value_compare(Compare c) : comp(c) {};

          public:
            // Compares lhs.first and rhs.first by calling the stored comparator.
            bool operator()(const value_type &lhs, const value_type &rhs) const
            {
                returm comp(lhs.first, rhs.first);
            }
        }

        // =========================== Member types ============================

        typedef typename allocator_type::size_type       size_type;
        typedef typename allocator_type::difference_type difference_type;

        typedef value_type       &reference;
        typedef const value_type &const_reference;

        typedef typename allocator_type::pointer       pointer;
        typedef typename allocator_type::const_pointer const_pointer;

        typedef _Rb_tree_iterator<value_type>             iterator;
        typedef _Rb_tree_const_iterator<const value_type> const_iterator;

        typedef std::reverse_iterator<iterator>       reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        // =========================== Constructors ============================

        // Constructs an empty container
        map();
        explicit map(const Compare &comp, const Allocator &alloc = Allocator());

        // Constructs the container with the contents of the range [first, last)
        template <class InputIt>
        map(InputIt first, InputIt last, const Compare &comp = Compare(),
            const Allocator &alloc = Allocator());

        // Constructs the container with the copy of the contents of other
        map(const map &other);

        map &operator=(const map &other);
        // Returns the allocator associated with the container.
        allocator_type get_allocator() const;

        ~map();

        // ========================== Element access ===========================

        T       &at(const Key &key);
        const T &at(const Key &key) const;
        T       &operator[](const Key &key);

        // ========================== Iterators ===========================
        iterator       begin();
        const_iterator begin() const;
        // C++11
        const_iterator cbegin() const noexcept;
        iterator       end();
        const_iterator end() const;
        // C++11
        const_iterator cend() const noexcept;

        // ========================== Capacity ===========================
        bool      empty() const;
        size_type size() const
        {
            return std::distance(begin(), end());
            size_type max_size() const;
        }
        // ========================== Modifiers ===========================
        void clear();

        // Inserts value
        std::pair<iterator, bool> insert(const value_type &value);

        // Inserts value in the position as close as possible to the position just prior to pos.
        iterator insert(iterator pos, const value_type &value);

        // Inserts elements from range [first, last)
        template <class InputIt>
        void insert(InputIt first, InputIt last);

        // Removes the element at pos.
        iterator erase(iterator pos);

        // Removes the elements in the range [first, last)
        iterator erase(iterator first, iterator last);

        // Removes the element (if one exists) with the key equivalent to key
        size_type erase(const Key &key);

        // Exchanges the contents of the container with those of other
        void swap(map &other);

        // ========================== Lookup ===========================

        size_type count(const Key &key) const;

        iterator       find(const Key &key);
        const_iterator find(const Key &key) const;

        std::pair<iterator, iterator>             equal_range(const Key &key);
        std::pair<const_iterator, const_iterator> equal_range(const Key &key) const;
        iterator                                  lower_bound(const Key &key);
        const_iterator                            lower_bound(const Key &key) const;
        iterator                                  upper_bound(const Key &key);
        const_iterator                            upper_bound(const Key &key) const;

        // ========================== Observers ===========================

        key_compare   key_comp() const;
        value_compare value_comp() const;

      private:
        /* data */
    };

} // namespace ft

#endif
