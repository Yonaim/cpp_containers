#ifndef FT_RB_TREE_H
#define FT_RB_TREE_H

// #include <memory> // TODO: ft::allocator로 교체
#include "allocator.h"
#include "iterator_tags.h"
#include "rb_tree_node.h"
#include "rb_tree_iterator.h"

/*
- STL 구현상 tree는 map, set에서만 쓰임
- RBT(Red Black Tree)만 사용
- 따라서 stl_map.h = 사실상 RBT 구현
- map, set의 내부 처리
    - 트리 알고리즘 자체는 동일
    - value 처리만 다르다 (compare를 어떤 타입에서 하고, 어떤 부분을 복사하는지 등)
*/

#include <cstddef>

namespace ft
{
    // =============================== Rb_tree =================================

    // Helper type to manage default initialization of node count and header.
    struct _Rb_tree_header
    {
        typedef _Rb_tree_node_base *_Base_ptr;
        _Base_ptr                   root;
        size_t                      count;
    };

    // _KeyOfValue: value에서 key를 뽑는 정책 (함수 객체)
    // KeyOfValue 덕분에 associative array까지 커버하는 '범용 트리 엔진'
    // value = pair<key, mapped>임. mapped_type과는 다르니 헷갈리지 말 것!
    template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
              typename _Alloc = allocator<_Val>>
    class _Rb_tree
    {
      public:
        // key & value
        typedef _Key              key_type;
        typedef _Val              value_type;
        typedef value_type       *pointer;
        typedef const value_type *const_pointer;
        typedef value_type       &reference;
        typedef const value_type &const_reference;

        // allocator
        typedef _Alloc                   allocator_type;
        typedef typename _Alloc::difference_type difference_type;
        typedef typename _Alloc::size_type       size_type;

        // node
        typedef _Rb_tree_node<_Val> _Node;
        typedef _Rb_tree_node_base  _Base;
        typedef _Node              *_Node_ptr;
        typedef _Base              *_Base_ptr;

        // iterator
        typedef _Rb_tree_iterator<_Val, _Val &, _Val *>             iterator;
        typedef _Rb_tree_iterator<_Val, const _Val &, const _Val *> const_iterator;

        // ==================== API ====================
        // allocation/deallocation
        _Rb_tree();
        _Rb_tree(const _Compare &, const allocator_type &);
        ~_Rb_tree();

        // iterators
        iterator       begin();
        const_iterator begin() const;
        iterator       end();
        const_iterator end() const;
        bool           empty() const;
        size_type      size() const { return std::distance(begin(), end()); }

        // find
        iterator       find(const key_type &k);
        const_iterator find(const key_type &k) const;

        // lower_bound / upper_bound / equal_range
        iterator                             lower_bound(const key_type &k);
        const_iterator                       lower_bound(const key_type &k) const;
        iterator                             upper_bound(const key_type &k);
        const_iterator                       upper_bound(const key_type &k) const;
        pair<iterator, iterator>             equal_range(const key_type &k);
        pair<const_iterator, const_iterator> equal_range(const key_type &k) const;

        // insert
        pair<iterator, bool> insert_unique(const value_type &v);
        iterator             insert_equal(const value_type &v);
        iterator             insert_unique(iterator position, const value_type &v);
        iterator             insert_equal(iterator position, const value_type &v);

        // erase
        /*
            왜 하필 이름이 erase?
            - remove는 std::remove에서 사용됨.
                std::remove는 '앞으로 옮겨서' 지워진 것처럼 보이게하는 함수이고
                할당된 공간을 회수하지 않음
            - destroy는 'destructor만 호출'을 의미 (allocator에서 사용)
            - delete는 예약된 C++ 키워드
        */
        void      erase(iterator position);
        size_type erase(const key_type &k);
        void      erase(iterator first, iterator last);

      private:
        _Rb_tree_header _header;

        // node create & destroy
        _Node_ptr _create_node(const value_type &v);
        void      _destroy_node(_Node_ptr);

        // balancing (fixup)
        void _insert_fixup(_Base_ptr x);
        void _erase_fixup(_Base_ptr x, _Base_ptr x_parent);

        // find min / max node (stateless)
        static _Node_ptr _minimum(_Node_ptr x);
        static _Node_ptr _maximum(_Node_ptr x);
    };

} // namespace ft

#endif
