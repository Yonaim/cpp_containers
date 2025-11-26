#ifndef FT_RB_TREE_ITERATOR_H
#define FT_RB_TREE_ITERATOR_H

#include "rb_tree_node.h"
#include "iterator_tags.h"

namespace ft
{
    // ================= iterator =================

    /*
        - STL Bidirectional Iterator의 규칙을 만족
        - ++, --, *, ->, ==, !=
        - 중위 순회 알고리즘 (inorder traversal)
        - dereference (*, ->) 연산 때문에 타입 T를 알아야 함
            => 템플릿 사용
    */

    // base iterator가 따로 존재하는 이유: iterator의 동작 자체는 value_type과 무관하기 때문
    struct _Rb_tree_base_iterator
    {
        typedef _Rb_tree_node_base::_Base_ptr _Base_ptr; // 내부 alias 존중
        typedef bidirectional_iterator_tag    iterator_category;
        typedef ptrdiff_t                     difference_type;

        // 순회 상태 저장
        _Base_ptr _base_node;

        void _increment();
        void _decrement();
    };

    // iterator는 class로 구현하는 것이 일반적 (private가 필요한 경우가 있음 & 일관적으로)
    // 구현체 따라 다르긴 함. GNU는 struct로 구현했으며 이 구현체에서도 이를 따름
    /*
        템플릿 인자가 3개나 있는 이유:
        iterator, const_iterator 두 타입을 하나의 템플릿에서 생성하기 위해
    */
    template <class _Value, class _Ref, class _Ptr>
    struct _Rb_tree_iterator : public _Rb_tree_base_iterator
    {
        // ================== typedef ==================
        typedef _Value value_type;
        typedef _Ref   reference;
        typedef _Ptr   pointer;
        // itertor
        // _Value가 적절하지 않은 타입일 경우 typedef 처리 중 컴파일 에러
        typedef _Rb_tree_iterator<_Value, _Value &, _Value *> iterator;
        /*
            - 이미 const인 타입이 인자로 들어오는 경우, const가 2번 붙지만 하나의 const로
               정규화되므로 정상 처리됨
            - cv-qualification(const/volatile)은 중복되면 자동으로 하나만 있는 것으로 처리
            - 이미 const인 타입이 인자로 들어온다면 iterator와 const_iterator는 똑같다
        */
        typedef _Rb_tree_iterator<_Value, const _Value &, const _Value *> const_iterator;
        typedef _Rb_tree_node<_Value>                                    *_Node_ptr;

        // 현재의 타입이 iterator인지 const_iterator인지 상관하지 않고 동일한 코드로 처리하기
        // 위해 define
        typedef _Rb_tree_iterator<_Value, _Ref, _Ptr> _Self;

        // ================== allocation ==================

        _Rb_tree_iterator() {}
        // POD라서 생성자 호출이 필요없으므로 초기화 리스트를 사용하지 않아도 무빙
        _Rb_tree_iterator(_Node_ptr n) { _base_node = n; }
        _Rb_tree_iterator(const iterator &it) { _base_node = it._base_node; }

        // ================== operators ==================

        // * : 참조 반환
        // -> : 포인터 반환 (체이닝 o)
        /*
            it->value를 해석하면,
            it.operator->()->value가 됨

            1) iterator의 연산자 오버로딩 ->에 의해 포인터가 반환
            2) pointer의 연산자(기본) ->에 의해 value에 접근

            포인터를 반환하는 이유: 기본 포인터의 역할(체이닝 + 접근 가능)을 그대로 수행하기
           위함
        */
        reference operator*() const { return _Node_ptr(_base_node)->value; }
        pointer   operator->() const { return &(operator*()); }

        // 순회 이동 연산 구현
        _Rb_tree_iterator &operator++()
        {
            _increment();
            return *this;
        }
        _Rb_tree_iterator operator++(int)
        {
            _Rb_tree_iterator tmp = *this;
            _increment();
            return tmp;
        }
        _Rb_tree_iterator &operator--()
        {
            _decrement();
            return *this;
        }
        _Rb_tree_iterator operator--(int)
        {
            _Rb_tree_iterator tmp = *this;
            _decrement();
            return tmp;
        }

        /*
            비교 연산자는 외부에 정의한다.
            1) iterator와 const_iterator(템플릿 인자가 다른 경우)의 비교를 지원
            2) 양방향 비교의 허용 (코드 중복을 줄인다.)
        */
    };

    // 템플릿 인자가 다름 -> 완전히 다른 타입으로 취급
    // 따라서 별도의 operator 정의가 필요하다.
    /*
        1) iterator == iterator
        2) const_iterator == const_iterator
        3) const_iterator == iterator
        4) iterator == const_iterator
    */

    // 1), 2) 둘다 커버하기 위해 템플릿 인자를 3개 사용
    template <class _Value, class _Ref, class _Ptr>
    bool operator==(const _Rb_tree_iterator<_Value, _Ref, _Ptr> &lhs,
                    const _Rb_tree_iterator<_Value, _Ref, _Ptr> &rhs)
    {
        return lhs._base_node == rhs._base_node;
    }

    // 3)
    template <class _Value>
    bool operator==(const _Rb_tree_iterator<_Value, const _Value &, const _Value *> &lhs,
                    const _Rb_tree_iterator<_Value, _Value &, _Value *>             &rhs)
    {
        return lhs._base_node == rhs._base_node;
    }

    // 4)
    template <class _Value>
    bool operator==(const _Rb_tree_iterator<_Value, _Value &, _Value *>             &lhs,
                    const _Rb_tree_iterator<_Value, const _Value &, const _Value *> &rhs)
    {
        return lhs._base_node == rhs._base_node;
    }

} // namespace ft

#endif
