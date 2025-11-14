#ifndef FT_VECTOR_H
#define FT_VECTOR_H

#include <memory> // TODO: ft::allocator로 교체
#include <sstream>
#include <stdexcept>
#include "enable_if.h"
#include "equal.h"
#include "is_integral.h"
#include "lexicographical_compare.h"
#include "reverse_iterator.h"

// TODO: bool 타입에 대한 partial specialization

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
    - 원소의 순서를 항상 보장 (order guarantee)
    - 중간 삽입/삭제 시 삭제 위치 뒤쪽의 모든 원소 재배치 -> O(n)
    - 맨 뒤 삽입/삭제 시 평균적으로 O(1)
    - 메모리 공간 부족시 전체 새로 할당 후 다시 대입
    */

    // T: The type of the elements (must be CopyAssignable & CopyConstructible)
    // Allocator: customizable (default one is std::allocator)
    template <class T, class Allocator = std::allocator<T> >
    class vector
    {
      public:
        // -------------------- Member types -------------------- //
        typedef T                                        value_type;
        typedef Allocator                                allocator_type;
        typedef typename allocator_type::reference       reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;
        typedef pointer                                  iterator;
        typedef const_pointer                            const_iterator;
        typedef ft::reverse_iterator<iterator>           reverse_iterator;
        typedef ft::reverse_iterator<const_iterator>     const_reverse_iterator;
        typedef std::size_t                              size_type;
        typedef std::ptrdiff_t                           difference_type;

        // -------------------- Constructors -------------------- //

        // Default constructor
        explicit vector(const Allocator &alloc = Allocator())
            : _allocator(alloc), _data(NULL), _size(0), _capacity(0)
        {
        }

        // Fill constructor (default-inserted elements)
        explicit vector(size_type count, const Allocator &alloc = Allocator())
            : _allocator(alloc), _data(_allocator.allocate(count)), _size(count), _capacity(count)
        {
            for (size_type i = 0; i < count; ++i)
                _allocator.construct(_data + i, value_type());
        }

        // Fill constructor (with specified value)
        vector(size_type count, const T &value, const Allocator &alloc = Allocator())
            : _allocator(alloc), _data(_allocator.allocate(count)), _size(count), _capacity(count)
        {
            for (size_type i = 0; i < count; ++i)
                _allocator.construct(_data + i, value);
        }

        /*
        C++98에서는 default-inserted elements 개념이 없었기 때문에,
        count만 지정하면서 Allocator를 커스텀하려면 두 번째 인자에 기본 생성자를 직접 넘겨야 했다.
        ex) vector(42, MyType(), MyAlloc<MyType>());
        */

        // Range constructor
        // InputIt 타입을 오인하는 것을 방지하기 위해,
        // enable_if를 이용해 !is_integral(=not integral)을 강제 (SFINAE)
        // TODO: iterator_traits로 막는 것이 어려운 이유에 대해 조사
        template <class InputIt>
        vector(InputIt first, InputIt last, const Allocator &alloc = Allocator(),
               typename ft::enable_if<!ft::is_integral<InputIt>::value, void>::type * = 0)
            : _allocator(alloc), _data(NULL), _size(0), _capacity(0)
        {
            size_type n = 0;
            for (InputIt it = first; it != last; ++it)
                ++n;

            _size = n;
            _capacity = n;
            _data = _allocator.allocate(n);

            size_type i = 0;
            try
            {
                for (InputIt it = first; it != last; ++it, ++i)
                    _allocator.construct(_data + i, *it);
            }
            catch (...)
            {
                for (size_type j = 0; j < i; ++j)
                    _allocator.destroy(_data + j);
                _allocator.deallocate(_data, n);
                throw;
            }
        }

        // Copy constructor
        vector(const vector &other)
            : _allocator(other._allocator), _size(other._size), _capacity(other._capacity)
        {
            _data = _allocator.allocate(_capacity);
            for (size_type i = 0; i < _size; ++i)
                _allocator.construct(_data + i, other._data[i]);
        }

        // Copy assignment operator
        vector &operator=(const vector &other)
        {
            if (this == &other)
                return *this;

            if (_capacity < other._size)
            {
                clear();
                _allocator.deallocate(_data, _capacity);
                _data = _allocator.allocate(other._size);
                _capacity = other._size;
            }
            else
                clear();

            for (size_type i = 0; i < other._size; ++i)
                _allocator.construct(_data + i, other._data[i]);

            _size = other._size;
            return *this;
        }

        // Destructor
        ~vector()
        {
            for (size_type i = 0; i < _size; ++i)
                _allocator.destroy(_data + i);
            if (_data)
                _allocator.deallocate(_data, _capacity);
        }

        // -------------------- Element access -------------------- //
        reference       operator[](size_type pos) { return _data[pos]; }
        const_reference operator[](size_type pos) const { return _data[pos]; }

        reference at(size_type pos)
        {
            // TODO: 컴파일러 별로 다르다는데?
            if (pos >= _size)
            {
                std::ostringstream oss;
                oss << "vector::_M_range_check: __n (which is " << pos
                    << ") >= this->size() (which is " << _size << ")";
                throw std::out_of_range(oss.str());
            }
            return _data[pos];
        }

        const_reference at(size_type pos) const
        {
            if (pos >= _size)
                throw std::out_of_range("ft::vector::at: index out of range");
            return _data[pos];
        }

        reference       front() { return _data[0]; }
        const_reference front() const { return _data[0]; }
        reference       back() { return _data[_size - 1]; }
        const_reference back() const { return _data[_size - 1]; }

        T       *data() { return _data; }
        const T *data() const { return _data; }

        // -------------------- Capacity -------------------- //
        size_type size() const { return _size; }
        size_type max_size() const { return _allocator.max_size(); }
        size_type capacity() const { return _capacity; }
        bool      empty() const { return (_size == 0); }

        // Increase the capacity of the vector
        // If new_cap is greater than the current capacity(), new storage is allocated,
        // otherwise the function does nothing.
        /*
            - trivially copyable인 경우: memcpy()를 사용해도 됨.
            - trivially copyable이 아니면: 객체의 lifetime을 잃는 등, 논리적인 복제가 불가
            - 모던 c++에 is_trivially_copyable 함수 존재
            - POD(Plain Old Data)는 trivially copyable
        */
        void reserve(size_type new_cap)
        {
            if (new_cap > max_size())
                throw std::length_error("ft::vector::reserve: length_error");
            // TODO: 메세지 맞춰두기

            if (new_cap <= _capacity)
                return;

            T *new_data = _allocator.allocate(new_cap);
            for (size_type i = 0; i < _size; i++)
            {
                // POD가 아닌 한, memcpy()와 같은 단순한 메모리 복사는 불가
                // size 횟수만큼 생성과 소멸을 수행
                _allocator.construct(new_data + i, _data[i]);
                _allocator.destroy(&_data[i]);
            }
            _allocator.deallocate(_data, _capacity);
            _data = new_data;
            _capacity = new_cap;
        }

        // -------------------- Modifiers -------------------- //

        // 모든 원소 destroy 후 _size = 0으로 설정 (메모리 공간은 해제하지 않는다)
        void clear()
        {
            for (size_type i = 0; i < _size; ++i)
                _allocator.destroy(_data + i);
            _size = 0;
        }

        // adds an element to the end (복사본 저장)
        // growth policy: 1.5배 혹은 2배
        void push_back(const T &value)
        {
            if (_size + 1 > _capacity)
            {
                size_type new_cap;
                if (_capacity == 0)
                    new_cap = 1;
                else
                    new_cap = _capacity * 2;
                reserve(new_cap);
            }
            _allocator.construct(_data + (_size++), value);
        }
        // removes the last element
        void pop_back()
        {
            if (_size == 0)
                return;
            // non-trivial 타입일 수도 있으므로 destroy를 한다.
            _allocator.destroy(_data + (_size - 1));
            --_size;
        }

        // Inserts elements at the specified location in the container
        // 특정 위치에 '삽입'하므로, 지정한 pos 뒤의 원소들은 한 칸씩 뒤로 밀어야 함
        // 새 공간을 할당하지 않고 진행하려면 뒤에서부터 복사하여 덮어씌워지는 일을 방지
        // 단일 삽입
        iterator insert(const_iterator pos, const T &value)
        {
            const size_type idx = pos - begin();

            if (_size == _capacity)
                reserve(_capacity == 0 ? 1 : _capacity * 2);

            // size 타입 초기화를 _size - 1로 할때, overflow 주의 (_size = 0)
            // for (size_type i = _size - 1; i >= idx; --i)
            for (size_type i = _size; i-- > idx;) // 이 라인에서 (--) 연산을 함
            {
                _allocator.construct(_data + i + 1, _data[i]);
                _allocator.destroy(_data + i);
            }
            _allocator.construct(_data + idx, value);
            ++_size;

            // 새로 할당될시 pos는 옛날 메모리를 가리키는 것이므로 다를 수 있음!!
            // 그래서 begin + idx를 리턴한다.
            return begin() + idx;
        }
        // 다수 삽입
        iterator insert(const_iterator pos, size_type count, const T &value)
        {
            const size_type idx = pos - begin();

            if (count == 0)
                return const_cast<iterator>(pos);
            if (_size + count > _capacity)
            {
                size_type new_cap = (_capacity == 0 ? count : _capacity);
                while (new_cap < _size + count)
                    new_cap *= 1.5;
                reserve(new_cap);
            }
            // 뒤에서부터 원본 백업
            for (size_type i = _size; i-- > idx;)
            {
                _allocator.construct(_data + i + count, _data[i]);
                _allocator.destroy(_data + i);
            }
            // 새 값 삽입
            for (size_type i = 0; i < count; ++i)
                _allocator.construct(_data + idx + i, value);
            _size += count;
            return begin() + idx;
        }

        // 범위 삽입
        // enable_if로 혼동 방지
        /*
            iterator의 tag에 따라 분기 처리
            (1) Input iterator
                - distance 계산 & multi-pass 둘다 불가능
                - 하나씩 insert() 해야함.
                - insert()는 O(n)이고 이것을 n번 실행, 즉 O(n^2)
            (2) Forward & Bidirectional iterator
                - distance 계산 O(n)에 가능
                - 이후 O(n) 처리 가능
            (3) Random Access
                - distance 계산 O(1)에 가능
                - 이후 O(n) 처리 가능
        */
        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last,
                        typename ft::enable_if<!ft::is_integral<InputIt>::value, void>::type * = 0)
        {
            if (first == last)
                return const_cast<iterator>(pos);

            typedef typename ft::iterator_traits<InputIt>::iterator_category category;
            // category()는 category 타입으로 만드는 임시 객체 (오버로딩 분기용)
            return _range_insert(pos, first, last, category());
        }

        // Input iterator (single-pass)
        // pos는 random access 보장됨
        template <class InputIt>
        iterator _range_insert(const_iterator cpos, InputIt first, InputIt last,
                               std::input_iterator_tag)
        {
            const size_type idx = cpos - begin();
            iterator        pos;

            // single-pass라서 distance 계산이 불가 -> 하나씩 insert
            pos = begin() + idx;
            for (; first != last; ++first)
            {
                // insert 내부에서 reserve 호출시 기존 itertor는 무효화되므로,
                // 반드시 리턴을 받아 사용
                pos = insert(pos, *first);
                ++pos;
            }
            return begin() + idx;
        }

        // Forward & Bidirectional iterator (double-pass)
        // 상속 구조에 의해, Bidirectional은 이 함수로 dispatch
        // Random Access도 여기서 처리 (std::distance가 내부적으로 dispatch)
        template <class InputIt>
        iterator _range_insert(const_iterator cpos, InputIt first, InputIt last,
                               std::forward_iterator_tag)
        {
            const size_type idx = cpos - begin();
            const size_type count = std::distance(first, last);

            if (_size + count > _capacity)
            {
                size_type new_cap = (_capacity == 0 ? count : _capacity);
                while (new_cap < _size + count)
                    new_cap *= 1.5;
                reserve(new_cap);
            }
            // 뒤에서부터 원본 백업
            for (size_type i = _size; i-- > idx;)
            {
                _allocator.construct(_data + i + count, _data[i]);
                _allocator.destroy(_data + i);
            }
            // 새 값 삽입
            for (size_type i = 0; first != last; ++first, ++i)
                _allocator.construct(_data + idx + i, *first);
            _size += count;
            return begin() + idx;
        }

        /*
            - STL 내부 iterator는 어차피 포인터임.
                => iterator는 값이 아니라 offset으로 다루는게 안전하다
            - iterator invalidation(반복자 무효화) 규칙:
                삭제 구간 이후의 모든 iterator는 무효화된다.
        */
        // erases elements
        // 단일 삭제
        iterator erase(iterator pos)
        {
            const size_type idx = pos - begin();

            // 삭제 대상 위치 뒤쪽의 원소들을 앞으로 옮김
            for (size_type i = idx; i < _size; ++i)
            {
                _allocator.destroy(_data + i);                 // 현재 원소 삭제
                _allocator.construct(_data + i, _data[i + 1]); // 바로 뒤 원소를 복사
            }
            // 마지막 원소 제거
            _allocator.destroy(_data + _size - 1);
            --_size;

            // iterator 원칙상, 삭제된 원소는 invalidated된 것으로 취급
            // 따라서 주소가 같을지라도 새 iterator를 반환해야한다
            return begin() + idx;
        }
        // 범위 삭제
        iterator erase(iterator first, iterator last)
        {
            if (first == last)
                return first;

            const size_type idx = first - begin(); // 삭제 시작 위치
            const size_type count = last - first;

            // 삭제 대상 위치 뒤쪽의 원소들을 앞으로 옮김
            for (size_type i = idx; i + count < _size; ++i)
            {
                _allocator.destroy(_data + i);                         // 현재 원소 삭제
                _allocator.construct(_data + i, *(_data + i + count)); // count 거리 뒤 원소를 복사
            }
            for (size_type i = _size - count; i < _size; ++i)
                _allocator.destroy(_data + i);
            _size -= count;
            return first;
        }

        // changes the number of elements stored
        void resize(size_type count)
        {
            if (_size > count)
            {
                // remove
                while (_size > count)
                    _allocator.destroy(_data + (--_size));
            }
            else if (_size < count)
            {
                // add copies of T() (or value)
                if (count > _capacity)
                    reserve(count);
                while (_size < count)
                    _allocator.construct(_data + (_size++), value_type());
            }
        }
        void resize(size_type count, const value_type &value)
        {
            if (_size > count)
            {
                // remove
                while (_size > count)
                    _allocator.destroy(_data + (--_size));
            }
            else if (_size < count)
            {
                // add copies of T() (or value)
                if (count > _capacity)
                    reserve(count);
                while (_size < count)
                    _allocator.construct(_data + (_size++), value);
            }
        }

        // -------------------- Iterators -------------------- //
        iterator       begin() { return _data; }
        const_iterator begin() const { return _data; }
        iterator       end() { return _data + _size; }
        const_iterator end() const { return _data + _size; }

        // 타입명과 생성자명이 reverse_iterator (const reverse_iterator)로 같은 상황
        // 그러나 컴파일러는 문맥상으로 알맞게 판별한다
        // 반환 타입은 선언 문맥으로, return 뒤의 괄호 호출은 값 생성(표현식)으로 해석
        reverse_iterator       rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        reverse_iterator       rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

      private:
        allocator_type _allocator;
        pointer        _data;
        size_type      _size;
        size_type      _capacity;
    };

    // -------------------- Non-member operators -------------------- //

    template <class T, class Allocator>
    bool operator==(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs)
    {
        if (lhs.size() != rhs.size())
            return false;
        return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class T, class Allocator>
    bool operator!=(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs)
    {
        return !(lhs == rhs);
    }

    template <class T, class Allocator>
    bool operator<(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs)
    {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class T, class Allocator>
    bool operator<=(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs)
    {
        return !(rhs < lhs);
    }

    template <class T, class Allocator>
    bool operator>(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs)
    {
        return (rhs < lhs);
    }

    template <class T, class Allocator>
    bool operator>=(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs)
    {
        return !(lhs < rhs);
    }

} // namespace ft

#endif // FT_VECTOR_H
