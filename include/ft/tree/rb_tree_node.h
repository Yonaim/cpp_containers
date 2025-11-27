#ifndef FT_RB_TREE_NODE_H
#define FT_RB_TREE_NODE_H

#include <cstddef>

namespace ft
{
    // ================= node base =================

    enum _Rb_tree_color
    {
        RED = false,
        BLACK = true
    };

    // value 미포함
    // 일반 노드 클래스(_Rb_tree_node)는 이 base를 상속받음
    // header 노드의 경우 value 미포함이므로 이 base를 상속받는다
    struct _Rb_tree_node_base
    {
        typedef _Rb_tree_node_base *_Base_ptr;
        typedef _Rb_tree_color      _Color_type;
        _Color_type                 color; // Red & Black
        _Base_ptr                   parent;
        _Base_ptr                   left;
        _Base_ptr                   right;

        /*
            최소/최대 노드를 찾는 연산:
            - value_type가 관계가 없다
            - 트리 구조와 상관 없는 포인터 기반의 연산

            base node 구조체에 연산을 정의하는 이유???
            -> value를 가지지 않는 노드(header)도 존재하므로, base 레벨에서 연산을 정의해야한다.
        */
        static _Base_ptr _minumum(_Base_ptr ptr)
        {
            while (ptr->left != 0)
                ptr = ptr->left;
            return ptr;
        }

        static _Base_ptr _maximum(_Base_ptr ptr)
        {
            while (ptr->right != 0)
                ptr = ptr->right;
            return ptr;
        }
    };

    // ================= node<Value> =================

    // value를 포함한 일반 노드 타입
    template <class Value>
    struct _Rb_tree_node : public _Rb_tree_node_base
    {
        Value value;
    };

} // namespace ft

#endif
