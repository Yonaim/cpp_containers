#ifndef FT_RB_TREE_IMPL_H
#define FT_RB_TREE_IMPL_H

#include "swap.h"
// TODO: 비멤버 함수로 변경할거 변경
// TODO: 템플릿 인자명 헤더와 통일

namespace ft
{

    /* ============================= constructor ============================ */

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Rb_tree() : _Base(), _key_compare()
    {
        _empty_initialize();
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Rb_tree(
        const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> &x)
        : _Base(x.get_allocator()), _key_compare(x._key_compare)
    {
        if (x._root() == NULL)
            _empty_initialize();
        else
        {
            // 불변식: root의 parent는 header이다
            _root_node() = _copy(x._root_node(), (_Node_ptr)this->_header._base_ptr);
            this->_header._base_ptr->left = _minimum(_root_node());  // leftmost = header
            this->_header._base_ptr->right = _maximum(_root_node()); // rightmost = header
            this->_header._base_ptr->color = RED;                    // header color = red
            this->_header.count = x._header.count;
        }
    }

    // explicit 키워드는 클래스 내부에서만 쓸 수 있음
    // 기본 인자는 정의에서는 못 쓰고 선언에서만 쓸 수 있음
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Rb_tree(const _Compare &comp,
                                                                    const _Alloc   &alloc)
        : _Base(alloc), _key_compare(comp)
    {
        _empty_initialize();
    }

    /* =============================== swap() =============================== */

    // 같은 템플릿 인자 타입을 갖는 other와 this의 데이터를 서로 바꾼다
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::swap(_Rb_tree &other)
    {
        if (this == &other)
            return;

        // 1) 헤더 노드 내용 + 노드 개수 교환 (포인터 멤버 _base_ptr은 교환하지 않음)
        ft::swap(this->_header._base_node, other._header._base_node);
        ft::swap(this->_header.count, other._header.count);
        ft::swap(this->_key_compare, other._key_compare);

        // 2) _base_ptr은 "항상 자기 _base_node"를 가리키게 재설정
        this->_header._base_ptr = &this->_header._base_node;
        other._header._base_ptr = &other._header._base_node;

        // 3) root가 있으면 root->parent를 새 헤더로 보정
        if (this->_header._base_node.parent)
            this->_header._base_node.parent->parent = this->_header._base_ptr;

        if (other._header._base_node.parent)
            other._header._base_node.parent->parent = other._header._base_ptr;
    }

    /* ============================== clear() =============================== */

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::clear()
    {
        if (this->_header.count == 0)
            return;
        _erase_subtree(_root());
        _leftmost() = (_Node_ptr)this->_header._base_ptr;
        _rightmost() = (_Node_ptr)this->_header._base_ptr;
        this->_header.count = 0;
    }

    /* ========================== iterator functions ========================== */

    /*
        - begin = leftmost()
        - end = header (rightmost가 아님!!! rightmost는 실존하는 원소)
        - reverse iterator는 iterator의 wrapper
    */
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::begin()
    {
        return iterator(_leftmost());
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::begin() const
    {
        return const_iterator(_leftmost());
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::end()
    {
        return iterator(_header._base_ptr);
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::end() const
    {
        return const_iterator(_header._base_ptr);
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::reverse_iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::rbegin()
    {
        return reverse_iterator(end());
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_reverse_iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::rbegin() const
    {
        return const_reverse_iterator(end());
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::reverse_iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::rend()
    {
        return reverse_iterator(begin());
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_reverse_iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::rend() const
    {
        return const_reverse_iterator(begin());
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    bool _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::empty() const
    {
        return this->_header.count == 0;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::size_type
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::size() const
    {
        return this->_header.count;
    }

    /* =========================== find / lower / upper / equal_range ============ */

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::find(const key_type &k)
    {
        _Base_ptr x = _root();           // current node (currently searching)
        _Base_ptr y = _header._base_ptr; // last node which is not less than k

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
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::find(const key_type &k) const
    {
        _Base_ptr x = _root();           // current node (currently searching)
        _Base_ptr y = _header._base_ptr; // last node which is not less than k

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

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::lower_bound(const key_type &k)
    {
        _Base_ptr x = _root();           // current node (currently searching)
        _Base_ptr y = _header._base_ptr; // last node which is not less than k

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

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::lower_bound(const key_type &k) const
    {
        _Base_ptr x = _root();           // current node (currently searching)
        _Base_ptr y = _header._base_ptr; // last node which is not less than k

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
        return const_iterator(y);
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::upper_bound(const key_type &k)
    {
        _Base_ptr x = _root();           // current node (currently searching)
        _Base_ptr y = _header._base_ptr; // last node which is less than k

        while (x != NULL)
        {
            if (_key_compare(k, _key(x)))
            {
                y = x;
                x = _left(x);
            }
            else
                x = _right(x);
        }
        return iterator(y);
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::upper_bound(const key_type &k) const
    {
        _Base_ptr x = _root();           // current node (currently searching)
        _Base_ptr y = _header._base_ptr; // last node which is less than k

        while (x != NULL)
        {
            if (_key_compare(k, _key(x)))
            {
                y = x;
                x = _left(x);
            }
            else
                x = _right(x);
        }
        return const_iterator(y);
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator,
         typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator>
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::equal_range(const key_type &k)
    {
        return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_iterator,
         typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_iterator>
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::equal_range(const key_type &k) const
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
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename ft::_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
    ft::_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_insert(_Base_ptr         x_hint,
                                                                       _Base_ptr         y,
                                                                       const value_type &v)
    {
        // x_hint: 강제 왼쪽 삽입 유무 플래그
        // y: 삽입할 위치의 부모 노드
        // z: 삽입할 노드
        _Node_ptr z = _create_node(v);

        // 왼쪽에 달기
        if (y == _header._base_ptr || x_hint != NULL || _key_compare(_key(v), _key(y)))
        {
            y->left = z;
            if (y == _header._base_ptr) // 새로 삽입하는 노드가 루트
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
            if (y == _rightmost()) // 새로 삽입하는 노드가 제일 큼
                _rightmost() = z;
        }
        z->parent = y;
        z->left = NULL;
        z->right = NULL;
        _rebalance_for_insert(z, _header._base_ptr->parent);
        ++_header.count;
        return iterator(z);
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const value_type &v)
    {
        _Base_ptr x = _root();           // currently searching
        _Base_ptr y = _header._base_ptr; // parent of x
        bool      comp = true;

        // 1) lower_bound 탐색
        // 부모 노드와 방향을 기억해야하므로 lower_bound()를 호출할 수 없음
        while (x != NULL)
        {
            y = x;
            comp = _key_compare(_key(v), _key(x)); // v < x ?
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
        if (_key_compare(_key(*it), _key(v)))
            return pair<iterator, bool>(_insert(x, y, v), true);
        // 중복 노드 존재로 삽입 실패
        return pair<iterator, bool>(it, false); // 중복 노드의 이터레이터 반환
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_equal(const value_type &v)
    {
        _Base_ptr x = _root();           // currently searching
        _Base_ptr y = _header._base_ptr; // parent of x
        bool      comp = true;

        // lower_bound 탐색
        while (x != NULL)
        {
            y = x;
            comp = _key_compare(_key(v), _key(x)); // v < x ?
            x = comp ? x->left : x->right;         // 같은 경우 오른쪽으로 계속 내려감
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
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator          position,
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
        else if (position._base_node == _header._base_ptr)
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
                if (before._base_node->right == NULL) // before의 right child
                    return _insert(NULL, before._base_node, v);
                else // position의 left child로 삽입
                    return _insert(position._base_node, position._base_node, v);
            }
            else
                return insert_unique(v).first; // fallback
        }
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_equal(iterator          position,
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

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    template <class _InputIterator>
    void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(_InputIterator first,
                                                                              _InputIterator last)
    {
        for (; first != last; ++first)
            insert_unique(*first);
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    template <class _InputIterator>
    void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_equal(_InputIterator first,
                                                                             _InputIterator last)
    {
        for (; first != last; ++first)
            insert_equal(*first);
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(iterator position)
    {
        _Base_ptr z = position._base_node;
        _Base_ptr y = _rebalance_for_erase(z);

        // 실제 노드 해제
        _destroy_node(static_cast<_Node_ptr>(y));
        --_header.count;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::size_type
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(const key_type &k)
    {
        pair<iterator, iterator> p = equal_range(k);
        size_type                n = ft::distance(p.first, p.second);
        erase(p.first, p.second);
        return n;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(iterator first, iterator last)
    {
        if (first == begin() && last == end())
            clear();
        else
            // for문일시 first가 가리키는 노드가 지워진 후 ++을 시도하려 할 수 있음
            // while문으로 작성하여 post increment(++)를 사용
            while (first != last)
                erase(first++);
    }

    // 포인터인 이유: 연속 메모리 배열의 시작 지점과 끝지점
    // 값 경계(low/high)가 아니라 리스트를 받는 것임
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(const key_type *first,
                                                                      const key_type *last)
    {
        while (first != last)
            erase(*first++);
    }

    /* =========================== Internal helpers ============================== */

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Base_ptr &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_root() const
    {
        /*
            - base_ptr -> derived_ptr로 다운캐스팅은 문법적으로 가능하지만,
                derived_ptr 참조 타입으로 캐스팅은 불가 (참조할 대상이 없다)
            - 타입 시스템을 무시한 메모리 재해석(reinterpret 캐스팅, type punning)은 가능하다
        */
        // return *reinterpret_cast<_Node_ptr*>(&this->_header._base_ptr->parent);
        // return static_cast<_Node_ptr &>(this->_header._base_ptr->parent);
        return (this->_header._base_ptr->parent);
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Node_ptr &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_root_node() const
    {
        return (_Node_ptr &)(_root());
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Base_ptr &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_leftmost() const
    {
        return this->_header._base_ptr->left;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Base_ptr &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_rightmost() const
    {
        return this->_header._base_ptr->right;
    }

    /* static helpers */

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Node_ptr &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_left(_Node_ptr n)
    {
        return (_Node_ptr &)n->left;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Node_ptr &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_right(_Node_ptr n)
    {
        return (_Node_ptr &)n->right;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Node_ptr &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_parent(_Node_ptr n)
    {
        return (_Node_ptr &)n->parent;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::reference
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_value(_Node_ptr n)
    {
        return n->value;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    const typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::key_type &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_key(_Node_ptr n)
    {
        return _KeyOfValue()(n->value);
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Color_type &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_color(_Node_ptr n)
    {
        return (_Color_type &)n->color;
    }

    /* Base_ptr versions */

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Node_ptr &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_left(_Base_ptr b)
    {
        return (_Node_ptr &)b->left;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Node_ptr &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_right(_Base_ptr b)
    {
        return (_Node_ptr &)b->right;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Node_ptr &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_parent(_Base_ptr b)
    {
        return (_Node_ptr &)b->parent;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::reference
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_value(_Base_ptr b)
    {
        return reinterpret_cast<_Node_ptr>(b)->value;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    const typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::key_type &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_key(_Base_ptr b)
    {
        return _KeyOfValue()(_value(b));
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Color_type &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_color(_Base_ptr b)
    {
        return (_Color_type &)b->color;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    const typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::key_type &
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_key(const value_type &v)
    {
        return _KeyOfValue()(v);
    }

    /* minimum / maximum */

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Node_ptr
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_minimum(_Node_ptr x)
    {
        return (_Node_ptr)_Rb_tree_node_base::_minimum(x);
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Node_ptr
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_maximum(_Node_ptr x)
    {
        return (_Node_ptr)_Rb_tree_node_base::_maximum(x);
    }

    /* =========================== Internal private methods ====================== */

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_empty_initialize()
    {
        this->_header._base_ptr->parent = 0;                      // root initially null
        this->_header._base_ptr->left = this->_header._base_ptr;  // leftmost = header
        this->_header._base_ptr->right = this->_header._base_ptr; // rightmost = header
        this->_header._base_ptr->color = RED;                     // header color = red
        this->_header.count = 0;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Node_ptr
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_create_node(const value_type &v)
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

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Node_ptr
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_clone_node(_Node_ptr orig)
    {
        _Node_ptr n_ptr = _create_node(orig->value);
        n_ptr->color = orig->color;
        n_ptr->left = orig->left;
        n_ptr->right = orig->right;
        n_ptr->parent = orig->parent;
        return n_ptr;
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_destroy_node(_Node_ptr n_ptr)
    {
        this->get_allocator().destroy(&n_ptr->value);
        this->_put_node(n_ptr);
    }

    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Node_ptr
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_copy(_Node_ptr x, _Node_ptr p)
    {
        // x: 원본 서브트리의 노드 커서
        // p: 복제된 서브트리를 붙일 위치 (부모 노드)
        // top: 복제된 서브트리의 루트 노드
        _Node_ptr top = _clone_node(x);
        top->parent = p;

        // 재귀 호출을 줄이기 위해 우측 자식 서브트리는 재귀, 좌측 자식 서브트리는 반복으로 해결
        // 양쪽 다 반복으로 해결하려면 유저가 직접 스택 사용해야 함 (재귀는 콜 스택을 사용)

        // 우측 서브트리 복제 (재귀)
        if (x->right)
            top->right = _copy((_Node_ptr)x->right, top);
        x = (_Node_ptr)x->left;
        p = top;

        // 좌측 서브트리 복제 (반복)
        // x: 원본 트리 커서, y: 복제 트리 커서 (현 시점의 x를 복제)
        // p: 복제 트리 내부 y의 부모노드 (직전 반복의 y)
        while (x != NULL)
        {
            _Node_ptr y = _clone_node(x);
            y->parent = p;
            p->left = y;
            if (x->right)
                y->right = _copy((_Node_ptr)x->right, x);
            p = y;
            x = (_Node_ptr)x->left;
        }
        return top;
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
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_rotate_left(_Base_ptr x)
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
        else if (x == x->parent->left) // x는 부모의 좌측 자식
            x->parent->left = y;
        else
            x->parent->right = y;

        // 3) y의 왼쪽 자식 설정: x
        y->left = x;
        x->parent = y;
    }

    // rotate_right(x): x->left가 위로 올라감
    // rotate_left의 좌우대칭
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_rotate_right(_Base_ptr x)
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
        else if (x == x->parent->right) // x는 부모의 우측 자식
            x->parent->right = y;
        else
            x->parent->left = y;

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
        4. RED 노드의 자식은 BLACK (지켜지지 않을시 Double Red)
        5. 모든 리프 노드에서 Black Depth는 같다
            (Black Depth: 리프 노드에서 루트 노드까지의 경로에서 만나는 BLACK 노드의 개수)
        6. 새로운 노드는 항상 RED
        7. BLACK 노드의 자식은 RED, BLACK 모두 가능
    */

    // insert: 새 노드를 RED로 삽입 -> Double RED 문제 발생
    /*
        - 새로 삽입하는 노드가 RED이므로, 부모가 RED인 경우 문제 발생
        - 삼촌이 Red면 recolor, 삼촌이 Black이면 회전 + Recolor로 해결
    */
    /*
         rotate_right(G) : G를 내려보내고 P를 올리는 변화
         rotate_right(P) : P를 내려보내고 X를 올리는 변화
    */
    // TODO: 비멤버 함수로 빼기
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    void
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_rebalance_for_insert(_Base_ptr  x,
                                                                                 _Base_ptr &root)
    {
        // x는 현재 가리키고 있는 커서
        _Base_ptr p, g, u; // parent, grandparent, uncle

        // Double RED 문제가 해결될 때까지 반복해서 수행
        while (x != root && x->parent->color == RED)
        {
            p = x->parent;
            g = x->parent->parent;
            if (p == g->left) // p가 g의 왼쪽 자식 (u는 g의 오른쪽 자식) (LL, LR)
            {
                u = g->right;
                if (u && u->color == RED) // Case 1: Uncle이 RED
                {
                    // parent와 uncle를 BLACK으로 바꿔주고 grandparent는 RED로 바꿔준다
                    p->color = BLACK;
                    u->color = BLACK;
                    g->color = RED;
                    x = g;
                }
                else // Case 2/3: Uncle이 BLACK (또는 null) -> 회전 필요
                {
                    if (x == p->right) // Case 2: x가 안쪽(inner)에 있음 (LR)
                    {
                        x = p;
                        _rotate_left(x); // Case 3 형태로 바꿈
                        p = x->parent;
                        g = p->parent;
                    }
                    // Case 3: x가 바깥쪽(outer)에 있음 (LL)
                    // recolor + 반대 회전
                    // 부모(BLACK)을 위로 올림
                    // grandparent(RED)는 부모의 우측 자식으로 내림
                    p->color = BLACK;
                    g->color = RED;
                    _rotate_right(g);
                }
            }
            else // p가 g의 오른쪽 자식 (u는 g의 왼쪽 자식) (RL, RR)
            {
                u = g->left;
                if (u && u->color == RED) // Case 1: Uncle이 RED
                {
                    // parent와 uncle를 BLACK으로 바꿔주고 grandparent는 RED로 바꿔준다
                    p->color = BLACK;
                    u->color = BLACK;
                    g->color = RED;
                    x = g;
                }
                else // Case 2/3: Uncle이 BLACK (또는 null) -> 회전 필요
                {
                    if (x == p->left) // Case 2: x가 안쪽(inner)에 있음 (RL)
                    {
                        x = p;
                        _rotate_right(x); // Case 3 형태로 바꿈
                        p = x->parent;
                        g = p->parent;
                    }
                    // Case 3: x가 바깥쪽(outer)에 있음 (RR)
                    // recolor + 반대 회전
                    // 부모(BLACK)을 위로 올림
                    // grandparent(RED)를 부모의 좌측 자식으로 내림
                    p->color = BLACK;
                    g->color = RED;
                    _rotate_left(g);
                }
            }
        }
        root->color = BLACK;
    }

    // erase: 제거한 노드가 BLACK일 경우 -> Black Depth 불균형 문제 발생
    // 실제로 free 해야 할 노드 포인터를 반환

    // ------------------------------------------------------------
    // erase rebalance (SGI/GCC 스타일)
    // return: 실제로 free 해야 할 노드 포인터
    // ------------------------------------------------------------
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    typename ft::_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Base_ptr
    ft::_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_rebalance_for_erase(_Base_ptr z)
    {
        _Base_ptr &root = _root();
        _Base_ptr &leftmost = _leftmost();
        _Base_ptr &rightmost = _rightmost();

        _Base_ptr y = z;
        _Base_ptr x = 0;
        _Base_ptr x_parent = 0;

        // 1) y(실제로 unlink할 노드), x(y의 유일한 자식 또는 0) 결정
        if (y->left == 0)
            x = y->right;
        else if (y->right == 0)
            x = y->left;
        else
        {
            y = y->right;
            while (y->left != 0)
                y = y->left; // successor
            x = y->right;
        }

        // 2) y != z: successor를 z 자리로 올려치우기
        if (y != z)
        {
            // z의 left를 y로
            z->left->parent = y;
            y->left = z->left;

            if (y != z->right)
            {
                x_parent = y->parent;
                if (x)
                    x->parent = y->parent;
                y->parent->left = x; // y는 successor라 parent의 left 자식이었음

                y->right = z->right;
                z->right->parent = y;
            }
            else
                x_parent = y;

            // z의 parent 쪽에서 z 대신 y 연결
            if (root == z)
                root = y;
            else if (z->parent->left == z)
                z->parent->left = y;
            else
                z->parent->right = y;

            y->parent = z->parent;

            // 색 swap 트릭
            _Color_type tmp = y->color;
            y->color = z->color;
            z->color = tmp;

            // 이제 실제로 free할 노드는 z
            y = z;
        }
        // 3) y == z: z를 직접 splice out
        else
        {
            x_parent = y->parent;
            if (x)
                x->parent = y->parent;

            if (root == z)
                root = x;
            else if (z->parent->left == z)
                z->parent->left = x;
            else
                z->parent->right = x;

            // leftmost/rightmost 갱신
            if (leftmost == z)
            {
                if (z->right == 0)
                    leftmost = z->parent; // z가 root였으면 header가 됨
                else
                    leftmost = _Rb_tree_node_base::_minimum(x);
            }
            if (rightmost == z)
            {
                if (z->left == 0)
                    rightmost = z->parent;
                else
                    rightmost = _Rb_tree_node_base::_maximum(x);
            }
        }

        // 4) BLACK이 빠져나갔으면 fix-up
        if (y->color == BLACK)
        {
            while (x != root && (x == 0 || x->color == BLACK))
            {
                if (x == x_parent->left)
                {
                    _Base_ptr w = x_parent->right;

                    if (w && w->color == RED)
                    {
                        w->color = BLACK;
                        x_parent->color = RED;
                        _rotate_left(x_parent);
                        w = x_parent->right;
                    }

                    if (w == 0 || ((w->left == 0 || w->left->color == BLACK) &&
                                   (w->right == 0 || w->right->color == BLACK)))
                    {
                        if (w)
                            w->color = RED;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else
                    {
                        if (w->right == 0 || w->right->color == BLACK)
                        {
                            if (w->left)
                                w->left->color = BLACK;
                            w->color = RED;
                            _rotate_right(w);
                            w = x_parent->right;
                        }
                        w->color = x_parent->color;
                        x_parent->color = BLACK;
                        if (w->right)
                            w->right->color = BLACK;
                        _rotate_left(x_parent);
                        break;
                    }
                }
                else
                {
                    // symmetric
                    _Base_ptr w = x_parent->left;

                    if (w && w->color == RED)
                    {
                        w->color = BLACK;
                        x_parent->color = RED;
                        _rotate_right(x_parent);
                        w = x_parent->left;
                    }

                    if (w == 0 || ((w->right == 0 || w->right->color == BLACK) &&
                                   (w->left == 0 || w->left->color == BLACK)))
                    {
                        if (w)
                            w->color = RED;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else
                    {
                        if (w->left == 0 || w->left->color == BLACK)
                        {
                            if (w->right)
                                w->right->color = BLACK;
                            w->color = RED;
                            _rotate_left(w);
                            w = x_parent->left;
                        }
                        w->color = x_parent->color;
                        x_parent->color = BLACK;
                        if (w->left)
                            w->left->color = BLACK;
                        _rotate_right(x_parent);
                        break;
                    }
                }
            }
            if (x)
                x->color = BLACK;
        }

        return y; // free 해야 할 노드
    }
    /* subtree erase */
    // 부모 노드를 맨 마지막에 지우는 것이 안전
    // 즉 post-order (L-R-Root)
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc>
    void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_erase_subtree(_Base_ptr x)
    {
        if (!x)
            return;
        _erase_subtree(x->left);
        _erase_subtree(x->right);
        _destroy_node(static_cast<_Node_ptr>(x));
    }
} // namespace ft

#endif
