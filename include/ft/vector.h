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
Random access - constant 𝓞(1).
Insertion or removal of elements at the end - amortized constant 𝓞(1).
Insertion or removal of elements - linear in the distance to the end of the vector 𝓞(n).
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
        typedef typename Allocator::reference        reference;
        typedef typename Allocator::const_reference  const_reference;
        typedef typename Allocator::pointer          pointer;
        typedef typename Allocator::const_pointer    const_pointer;
        typedef pointer                              iterator;
        typedef const_pointer                        const_iterator;
        typedef ft::reverse_iterator<iterator>       reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

        // -------------------- constructors -------------------- //

        // delegating constructor 사용 (C++11)
        // delegating constructor: 자신의 다른 생성자를 초기화 리스트에서 호출할 수 있다.
        // vector() : vector(Allocator()) {}

        // Default constructor
        explicit vector(const Allocator &alloc = Allocator());

        // Fill constructors: Default-inserted elements (C++98에는 없었다)
        // T는 기본 생성자를 통해 자동 생성
        explicit vector(size_type count, const Alloc &alloc = Allocator());

        // Fill constructors: Fill constructor with specified value
        explicit vector(size_type count, const T &value = T(),
                        const Allocator &alloc = Allocator());

        /*
        C++98 시절에는 Default-inserted elements가 없었기 때문에, count를 지정하면서 Allocator를
        커스텀하고 싶다면 두 번째 인자에 직접 기본 생성자를 포함해야했다.
        ex) vector(42, MyType(), MyAlloc<MyType>());
        */

        // Range constructor: iterator-based
        // c++23에서는 std::ranges based도 추가됨
        template <class InputIt>
        vector(InputIt first, InputIt last, const Allocator &alloc = Allocator());
        template <class InputIt>
        vector(InputIt first, InputIt last, const Allocator &alloc = Allocator());

        // Copy constructor
        // class scope 내부이므로 template argument는 적지 않아도 무방 (외부일시 적어야 함)
        vector(const vector &other);

        // Copy assignment constructor
        vector &operator=(const vector &other);

        // destructor
        ~vector();

        // -------------------- element access -------------------- //

        reference       operator[](size_type);
        const reference operator[](size_type) const;
        reference       at(size_type);
        const reference at(size_type) const;
        reference       front();
        const reference front() const;
        reference       back();
        const reference back() const;
        T              *data();
        const T        *data() const;

        // -------------------- capacity -------------------- //
        size_type size() const;
        size_type max_size() const;
        size_type capacity() const;
        bool      empty() const;

      private:
        T     *data;
        size_t size;
    };

} // namespace ft

#endif // FT_VECTOR_H
