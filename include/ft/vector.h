#ifndef FT_VECTOR_H
#define FT_VECTOR_H

#include <memory> // TODO: ft::allocator로 교체
// TODO: bool 타입에 대한 partial specialization 추가

/*
Except for the std::vector<bool> partial specialization, the elements are stored contiguously, which
means that elements can be accessed not only through iterators, but also using offsets to regular
pointers to elements.
*/

/*
function types:
- constructor
- destructor
- element access
- iterators
- capacity
- modifiers
- operators (non-member)
*/
namespace ft
{
    /*
    - contiguous memory 사용
    - 원소의 순서를 항상 보장 (order guarentee)
    - 중간 삽입/삭제 시 삭제 위치 뒤쪽의 모든 원소 재배치 -> O(n)
    - 맨 뒤 삽입/삭제 시 평균적으로 O(1)
    - 메모리 공간 부족시 공간 전체 새로 할당 후 다시 대입
    */

    // T: The type of the elements (must copy-assignable & copy-constructible)
    // allocator: customizable (default one is std)
    template <class T, class Alloc = std::allocator<T>>
    class vector
    {
      public:
        typedef T                                    value_type;
        typedef Alloc                                allocator_type;
        typedef std::size_t                          size_type;
        typedef std::ptrdiff_t                       difference_type;
        typedef typename Alloc::reference            reference;
        typedef typename Alloc::const_reference      const_reference;
        typedef typename Alloc::pointer              pointer;
        typedef typename Alloc::const_pointer        const_pointer;
        typedef pointer                              iterator;
        typedef const_pointer                        const_iterator;
        typedef ft::reverse_iterator<iterator>       reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

        // delegating constructor 사용 (C++11)
        // delegating constructor: 자신의 다른 생성자를 초기화 리스트에서 호출할 수 있다.
        // vector() : vector(Allocator()) {}

        // Default constructor
        explicit vector(const Allocator &alloc = Allocator())
            : _allocator(alloc), data(NULL), size(0), capacity(0)
        {
        }

        // Fill constructors: Default-inserted elements (C++98에는 없었다)
        // T는 기본 생성자를 통해 자동 생성
        explicit vector(size_type count, const Alloc &alloc = Allocator())
            : _allocator(alloc), data(allocator.allocate(count)), size(count), capacity(count)
        {
            for (size_t i = 0; i < count; ++i)
            {
                _allocator.construct(data + i, 0);
            }
        }

        // Fill constructors: Fill constructor with specified value
        explicit vector(size_type count, const T &value = T(), const Allocator &alloc = Allocator())
            : _allocator(alloc), data(allocator.allocate(count)), size(count), capacity(count)

        {
            for (size_t i = 0; i < count; ++i)
            {
                _allocator.construct(data + i, value);
            }
        }

        /*
        C++98 시절에는 Default-inserted elements가 없었기 때문에, count를 지정하면서 Allocator를
        커스텀하고 싶다면 두 번째 인자에 직접 기본 생성자를 포함해야했다.
        ex) vector(42, MyType(), MyAlloc<MyType>());
        */

        // Range constructor: iterator-based
        // 범위 유효성은 검사하지 않는다 -> UB로 취급
        template <class InputIt>
        vector(InputIt first, InputIt last, const Allocator &alloc = Allocator())
            : _allocator(alloc), data(NULL), size(0), capacity(0)
        {
            size_type n = 0;
            for (InputIt it = first; it != last; ++it)
                ++n;

            // RandomAccessIterator의 경우 범위 유효성 검사를 포함할 수 있음.
            // if (first > last) { error ...}
            size = n;
            capacity = n;
            data = _allocator.allocate(n);

            size_type i = 0;
            try
            {
                for (InputIt it = first; it != last; ++it, ++i)
                    _allocator.construct(data + i, *it);
            }
            catch (...)
            {
                for (size_type j = 0; j < i; ++j)
                    _allocator.destroy(data + j);
                _allocator.deallocate(data, n);
            }
        }
        // c++23에서는 std::ranges based도 추가됨

        // Copy constructor
        // class scope 내부이므로 template argument는 적지 않아도 무방 (외부일시 적어야 함)
        vector(const vector &other)
            : _alloc(other._alloc), _size(other._size), _capacity(other._capacity)
        {
            // data: 새로 allocate한 후 하나씩 construct
            _data = _allocator.allocate(_capacity);
            for (size_t i = 0; i < _size; ++i)
                _allocator.construct(_data[i], other._data[i]);
        }

        // Copy assignment operator
        // 이미 존재하는 객체에 다른 객체의 내용을 덮어씀
        vector &operator=(const vector &other)
        {
            if (this == other)
                return this;

            // 기존 capacity 부족 -> 기존 객체들 소멸 및 메모리 공간 해제 후 새로 할당
            if (_capacity < other._size)
            {
                clear();
                _allocator.deallocate(_data);
                _allocator.allocate(other._size);
            }
            else
                // 기존 capacity 충분 -> 기존 객체들 소멸
                clear();

            for (size_t i = 0; i < _size; ++i)
                _allocator.construct(_data[i], other._data[i]);
        }

        // destructor
        ~vector()
        {
            for (size_t i = 0; i < size; ++i)
                _allocator.destory(data + i);
            _allocator.deallocate(data, capacity);
        }

        // -------------------- element access -------------------- //

        // []: 범위 검사 없음
        // at(): 범위 검사 수행, 오류시 out_of_range 예외 던짐
        reference       operator[](size_type pos) { return _data[pos]; }
        const reference operator[](size_type pos) const { return _data[pos]; }
        reference       at(size_type pos)
        {
            if (pos >= _size)
                throw std::out_of_range();
            return _data[pos];
        }
        const reference at(size_type pos) const
        {
            if (pos >= _size)
                throw std::out_of_range();
            return _data[pos];
        }
        reference       front() { return _data[0]; }
        const reference front() const { return _data[0]; }
        reference       back() { return _data[_size - 1]; }
        const reference back() const { return _data[_size - 1]; }
        T              *data() { return _data; }
        const T        *data() const { return _data; }

        // -------------------- capacity -------------------- //
        size_type size() const { return size; }
        size_type max_size() const { return _allocator.max_size(); }
        size_type capacity() const { return capacity; }
        bool      empty() const { return (size == 0); }
        void      reserve(size_type new_cap);

        // -------------------- modifiers -------------------- //

        void     push_back(const T &value);
        void     pop_back();
        iterator insert(const_iterator pos, const T &value);
        iterator insert(const_iterator pos, size_type count, const T &value);
        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last);
        void     resize(size_type count) void resize(size_type count, const value_type &value);

        // -------------------- iterators -------------------- //
        iterator               begin();
        const_iterator         begin() const;
        iterator               end();
        const_iterator         end() const;
        reverse_iterator       rbegin();
        const_reverse_iterator rbegin() const;
        reverse_iterator       rend();
        const_reverse_iterator rend() const;

      private:
        Alloc<T> _allocator;
        T       *_data;
        size_t   _size;
        size_t   _capacity;

        // 모든 원소 destroy 후 _size = 0으로 설정 (메모리 공간은 해제하지 않는다)
        void clear()
        {
            for (size_type i = 0; i < _size; ++i)
                _alloc.destroy(_data + i);
            _size = 0;
        }
    };

    // -------------------- operators -------------------- //

    template <class T, class Alloc>
    bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

    template <class T, class Alloc>
    bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

    template <class T, class Alloc>
    bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

    template <class T, class Alloc>
    bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

    template <class T, class Alloc>
    bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

    template <class T, class Alloc>
    bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

} // namespace ft

#endif // FT_VECTOR_H
