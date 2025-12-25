#ifndef FT_RB_TREE_IMPL_H
#define FT_RB_TREE_IMPL_H

namespace ft
{

    /* ============================= constructor ============================ */

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Rb_tree() : _Base(), _key_compare()
    {
        _empty_initialize();
    }

    /* ============================= clear() ================================ */

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear()
    {
        if (this->_header.count == 0)
            return;

        _erase_subtree(_root());
        _leftmost() = this->_header._base_ptr;
        _rightmost() = this->_header._base_ptr;
        this->_header.count = 0;
    }

    /* ========================== iterator functions ========================== */

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::begin()
    {
        return iterator(_leftmost());
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::begin() const
    {
        return const_iterator(_leftmost());
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::end()
    {
        return iterator(_header._base_ptr);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::end() const
    {
        return const_iterator(_header._base_ptr);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::reverse_iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rbegin()
    {
        return reverse_iterator(_leftmost());
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_reverse_iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rbegin() const
    {
        return const_reverse_iterator(_leftmost());
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::reverse_iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rend()
    {
        return reverse_iterator(_header._base_ptr);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_reverse_iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rend() const
    {
        return const_reverse_iterator(_header._base_ptr);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    bool _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::empty() const
    {
        return this->_header.count == 0;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size() const
    {
        return this->_header.count;
    }

    /* =========================== find / lower / upper / equal_range ============ */

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type &k)
    {
        _Node_ptr x = _root();           // current node (currently searching)
        _Node_ptr y = _header._base_ptr; // last node which is not less than k

        // lower_bound를 먼저 찾는다
        while (x != NULL)
        {
            if (!_key_compare(_key(x), k)) // !(x < k) ?
            {
                y = x;
                x = _left(x);
            }
            else
                x = _right(x);
        }
        // 찾은 lower_bound가 k보다 작거나 같을시 -> 같은 값
        // 찾은 lower_bound가 k보다 클시 -> 다른 값
        iterator it = iterator(y);
        return (it == end() || _key_compare(k, _key(y))) ? end() : iterator(y);
    }

    // 찾는게 없으면 end() 리턴
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type &k) const
    {
        _Node_ptr x = _root();           // current node (currently searching)
        _Node_ptr y = _header._base_ptr; // last node which is not less than k

        // lower_bound를 먼저 찾는다
        while (x != NULL)
        {
            if (!_key_compare(_key(x), k)) // !(x < k) ?
            {
                y = x;
                x = _left(x);
            }
            else
                x = _right(x);
        }
        // 찾은 lower_bound가 k보다 작거나 같을시 -> 같은 값
        // 찾은 lower_bound가 k보다 클시 -> 다른 값
        iterator it = iterator(y);
        return (it == end() || _key_compare(k, _key(y))) ? end() : const_iterator(y);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type &k)
    {
        _Node_ptr x = _root();           // current node (currently searching)
        _Node_ptr y = _header._base_ptr; // last node which is not less than k

        while (x != NULL)
        {
            if (!_key_compare(_key(x), k))
            {
                y = x;
                x = _left(x);
            }
            else
                x = _right(x);
        }
        return iterator(y);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type &k) const
    {
        _Node_ptr x = _root();           // current node (currently searching)
        _Node_ptr y = _header._base_ptr; // last node which is not less than k

        while (x != NULL)
        {
            if (!_key_compare(_key(x), k))
            {
                y = x;
                x = _left(x);
            }
            else
                x = _right(x);
        }
        return iterator(y);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type &k)
    {
        _Node_ptr x = _root();           // current node (currently searching)
        _Node_ptr y = _header._base_ptr; // last node which is less than k

        while (x != NULL)
        {
            if (_key_compare(_key(x), k))
            {
                y = x;
                x = _left(x);
            }
            else
                x = _right(x);
        }
        return iterator(y);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type &k) const
    {
        _Node_ptr x = _root();           // current node (currently searching)
        _Node_ptr y = _header._base_ptr; // last node which is less than k

        while (x != NULL)
        {
            if (_key_compare(_key(x), k))
            {
                y = x;
                x = _left(x);
            }
            else
                x = _right(x);
        }
        return iterator(y);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    pair<typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
         typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type &k)
    {
        return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    pair<typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
         typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type &k) const
    {
        return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
    }

    /* =========================== insert / erase (skeleton) ===================== */

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    pair<typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type &v)
    {
        // TODO

        return pair<iterator, bool>(iterator(_rightmost()), false);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type &v)
    {
        // TODO
        return iterator(_rightmost());
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator          position,
                                                                    const value_type &v)
    {
        // TODO
        return iterator(_rightmost());
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(iterator          position,
                                                                   const value_type &v)
    {
        // TODO
        return iterator(_rightmost());
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position)
    {
        // TODO
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const key_type &k)
    {
        // TODO
        return 0;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last)
    {
        // TODO
    }

    /* =========================== Internal helpers ============================== */

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_root() const
    {
        return (_Node_ptr &)this->_header._base_ptr->parent;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_leftmost() const
    {
        return (_Node_ptr &)this->_header._base_ptr->left;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_rightmost() const
    {
        return (_Node_ptr &)this->_header._base_ptr->right;
    }

    /* static helpers */

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_left(_Node_ptr n)
    {
        return (_Node_ptr &)n->left;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_right(_Node_ptr n)
    {
        return (_Node_ptr &)n->right;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_parent(_Node_ptr n)
    {
        return (_Node_ptr &)n->parent;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::reference
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_value(_Node_ptr n)
    {
        return n->value;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    const typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::key_type &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_key(_Node_ptr n)
    {
        return KeyOfValue()(n->value);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Color_type &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_color(_Node_ptr n)
    {
        return (_Color_type &)n->color;
    }

    /* Base_ptr versions */

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_left(_Base_ptr b)
    {
        return (_Node_ptr &)b->left;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_right(_Base_ptr b)
    {
        return (_Node_ptr &)b->right;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_parent(_Base_ptr b)
    {
        return (_Node_ptr &)b->parent;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::reference
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_value(_Base_ptr b)
    {
        return reinterpret_cast<_Node_ptr>(b)->value;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    const typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::key_type &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_key(_Base_ptr b)
    {
        return KeyOfValue()(_value(b));
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Color_type &
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_color(_Base_ptr b)
    {
        return (_Color_type &)b->color;
    }

    /* minimum / maximum */

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_minimum(_Node_ptr x)
    {
        return (_Node_ptr)_Rb_tree_node_base::_minimum(x);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_maximum(_Node_ptr x)
    {
        return (_Node_ptr)_Rb_tree_node_base::_maximum(x);
    }

    /* =========================== Internal private methods ====================== */

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_empty_initialize()
    {
        this->_header._base_ptr->parent = 0;                      // root initially null
        this->_header._base_ptr->left = this->_header._base_ptr;  // leftmost = header
        this->_header._base_ptr->right = this->_header._base_ptr; // rightmost = header
        this->_header._base_ptr->color = _Rb_tree_color::RED;     // header color = red
        this->_header.count = 0;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_create_node(const value_type &v)
    {
        _Node_ptr n_ptr = this->_get_node();
        try
        {
            this->get_allocator().construct(&n_ptr->value, v);
        }
        catch (...)
        {
            this->_put_node(n_ptr);
            throw;
        }
        return n_ptr;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node_ptr
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_clone_node(_Node_ptr orig)
    {
        _Node_ptr n_ptr = _create_node(orig->value);
        n_ptr->color = orig->color;
        n_ptr->left = orig->left;
        n_ptr->right = orig->right;
        return n_ptr;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_destroy_node(_Node_ptr n_ptr)
    {
        this->get_allocator().destroy(&n_ptr->value);
        this->_put_node(n_ptr);
    }

    /* rotate */

    /* fixup */

    /*
        RED-BLACK 트리의 조건

        1. 모든 노드는 RED 혹은 BLACK
        2. 루트 노드는 BLACK
        3. 모든 리프 노드(NIL)들은 BLACK
        4. RED 노드의 자식은 BLACK
        5. 모든 리프 노드에서 Black Depth는 같다
            (Black Depth: 리프 노드에서 루트 노드까지의 경로에서 만나는 BLACK 노드의 개수)
        6. 새로운 노드는 항상 RED
    */

    // insert: 새 노드를 RED로 삽입 -> Double RED 문제 발생
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert_fixup(_Base_ptr x)
    {
        // TODO
    }

    // erase: 제거한 노드가 BLACK일 경우 -> Black Depth 불균형 문제 발생
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_erase_fixup(_Base_ptr x,
                                                                        _Base_ptr x_parent)
    {
        // TODO
    }

    /* subtree erase */

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_erase_subtree(_Node_ptr x)
    {
        // TODO
    }

} // namespace ft

#endif
