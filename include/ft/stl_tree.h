// https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_tree.h#L247

#ifndef FT_STL_TREE_H
#define FT_STL_TREE_H

// #include <memory> // TODO: ft::allocator로 교체
#include "allocator.h"

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
    // ================= node base =================

    enum _Rb_tree_color
    {
        RED = false,
        BLACK = true
    };

    struct _Rb_tree_node_base
    {
        typedef _Rb_tree_node_base *_base_ptr;
        _base_ptr                   parent;
        _base_ptr                   left;
        _base_ptr                   right;
        _Rb_tree_color              color; // Red & Black
    };

    // ================= node<Value> =================

    template <class Value>
    struct _Rb_tree_node : public _Rb_tree_node_base
    {
        Value value;
    };

    // ================= iterator =================

    /*
        - STL Bidirectional Iterator의 규칙을 만족
        - ++, --, *, ->, ==, !=
        - 중위 순회 알고리즘
        - dereference (*, ->) 연산 때문에 타입 T를 알아야 함
            => 템플릿 사용
    */

    // iterator는 class로 구현하는 것이 일반적 (private가 필요한 경우가 있음 & 일관적으로)
    // 구현체 따라 다르긴 함. GNU는 struct로 구현했으며 이 구현체에서도 이를 따름
    template <class _Tp>
    struct _Rb_tree_iterator
    {
        typedef _Tp  value_type;
        typedef _Tp &reference;
        typedef _Tp *pointer;

        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t                  difference_type;

        typedef _Rb_tree_node_base::_base_ptr _base_ptr; // 내부 alias 존중
        typedef _Rb_tree_node<_Tp>           *_node_ptr;

        // 순회 상태 저장
        _node_ptr _node;

        // ================== operators ==================
        // TODO
        _Rb_tree_iterator &operator++() {};
        _Rb_tree_iterator  operator++(int) {};
        _Rb_tree_iterator &operator--() {};
        _Rb_tree_iterator  operator--(int) {};

        // * : 참조 반환
        // -> : 포인터 반환 (체이닝 o)
        /*
            it->value를 해석하면,
            it.operator->()->value가 됨

            1) iterator의 연산자 오버로딩 ->에 의해 포인터가 반환
            2) pointer의 연산자(기본) ->에 의해 value에 접근

            포인터를 반환하는 이유: 기본 포인터의 역할(체이닝 + 접근 가능)을 그대로 수행하기 위함
        */
        // iterator 값
        reference operator*() {};
        pointer   operator->() {};

        bool operator==(const _Rb_tree_iterator &other) {};
        bool operator!=(const _Rb_tree_iterator &other) {};
    };

    /*
    reverse_iterator와 달리 const_iterator 타입을 따로 선언해야하는 이유:

    1) 상속한 클래스에서 type alias를 overriding 할 수 없다.

    2) value_type(=iterator가 가리키는 값 타입)이 다르다
        - reverse_iterator는 애초에 iterator의 wrapper라서 Iter가 Template 인자
            -> 걍 Iter를 바꿔끼면 됨
        - 반면 iterator는 iterator가 가리키는 value_type 자체가 다름 (const 한정자가 붙는 것)
        - 따라서 const_iterator가 별도로 필요하다.

        (표준) 일반 iterator와 const iterator 타입을 별도로 선언, 코드 중복 발생
        (대안) 템플릿 파라미터가 const 유무 표기하는 인자를 두기
    */
    /*
        '_Tp' 타입 자체에 const를 걸지 않는다.
        1) 원칙적으로 const iterator는 '읽기 전용' 이터레이터라서,
            iterator<T>와 const_iterator<T>의 T는 같아야 의미가 있음
        2) map의 경우 value가 pair<const Key, T>인데 const가 이중으로 걸려버림
    */
    template <class _Tp>
    struct _Rb_tree_const_iterator
    {
        typedef const _Tp  value_type;
        typedef const _Tp &reference;
        typedef const _Tp *pointer;

        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t                  difference_type;

        typedef _Rb_tree_node_base::_base_ptr _base_ptr; // 내부 alias 존중
        typedef const _Rb_tree_node<_Tp>     *_node_ptr;

        // 순회 상태 저장
        _node_ptr _node;
    };

    // ==================== Rb_tree ====================

    // Helper type to manage default initialization of node count and header.
    struct _Rb_tree_header
    {
        typedef _Rb_tree_node_base *_base_ptr;
        _base_ptr                   root;
        size_t                      count;
    };

    // _KeyOfValue: value에서 key를 뽑는 정책 (함수 객체)
    // KeyOfValue 덕분에 associative array까지 커버하는 '범용 트리 엔진'
    template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
              typename _Alloc = allocator<_Val>>
    struct _Rb_tree
    {
        typedef _Rb_tree_iterator       iterator;
        typedef _Rb_tree_const_iterator const_iterator;
        // TODO
    };

} // namespace ft

#endif
