#ifndef FT_STACK_H
#define FT_STACK_H

#include <deque>
#include <cstddef>

namespace ft
{
    template <class T, class Container = std::deque<T> >
    class stack
    {
      public:
        typedef T          value_type;
        typedef Container  container_type;
        typedef std::size_t size_type;

      protected:
        container_type c;

      public:
        // ---------------- Constructors / Destructor ---------------- //
        explicit stack(const container_type &cont = container_type()) : c(cont) {}
        stack(const stack &other) : c(other.c) {}
        ~stack() {}
        stack &operator=(const stack &other)
        {
            if (this != &other)
                c = other.c;
            return *this;
        }

        // ---------------- Element access ---------------- //
        value_type &top() { static value_type tmp = value_type(); return tmp; }
        const value_type &top() const { static value_type tmp = value_type(); return tmp; }

        // ---------------- Capacity ---------------- //
        bool empty() const { return true; }
        size_type size() const { return 0; }

        // ---------------- Modifiers ---------------- //
        void push(const value_type &val) { (void)val; }
        void pop() {}

        // ---------------- Comparison operators ---------------- //
        friend bool operator==(const stack &lhs, const stack &rhs)
        {
            (void)lhs; (void)rhs;
            return true;
        }
        friend bool operator!=(const stack &lhs, const stack &rhs)
        {
            return !(lhs == rhs);
        }
        friend bool operator<(const stack &lhs, const stack &rhs)
        {
            (void)lhs; (void)rhs;
            return false;
        }
        friend bool operator<=(const stack &lhs, const stack &rhs)
        {
            return !(rhs < lhs);
        }
        friend bool operator>(const stack &lhs, const stack &rhs)
        {
            return rhs < lhs;
        }
        friend bool operator>=(const stack &lhs, const stack &rhs)
        {
            return !(lhs < rhs);
        }
    };
} // namespace ft

#endif // FT_STACK_H
