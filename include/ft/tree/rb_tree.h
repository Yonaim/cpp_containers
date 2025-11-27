#ifndef FT_RB_TREE_H
#define FT_RB_TREE_H

// #include <memory> // TODO: ft::allocator로 교체
#include "pair.h"
#include "allocator.h"
#include "distance.h"
#include "reverse_iterator.h"
#include "iterator_tags.h"
#include "rb_tree_node.h"
#include "rb_tree_iterator.h"

/*
    - STL 구현상 tree는 map, set에서만 쓰임
    - RBT(Red Black Tree)만 사용
    - 따라서 stl_map.h = 사실상 RBT 구현 $
    - map, set의 내부 처리
        - 트리 알고리즘 자체는 동일
        - value 처리만 다르다 (compare를 어떤 타입에서 하고, 어떤 부분을 복사하는지 등)
*/

/*
    - Rb_tree_header: 헤더 노드
    - Rb_tree_alloc_base: 노드 메모리 할당 / 해제만 담당(역할 분리)
        - instance 버전 (allocator를 멤버로 가짐)
        - instanceless 버전 (allocator를 멤버로 가지지 않음)
    - Rb_tree_base: intanceless 최적화를 위해 존재, 분기 처리하여 해당하는 alloc_base 상속 받음
    - Rb_tree: 메인 엔진
*/

/*
    EBO(Empty Base Optimization)
*/

#include <cstddef>
#include <type_traits>

namespace ft
{
    template <class T>
    struct is_instanceless
    {
        static const bool value = std::is_empty<T>::value;
    };

    // =============================== Rb_tree_header =================================

    struct _Rb_tree_header
    {
        typedef _Rb_tree_node_base *_Base_ptr;
        _Base_ptr                   _base_ptr;
        size_t count;
    };

    // =============================== Rb_tree_alloc_base =================================

    template <class _Tp, class _Alloc, bool _instanceless>
    class _Rb_tree_alloc_base;

    template <class _Tp, class _Alloc>
    class _Rb_tree_alloc_base<_Tp, _Alloc, false>
    {
      public:
        typedef _Alloc allocator_type;
        typedef _Tp    value_type;

        typedef typename _Alloc::template rebind<_Rb_tree_node<value_type>>::other node_allocator_type;
        typedef _Rb_tree_node<value_type>                                       node_type;

        allocator_type get_allocator() const { return allocator_type(_node_allocator); }

        _Rb_tree_alloc_base(const allocator_type &a) : _node_allocator(a) {}

      protected:
        node_allocator_type _node_allocator;

        node_type *_get_node() { return _node_allocator.allocate(1); }
        void       _put_node(node_type *p) { return _node_allocator.deallocate(p, 1); }
    };

    template <class _Tp, class _Alloc>
    class _Rb_tree_alloc_base<_Tp, _Alloc, true>
    {
      public:
        typedef _Alloc allocator_type;
        typedef _Tp    value_type;

        typedef typename _Alloc::template rebind<_Rb_tree_node<value_type>>::other node_allocator_type;
        typedef _Rb_tree_node<value_type>                                       node_type;

        allocator_type get_allocator() const { return allocator_type(); }

        _Rb_tree_alloc_base(const allocator_type &a) {}

      protected:
        node_type *_get_node()
        {
            node_allocator_type a;
            return a.allocate(1);
        }
        void _put_node(node_type *p)
        {
            node_allocator_type a;
            return a.deallocate(p, 1);
        }
    };

    // =============================== Rb_tree_base =================================

    template <class _Tp, class _Alloc>
    class _Rb_tree_base : private _Rb_tree_alloc_base<_Tp, _Alloc, is_instanceless<_Alloc>::value>
    {
        typedef _Rb_tree_alloc_base<_Tp, _Alloc, is_instanceless<_Alloc>::value> Base;

      protected:
        typedef typename Base::node_type node_type;

        _Rb_tree_header _header;

        using Base::_get_node;
        using Base::_put_node;
        using Base::get_allocator;

      public:
        _Rb_tree_base(const _Alloc &alloc = _Alloc()) : Base(alloc), _header() {}
        ~_Rb_tree_base() {}
    };

    // =============================== Rb_tree =================================

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc = allocator<_Value>>
    class _Rb_tree : protected _Rb_tree_base<_Value, _Alloc>
    {
        typedef _Rb_tree_base<_Value, _Alloc> _Base;

      public:
        // key & value
        typedef _Key              key_type;
        typedef _Value            value_type;
        typedef value_type       *pointer;
        typedef const value_type *const_pointer;
        typedef value_type       &reference;
        typedef const value_type &const_reference;

        typedef _Alloc                           allocator_type;
        typedef typename _Alloc::difference_type difference_type;
        typedef typename _Alloc::size_type       size_type;
        typedef _Rb_tree_node_base              *_Base_ptr;
        typedef _Rb_tree_node<_Value>            _Rb_tree_node;
        typedef _Rb_tree_node                   *_Node_ptr;
        typedef _Rb_tree_color                   _Color_type;

        typedef _Rb_tree_iterator<_Value, _Value &, _Value *>             iterator;
        typedef _Rb_tree_iterator<_Value, const _Value &, const _Value *> const_iterator;
        typedef reverse_iterator<iterator>                                reverse_iterator;
        typedef reverse_iterator<const_iterator>                          const_reverse_iterator;

        // ================================ API ================================
        _Rb_tree();
        ~_Rb_tree() { clear(); };

        void clear();

        iterator               begin();
        const_iterator         begin() const;
        iterator               end();
        const_iterator         end() const;
        reverse_iterator       rbegin();
        const_reverse_iterator rbegin() const;
        reverse_iterator       rend();
        const_reverse_iterator rend() const;
        bool                   empty() const;
        size_type              size() const;

        iterator       find(const key_type &k);
        const_iterator find(const key_type &k) const;

        iterator                             lower_bound(const key_type &k);
        const_iterator                       lower_bound(const key_type &k) const;
        iterator                             upper_bound(const key_type &k);
        const_iterator                       upper_bound(const key_type &k) const;
        pair<iterator, iterator>             equal_range(const key_type &k);
        pair<const_iterator, const_iterator> equal_range(const key_type &k) const;

        pair<iterator, bool> insert_unique(const value_type &v);
        iterator             insert_equal(const value_type &v);
        iterator             insert_unique(iterator position, const value_type &v);
        iterator             insert_equal(iterator position, const value_type &v);

        void      erase(iterator position);
        size_type erase(const key_type &k);
        void      erase(iterator first, iterator last);

      protected:
        _Compare _key_compare;

        _Node_ptr &_root() const;
        _Node_ptr &_leftmost() const;
        _Node_ptr &_rightmost() const;

        static _Node_ptr      &_left(_Node_ptr n);
        static _Node_ptr      &_right(_Node_ptr n);
        static _Node_ptr      &_parent(_Node_ptr n);
        static reference       _value(_Node_ptr n);
        static const key_type &_key(_Node_ptr n);
        static _Color_type    &_color(_Node_ptr n);

        static _Node_ptr      &_left(_Base_ptr b);
        static _Node_ptr      &_right(_Base_ptr b);
        static _Node_ptr      &_parent(_Base_ptr b);
        static reference       _value(_Base_ptr b);
        static const key_type &_key(_Base_ptr b);
        static _Color_type    &_color(_Base_ptr b);

        static _Node_ptr _minimum(_Node_ptr x);
        static _Node_ptr _maximum(_Node_ptr x);

      private:
        void _empty_initialize();
        _Node_ptr _create_node(const value_type &v);
        _Node_ptr _clone_node(_Node_ptr orig);
        void      _destroy_node(_Node_ptr n_ptr);

        void _insert_fixup(_Base_ptr x);
        void _erase_fixup(_Base_ptr x, _Base_ptr x_parent);
        void _erase_subtree(_Node_ptr x);
    };

} // namespace ft

#include "rb_tree_impl.h"
#endif
