#ifndef FT_VECTOR_H
#define FT_VECTOR_H

#include <memory> // TODO: ft::allocator로 교체

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

        // vector() : vector(Allocator()) {} // delegating constructor 사용 (C++11)
        explicit vector(const Allocator &alloc = Allocator());
        explicit vector(size_type count, const T &value = T(),
                        const Allocator &alloc = Allocator());
        template <class InputIt>
        vector(InputIt first, InputIt last, const Allocator &alloc = Allocator());
        template <class InputIt>
        vector(InputIt first, InputIt last, const Allocator &alloc = Allocator());
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
