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

    /* =========================== insert / erase ===================== */

    /*
        - 기본값: 우측(right) 방향에 삽입
        - 일반 탐색 삽입이 아닌 힌트 insert의 경우, 좌측 삽입을 원할 수 있음
        - 이를 위해 x를 플래그로 이용 (Null일시 일반 탐색 삽입으로 간주, non-Null일시 좌측 삽입)
        - x는 Null 혹은 non-Null로만 구분하며 값은 중요하지 않음
    */
    // _insert: 이미 탐색한 위치에 붙이기 + fixup만 담당하는 하위 루틴
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert(_Node_ptr x_hint, _Node_ptr y,
                                                              const value_type &v)
    {
        // x_hint: 강제 왼쪽 삽입 유무 플래그
        // y: 삽입할 위치의 부모 노드
        // z: 삽입할 노드
        _Node_ptr z = _create_node(v);

        // 왼쪽에 달기
        if (y == _header._base_ptr || x_hint != NULL || _key_compare(key(v), key(y)))
        {
            y->left = z;
            if (y == _header.base_ptr) // 새로 삽입하는 노드가 루트
            {
                _root() = z;
                _rightmost() = z;
            }
            else if (y == _leftmost()) // 새로 삽입하는 노드가 제일 작음
                _leftmost() = z;
        }
        // 오른쪽에 달기
        else
        {
            y->right = z;
            else if (y == _rightmost()) // 새로 삽입하는 노드가 제일 큼
                _rightmost() = z;
        }
        z->parent = y;
        z->left = NULL;
        z->right = NULL;
        _rebalance_for_insert(z, _header._base_ptr->parent);
        ++_header.count;
        return iterator(z);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    pair<typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type &v)
    {
        _Node_ptr x = _root(); // currently searching
        _Node_ptr y;           // parent of x
        bool      comp = true;

        // 1) lower_bound 탐색
        // 부모 노드와 방향을 기억해야하므로 lower_bound()를 호출할 수 없음
        while (x != NULL)
        {
            y = x;
            comp = _key_compare(v, _key(x)); // v < x ?
            x = comp ? x->left : x->right;
        }

        // 2) 중복 검사
        iterator it = iterator(y);
        if (comp) // v < y
        {
            // y보다 작은 것은 확실하므로, y 바로 이전의 원소를 찾아 비교
            if (it == begin())
                // 현재 원소가 하나 밖에 없으므로 바로 삽입
                return pair<iterator, bool>(_insert(x, y, v), true);
            else
                --it;
        }
        // v와 *it가 같은지 확인 (v >= it)
        // v <= it인 경우, 즉 !(it < v)가 참인 경우 v == it
        if (compare_key_(_key(*it)), _key(v))
            return pair<iterator, bool>(_insert(x, y, v), true);
        // 중복 노드 존재로 삽입 실패
        return pair<iterator, bool>(it, false); // 중복 노드의 이터레이터 반환
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type &v)
    {
        _Node_ptr x = _root(); // currently searching
        _Node_ptr y;           // parent of x
        bool      comp = true;

        // lower_bound 탐색
        while (x != NULL)
        {
            y = x;
            comp = _key_compare(v, _key(x)); // v < x ?
            x = comp ? x->left : x->right;   // 같은 경우 오른쪽으로 계속 내려감
        }
        // 중복 검사 없음: 항상 삽입
        return _insert(x, y, v);
    }

    /*
        [position 힌트 기반 삽입]

        아래의 경우에 해당하면 O(1) 삽입이 가능하다.

        1. position이 begin이고 v < begin
        2. position이 end면 max < v
        3. before < v < position

        모두 해당되지 않으면 O(n) 탐색 필요 -> insert_unique
    */
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator          position,
                                                                    const value_type &v)
    {
        // 1. position = begin()
        if (position._base_node == _header._base_ptr->left)
        {
            if (size() > 0 && _key_compare(_key(v), _key(position._base_node))) // v < begin
                // v를 새로운 begin 삼는다 (left 삽입)
                // first argument just needs to be non-null (별 의미 없음)
                return _insert(position._base_node, position._base_node, v);
            else
                return insert_unique(v).first; // fallback
        }
        // 2. position = end()
        else if (position._base_node == _header._base_ptr->right)
        {
            if (_key_compare(_key(_rightmost()), _key(v))) // end < v
                // v를 새로운 max 삼는다
                return _insert(NULL, _rightmost(), v);
            else
                return insert_unique(v).first; // fallback
        }
        // 3. before < v < position인지 확인
        else
        {
            iterator before = position;
            --before;
            // before의 right child 혹은 position의 left child로 삽입 (탐색: O(1))
            // 불가능한 경우 탐색이 필요하므로 fallback
            if (_key_compare(_key(before._base_node), _key(v)) &&
                _key_compare(_key(v), _key(position._base_node)))
            {
                if (before.right == NULL) // before의 right child
                    return _insert(NULL, before._base_node, v);
                else // position의 left child로 삽입
                    return _insert(position._base_node, position._base_node, v);
            }
            else
                return insert_unique(v).first; // fallback
        }
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(iterator          position,
                                                                   const value_type &v)
    {
        // 1. position = begin()
        if (position._base_node == _header._base_node->left)
        {
            if (size() > 0 && !_key_compare(_key(position._base_node),
                                            _key(v))) // v <= begin
                                                      // v를 새로운 begin 삼는다 (left 삽입)
                // first argument just needs to be non-null (별 의미 없음)
                return _insert(position._base_node, position._base_node, v);
            else
                return insert_equal(v).first; // fallback
        }
        // 2. position = end()
        else if (position._base_node == _header._base_node->right)
        {
            if (!_key_compare(_key(_rightmost()), _key(v))) // v >= max
                                                            // v를 새로운 max 삼는다
                return _insert(NULL, _rightmost(), v);
            else
                return insert_equal(v).first; // fallback
        }
        // 3. before < v < position인지 확인
        else
        {
            iterator before = --position;
            // before의 right child 혹은 position의 left child로 삽입 (탐색: O(1))
            // 불가능한 경우 탐색이 필요하므로 fallback
            // before <= v <= position
            if (!_key_compare(_key(v), _key(before)) &&
                !_key_compare(_key(position._base_node), _key(v)))
            {
                if (before.right == NULL) // before의 right child
                    return _insert(NULL, before._base_node, v);
                else // position의 left child로 삽입
                    return _insert(position._base_node, position._base_node, v);
            }
            else
                return insert_equal(v).first; // fallback
        }
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    template <class _InputIterator>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(_InputIterator first,
                                                                         _InputIterator last)
    {
        for (; first != last; ++first)
            insert_unique(*first);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    template <class _InputIterator>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(_InputIterator first,
                                                                        _InputIterator last)
    {
        for (; first != last; ++first)
            insert_equal(*first);
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
    /*
        1. 회전 후에도 in-order(중위)순회 결과는 그대로여야함
        2. 색은 건들지 않고 부모-자식 포인터 관계만 재배치
    */
    // rotate_left(x): x->right가 위로 올라감
    /*
        예시 1) x가 루트인 경우

        x                     y
         \                   / \
          y      ==>         x   c
         / \                 \
        b   c                 b

    */
    /*
        예시 2) x가 루트가 아닌 경우

        p                          p
       / \                        / \
      x   (… )     ==>           y   (… )
     / \                        / \
    A   y                      x   C
       / \                    / \
      B   C                  A   B

    */
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_rotate_left(_Base_ptr x)
    {
        const _Base_ptr y = x->right; // y: x의 오른쪽 자식

        // 1) x의 오른쪽 자식 설정: y의 왼쪽 서브트리
        x->right = y->left;
        if (x->right)
            x->right->parent = x;

        // 2) y의 부모 설정: x의 부모
        y->parent = x->parent;
        if (x == _header._base_ptr->parent) // x = root
            _header._base_ptr->parent = y;
        else if (x = x->parent->left) // x는 부모의 좌측 자식
            x->parent->left = y;
        else
            x->parent->right = y;

        // 3) y의 왼쪽 자식 설정: x
        y->left = x;
        x->parent = y;
    }

    // rotate_right(x): x->left가 위로 올라감
    // rotate_left의 좌우대칭
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_rotate_right(_Base_ptr x)
    {
        const _Base_ptr y = x->left; // y: x의 왼쪽 자식

        // 1) x의 왼쪽 자식 설정: y의 우측 서브트리
        x->left = y->right;
        if (x->left)
            x->left->parent = x;

        // 2) y의 부모 설정: x의 부모
        y->parent = x->parent;
        if (x == _header._base_ptr->parent) // x = root
            _header._base_ptr->parent = y;
        else if (x = x->parent->left) // x는 부모의 좌측 자식
            x->parent->left = y;
        else
            x->parent->right = y;

        // 3) y의 우측 자식 설정: x
        y->right = x;
        x->parent = y;
    }

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
    // 부모 노드를 맨 마지막에 지우는 것이 안전
    // 즉 post-order (L-R-Root)
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_erase_subtree(_Node_ptr x)
    {
        // TODO
    }

} // namespace ft

#endif
