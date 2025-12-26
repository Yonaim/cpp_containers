#ifndef FT_IS_EMPTY_H
#define FT_IS_EMPTY_H

#include "is_class.h"

/*
    typedef vs enum
    - typedef에는 평가식을 넣을 수 없다
*/
namespace ft
{
    template <class T, bool = ft::is_class<T>::value>
    class _is_empty_impl
    {
        enum
        {
            value = false
        };
    };

    template <class T>
    class _is_empty_impl<T, true> // T is class or struct
    {
      private:
        struct Derived : T
        {
            char dummy;
        };

      public:
        enum
        {
            value = (sizeof(T) == sizeof(Derived))
        };
    };

    template <class T>
    class is_empty : _is_empty_impl<T>
    {
    };
} // namespace ft

#endif
