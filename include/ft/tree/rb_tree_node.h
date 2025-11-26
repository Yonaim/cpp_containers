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

    struct _Rb_tree_node_base
    {
        typedef _Rb_tree_node_base *_Base_ptr;
        typedef _Rb_tree_color      _Color_type;
        _Color_type                 color; // Red & Black
        _Base_ptr                   parent;
        _Base_ptr                   left;
        _Base_ptr                   right;

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

    template <class Value>
    struct _Rb_tree_node : public _Rb_tree_node_base
    {
        Value value;
    };

} // namespace ft

#endif
