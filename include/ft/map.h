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
    template <class Key, class T, class Compare = std::less<Key>,
              class Allocator = std::allocator<std::pair<const Key, T>>>
    class map
    {
      public:
        // =========================== Member types ============================
        typedef Key                     key_type;
        typedef T                       mapped_type;
        typedef std::pair<const Key, T> value_type;

        typedef Compare   key_compare;
        typedef Allocator allocator_type;

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

        map           &operator=(const map &other);
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
