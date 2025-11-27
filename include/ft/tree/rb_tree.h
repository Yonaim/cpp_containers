#ifndef FT_RB_TREE_H
#define FT_RB_TREE_H

// #include <memory> // TODO: ft::allocator로 교체
#include "pair.h"
#include "allocator.h"
#include "distance.h"
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
    : 빈 클래스(empty class)를 상속받을 때, 그 클래스가 차지하는 메모리를 0바이트로 최적화.

    원래는, 빈 클래스라도 기본적으로 1바이트가 필요함!!!
    객체는 고유한 주소를 가져야하기 때문이다.

    그런데 '상속' 구조에서는 이를 최적화할 수 있다. 상속 받는 클래스가 어차피 메모리를 차지하고 있기
   때문에 고유한 주소를 이미 갖고 있음.
    -> empty class에 대해 메모리를 할당할 필요가 x.
*/

#include <cstddef>
#include <type_traits>

namespace ft
{
    template <class T>
    struct is_instanceless
    {
        // 객체 생성 없이 바로 타입 정보를 캐낼 수 있도록 static 멤버
        // 어차피 판별하고 싶은 타입의 정보가 템플릿 인자임
        static const bool value = std::is_empty<T>::value;
        // std::is_empty는 컴파일 타임에서 타입 정보를 알기 위한 type traits
        // type traits의 결과는 value에 담기는 것이 컨벤션
    };

    // =============================== Rb_tree_header =================================

    // Helper type to manage default initialization of node count and header.
    struct _Rb_tree_header
    {
        /*
            왜 base 객체를 그대로 멤버로 가지지 않고 포인터를 멤버로 가지는걸까?
            ->

        */
        typedef _Rb_tree_node_base *_Base_ptr;
        _Base_ptr                   _base_ptr;

        /*
            - header는 노드가 아님 !!!! 트리의 데이터를 보관하는 용.
                - header->parent: 트리의 root
                - header->left: leftmost (begin())
                - header->right: rightmost (end())
            - leftmost, rightmost
                - 없어도 기능상으로는 문제가 없는데, 자주 쓰므로 저장해둠
                - begin(), end() 즉 iterator로 사용되기 때문에, 절대 NULL이면 안됨!!!
                - 트리가 비워져 있는 상태일 때, leftmost == rightmost 보장
                - NULL이어선 안됨 & 트리가 empty 상태일 때 같은 값인 것을 보장해야한다
                    -> header의 주소를 leftmost, rightmost에 넣어 초기화
        */
        size_t count;

        /*
            - 트리는 여러 번 리셋될 수 있는 구조이므로 생성자, 소멸자에서 작업해도 의미가 없음
            - 그런 상황에서 헤더 노드를 재생성 / 삭제하지 않고 기존 헤더를 재활용하기 때문
        */
    };

    // =============================== Rb_tree_alloc_base =================================

    // _Alloc = allocator<value_type>
    /*
        표준 요구사항:
            모든 컨테이너는 allocator_object 를 이용하여 value_type 에 대한 메모리를 할당하고
            필요하다면 rebind을 통해 다른 타입을 위한 allocator를 얻을 수 있어야 한다.
    */
    template <class _Tp, class _Alloc, bool _instanceless>
    class _Rb_tree_alloc_base;

    /*
        - instanceless: '이 인스턴스를 보관할 필요가 있는가'
        - stateless한 allocator라면 굳이 멤버로 들고 있을 필요 없음!!
            => 메모리를 아끼기 위해 specialization하자.
        - 개념상으론, stateless != instanceless
            - 근데 state가 없으면 보통 instance를 들고 있을 필요가 없곤하다.
                (이 경우도 해당)
    */

    // specialization for instance-required
    template <class _Tp, class _Alloc>
    class _Rb_tree_alloc_base<_Tp, _Alloc, false>
    {
      public:
        // Alloc은 이미 value_type에 대한 allocator이므로 allocator_type은 value_type에 관함
        typedef _Alloc allocator_type;
        typedef _Tp    value_type;

        /*
            rebind는 내부 템플릿 구조체(nested class template) 멤버
            템플릿 멤버인 점을 명시하기 위해 template 키워드 사용
        */
        // <_Rb_tree_node<value_type> > : 말 그대로 value_type을 T로 삼는 노드 타입
        typedef
            typename _Alloc::template rebind<_Rb_tree_node<value_type>>::other node_allocator_type;
        typedef _Rb_tree_node<value_type>                                      node_type;

        /*
            - 표준에 명시된 컨테이너의 요구사항에 의해,
                모든 컨테이너는 get_allocator() 함수를 가져야하며 반환형은 컨테이너의 value_type에
           대한 allocator여야함.
            - 이때 allocator의 내부 상태를 보존한 복사본을 반환해야한다.
            - 복사생성자 사용하여 반환하는 이유:
                - 내부 allocator는 node 기반임
                - 따라서 value_type으로 변환하기 위해 rebind 필요
        */
        // 컨테이너 요구사항에 포함된 공식 API이므로 언더바를 붙이지 않는다
        allocator_type get_allocator() const { return allocator_type(_node_allocator); }

        // 생성자: 외부에서 받은 allocator로 초기화
        // node_allocator_type(a)꼴로 생성자 호출
        // _header(): 명시적으로 기본 생성자 호출 (성능에는 영향x)
        _Rb_tree_alloc_base(const allocator_type &a) : _node_allocator(a) {}

      protected:
        node_allocator_type _node_allocator;

        /*
            - raw memory 확보 / 해제
                - get_node(): allocate
                - put_node(): deallocate (put back: 돌려놓는다)
            - value 객체 생성자 호출 / 소멸자 호출
                - construct()
                - destroy()
        */
        /*
            raw memory와 value 객체 생성, 파괴를 분리해야하는 이유?
            -> node와 value는 별개이다.
            node 자체는 그대로 냅두고 value만 건드는 경우도 있음 (node 재활용)
        */
        node_type *_get_node() { return _node_allocator.allocate(1); }
        void       _put_node(node_type *p) { return _node_allocator.deallocate(p, 1); }
    };

    // specialization for instanceless
    template <class _Tp, class _Alloc>
    class _Rb_tree_alloc_base<_Tp, _Alloc, true>
    {
      public:
        typedef _Alloc allocator_type;
        typedef _Tp    value_type;

        typedef
            typename _Alloc::template rebind<_Rb_tree_node<value_type>>::other node_allocator_type;
        typedef _Rb_tree_node<value_type>                                      node_type;

        allocator_type get_allocator() const { return allocator_type(); }

        _Rb_tree_alloc_base(const allocator_type &a) {}

      protected:
        // node_allocator_type은 empty class (instanceless한 Alloator이므로)
        /*
          c++은 empty class에 대해 '아무 일도 하지 않음'을 보장한다.
              - sizeof(empty class) = 1 (만약 EBO 적용시, 0바이트)
              - 생성/복사/파괴 비용은 0
          컴파일 언어이기 때문에 가능한 최적화.
          따라서 아래와 같이 코드를 작성해도 node_allocator_type을 매번 할당하는 일은 x.
        */
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

    // private 상속: 상속 받은 멤버들을 모두 private 멤버로 둠
    // 굳이 외부에 공개할 필요가 없으므로 private 상속 받음
    template <class _Tp, class _Alloc>
    class _Rb_tree_base : private _Rb_tree_alloc_base<_Tp, _Alloc, is_instanceless<_Alloc>::value>
    {
        typedef _Rb_tree_alloc_base<_Tp, _Alloc, is_instanceless<_Alloc>::value> Base;

        // 얘도 base 클래스라서 상속 받게할거임
      protected:
        typedef typename Base::node_type node_type;

        _Rb_tree_header _header;

        // alloc_base의 get, put 함수를 멤버 재노출 (현재 클래스의 scope로 끌어옴)
        using Base::_get_node;
        using Base::_put_node;
        using Base::get_allocator;

      public:
        // Base(alloc): 상속 받은 클래스 타입의 생성자 호출
        _Rb_tree_base(const _Alloc &alloc = _Alloc()) : Base(alloc), _header() {}
        ~_Rb_tree_base() {}
    };

    // =============================== Rb_tree =================================

    // _KeyOfValue: value에서 key를 뽑는 정책 (함수 객체, functor)
    // KeyOfValue 덕분에 associative array까지 커버하는 '범용 트리 엔진'
    // value = pair<key, mapped>임. mapped_type과는 다르니 헷갈리지 말 것!
    // _Compare: Funcion Object
    template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare,
              typename _Alloc = allocator<_Value>>
    class _Rb_tree : protected _Rb_tree_base<_Value, _Alloc>
    {
        typedef _Rb_tree_base<_Value, _Alloc> _Base;

      public:
        //==========================================================================
        // Public API
        //==========================================================================
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

        // iterator
        typedef _Rb_tree_iterator<_Value, _Value &, _Value *>             iterator;
        typedef _Rb_tree_iterator<_Value, const _Value &, const _Value *> const_iterator;
        typedef reverse_iterator<iterator>                                reverse_iterator;
        typedef reverse_iterator<const_iterator>                          const_reverse_iterator;

        // ================================ API ================================
        // allocation/deallocation
        _Rb_tree() : _Rb_tree(const _Compare &, const allocator_type &);
        ~_Rb_tree() { clear(); };

        void clear()
        {
            if (_header->count == 0)
                return;
            _erase_subtree(_root());
            _leftmost() = &_header;
        }

        // iterators
        iterator               begin() { return _leftmost(); }
        const_iterator         begin() const { return _leftmost(); }
        iterator               end() { return _rightmost(); }
        const_iterator         end() const { return _rightmost(); }
        reverse_iterator       rbegin() { return _leftmost(); }
        const_reverse_iterator rbegin() const { return _leftmost(); }
        reverse_iterator       rend() { return _rightmost(); }
        const_reverse_iterator rend() const { return _rightmost(); }
        bool                   empty() const { return _header.count == 0; }
        size_type              size() const { return _header.count; }

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

        // ================================ insert / erase API ================================

        // insert
        pair<iterator, bool> insert_unique(const value_type &v);
        iterator             insert_equal(const value_type &v);
        iterator             insert_unique(iterator position, const value_type &v);
        iterator             insert_equal(iterator position, const value_type &v);

        // erase: 원소 1개 제거
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

      protected:
        //==========================================================================
        // Internal helpers
        //==========================================================================
        // _Compare: stateful 함수 객체이므로 멤버로 둠
        _Compare _key_compare;

        // root, leftmost, rightmost 접근 함수
        _Node_ptr &_root() const { return (_Node_ptr &)_header._base_ptr->_parent; }
        _Node_ptr &_leftmost() const { return (_Node_ptr &)_header._base_ptr->_left; }
        _Node_ptr &_rightmost() const { return (_Node_ptr &)_header._base_ptr->_right; }

        /*
            [반환형을 _Node_ptr로 통일하는 이유]
            - 트리 조작 로직에서 내부적으로 _Node_ptr, _Base_ptr 둘다 씀
                - _Base_ptr: 트리 구조 조작용
                - _Node_ptr: value 접근용
                - _Node_ptr의 기능이 더 강력함
            - getter에 의해 반환된 값은 여러 목적으로 쓰일 수 있음
                -> 좀더 강력한 _Node_ptr 타입으로 반환한다!! (반환형 통일)
                - 위험성? -> header 노드가 value 관련 작업을 시도하지 않음을 로직상 보장하면 OK
            - header 노드를 감안하여 _Base_ptr로 반환할 수도 있으면 좋았겠지만...
                c++에서의 함수 오버로딩은 파라미터에 의해서만 가능하다.
        */

        /*
            [왜 _Node_ptr 버전 / _Base_ptr 버전을 따로 두는가?]

            - Base가 Node의 베이스 클래스이므로 사실 Base 버전만 있어도 기능상으로는 문제가 없다.
                - 그러나, 업캐스팅 과정에서 실제 타입 정보가 사라짐.
                - reinterpret_cast를 사용하면 문법적으로는 문제가 없으나,
                    공격적인 최적화가 어려워짐 (최적화 비용 증가)

            - 따라서 libstdc++는 역할을 분리하여
                _S_left(_Node_ptr) : 캐스팅 없음, 최적화 최고
                _S_left(_Base_ptr) : sentinel/header 등 Base-only 노드용
            두 버전으로 설계했다.

            - 파라미터의 implicit conversion 과정에서는 비용이 0
        */

        /*
            [reinterpret_cast의 연산 비용]
            - 실행 시 비용(즉 CPU가 명령을 수행하는 비용) 자체는 0이다.
                - CPU 입장에서는 할 일이 없음. 컴파일러 레벨에서 끝남!
            - 근데 타입이 확실하지 않으므로, 컴파일러의 공격적인(aggressive) 최적화가 어려워진다.
                즉 최적화 측면에서 비용이 발생한다고 할 수 있음.
        */

        // static helper 함수들
        static _Node_ptr      &_left(_Node_ptr n) { return (_Node_ptr &)(n->left); }
        static _Node_ptr      &_right(_Node_ptr n) { return (_Node_ptr &)(n->right); }
        static _Node_ptr      &_parent(_Node_ptr n) { return (_Node_ptr &)(n->parent); }
        static reference       _value(_Node_ptr n) { return n->value; }
        static const key_type &_key(_Node_ptr n)
        {
            // _KeyOfValue 타입의 객체(functor) 생성 후 operator() 호출
            return _KeyOfValue()(n->value);
        }
        static _Color_type &_color(_Node_ptr n) { return (_Color_type &)n->color; }

        static _Node_ptr      &_left(_Base_ptr b) { return (_Node_ptr &)(b->left); }
        static _Node_ptr      &_right(_Base_ptr b) { return (_Node_ptr &)(b->right); }
        static _Node_ptr      &_parent(_Base_ptr b) { return (_Node_ptr &)(b->parent); }
        static reference       _value(_Base_ptr b) { return reinterpret_cast<_Node_ptr>(b)->value; }
        static const key_type &_key(_Base_ptr b) { return _KeyOfValue()(_value(b)); }
        static _Color_type    &_color(_Base_ptr b) { return (_Color_type &)b->color; }

      private:
        // node create & destroy
        _Node_ptr _create_node(const value_type &v)
        {
            _Node_ptr n_ptr = _get_node();
            try
            {
                // &(n_ptr->value) = &n_ptr->value
                get_allocator().construct(&n_ptr->value, v);
            }
            catch (...)
            {
                _put_node(n_ptr);
            }
            return n_ptr;
        }

        // 주의: parent는 복사하지 않는다
        _Node_ptr _clone_node(_Node_ptr orig)
        {
            _Node_ptr n_ptr = _create_node(orig->value);
            n_ptr->color = orig->color;
            n_ptr->left = orig->left;
            n_ptr->right = orig->right;
            // n_ptr->parent = orig->parent;
            return n_ptr;
        }

        void _destroy_node(_Node_ptr n_ptr)
        {
            get_allocator().destroy(n_ptr);
            _put_node(n_ptr);
        }

        // balancing (fixup)
        void _insert_fixup(_Base_ptr x);
        void _erase_fixup(_Base_ptr x, _Base_ptr x_parent);

        // find min / max node (stateless)
        static _Node_ptr _minimum(_Node_ptr x)
        {
            return (_Node_ptr)_Rb_tree_node_base::_minumum(x);
        }
        static _Node_ptr _maximum(_Node_ptr x)
        {
            return (_Node_ptr)_Rb_tree_node_base::_maximum(x);
        }
    };

} // namespace ft

#endif
