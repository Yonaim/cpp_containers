#ifndef FT_ALLOCATOR_H
#define FT_ALLOCATOR_H

namespace ft
{
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

        // 기존 allocator의 정책을 유지한 채 template argument의 타입을 변경
        // std보다는 커스텀 allocator일때 유용
        // ex. typedef yona::allocator<orig>::rebind<converted>::other convertedAllocator;
        template <class U>
        struct rebind
        {
            typedef allocator<U> other;
        };

        // -------------------- constructors -------------------- //
        allocator() throw();
        allocator(const allocator &other) throw();
        template <class U>
        allocator(const allocator<U> &other) throw();
        ~allocator();


        // ---------------- other member functions ---------------- //

        // operator&가 오버로딩 되었을 경우를 대비한 우회용 함수
        pointer       address(reference x) const;
        const_pointer address(const_reference x) const;

        // memory
        pointer   allocate(size_type n, const void *hint = 0);
        void      deallocate(T *p, std::size_t n);
        size_type max_size() const throw();
        void      construct(pointer p, const_reference val);
        void      destroy(pointer p);
    };

    // default allocator는 stateless하므로 비교 연산 반환값은 고정
    template <class T1, class T2>
    bool operator==(const allocator<T1> &lhs, const allocator<T2> &rhs) throw();
    template <class T1, class T2>
    bool operator!=(const allocator<T1> &lhs, const allocator<T2> &rhs) throw();
} // namespace ft

#endif
