#ifndef FT_MAP_H
#define FT_MAP_H

#include <cstddef>
#include <functional>
#include <allocator.h> // TODO
#include "pair.h"

/*
- A sorted associative container that contains key-value pairs with 'unique' keys.
    - associative: key를 기준으로 데이터를 저장하고 찾음
- Keys are sorted by using the comparison function 'Compare'
- Search, removal, and insertion operations have logarithmic complexity
    - logarithmic complexity: 로그 복잡도, O(logN)
- Maps are usually implemented as Red–black trees.
*/
namespace ft
{
    template <class Key, class T, class Compare = std::less<Key>,
              class Allocator = std::allocator<std::pair<const Key, T>>>
    class map
    {
      public:
        // -------------------- Member types -------------------- //
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

        typedef tree_iterator<value_type>       iterator;
        typedef tree_iterator<const value_type> const_iterator;

        typedef std::reverse_iterator<iterator>       reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        map(/* args */);
        ~map();ㅎ

      private:
        /* data */
    };

} // namespace ft

#endif
