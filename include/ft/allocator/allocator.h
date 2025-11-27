#ifndef FT_ALLOCATOR_H
#define FT_ALLOCATOR_H

#include <cstddef>
#include <limits>

/*
    The std::allocator class template is the default Allocator used by all standard library containers
    if no user-specified allocator is provided.
*/

/*
    - custum allocator는 std::allocator를 상속해서 선언하는게 아님
    - 필수 allocator API만 갖추면 된다
        - typedef
        - 생성자
        - allocate / deallocate
        - construct / destroy
        - rebind
*/

/*
    allocator가 static이 아닌 이유:
        - std의 경우에는 stateless지만 모든 allocator가 그런 것은 아님
        - 같은 템플릿 인자 타입의 allocator라도 각 instance에서 policy가 다를 수 있는,
            policy object (정책 객체)이므로 호출하는 쪽에서는 무조건
            '객체(인스턴스)를 통한 호출'을 해야한다.
*/

namespace ft
{
    /*
    std::allocator
        - 단순히 ::operator new/delete의 wrapper
        - stateless함
        - 실제 메모리 관리 책임은 OS에 있음
        - memory management & object lifetime
    */
    template <class T>
    struct allocator
    {
        typedef T              value_type;
        typedef T             *pointer;
        typedef const T       *const_pointer;
        typedef T             &reference;
        typedef const T       &const_reference;
        typedef std::size_t    size_type;
        typedef std::ptrdiff_t difference_type;

        /*
            rebind
            - 기존 allocator의 정책을 유지한 채 template argument의 타입을 변경
                - policy: 메모리 할당 위치, alignment, stateful 유무 등
            - 'std::allocator'는, policy가 고정되어 있으므로 크게 의미는 없다.
                rebind는 custum allocator에서 의미를 가짐
            - ex. typedef yona::allocator<orig>::rebind<converted>::other convertedAllocator;
        */
        template <class U>
        struct rebind
        {
            typedef allocator<U> other;
        };
        // 단순히 타입 alias용 구조체 -> empty class
        // 객체를 생성한다고 해도 메모리에 실체하지 않음
        // 단지 컴파일 타임시 AST, 타입 테이블에 반영

        // -------------------- constructors --------------------

        // default allocator는 stateless하므로 아무런 동작을 하지 않는다.
        allocator() throw() {}

        allocator(const allocator &other) throw() {}

        template <class U>
        allocator(const allocator<U> &other) throw()
        {
        }

        ~allocator() {}

        // ---------------- other member functions ----------------

        // operator&가 오버로딩 되었을 경우를 대비한 우회용 함수
        // built-in 타입은 operator 오버로딩이 불가한 점을 이용,
        // char의 &연산자 결과를 이용하여 리턴한다
        pointer address(reference x) const
        {
            // volatile은 최적화에 의한 생략 방지
            // reinterpret_cast: raw 메모리 그대로 해석만 바꾼다
            // const_cast: const/volatile 제거
            const volatile char &x_char = reinterpret_cast<const volatile char &>(x);
            return reinterpret_cast<T *>(const_cast<char *>(&x_char));
        }
        const_pointer address(const_reference x) const
        {
            const volatile char &x_char = reinterpret_cast<const volatile char &>(x);
            return reinterpret_cast<const T *>(&x_char);
        }

        // memory
        pointer allocate(size_type n, const void *hint = 0)
        {
            return ::operator new(sizeof(T) * n);
        }
        // ::operator delete 자체가 사이즈 정보가 필요 없으므로 std::allocator에서는 n이 쓰이지 않음
        // 커스텀 allocator에 n이 쓰일 수 있기 때문에 명세상으로 파라미터 n이 존재
        void deallocate(T *p, std::size_t n)
        {
            // destructor는 deallocate 호출 이전 사용자가 명시적으로 호출
            ::operator delete(p);
        }
        size_type max_size() const throw()
        {
            return std::numeric_limits<size_type>::max() / sizeof(T);
        }
        void construct(pointer p, const_reference val)
        {
            // 'placement new'
            // new ptr typename(value)
            // 대표 사용 용례: new int(42);
            // 중간에 포인터(ptr) 삽입하여 특정 메모리 위치에 객체를 생성 가능
            // ::operator new와 다름
            // 내부적으로 할당 + 객체 생성자 호출 두 과정을 포함
            new ((void *)p) T(val);
            // p 위치에 T(val)을 넣는다
        }
        void destroy(pointer p) { p->~T(); }
    };

    // default allocator는 stateless하므로 비교 연산 반환값은 고정
    template <class T1, class T2>
    bool operator==(const allocator<T1> &lhs, const allocator<T2> &rhs) throw()
    {
        return true;
    }
    template <class T1, class T2>
    bool operator!=(const allocator<T1> &lhs, const allocator<T2> &rhs) throw()
    {
        return false;
    }

} // namespace ft

#endif
