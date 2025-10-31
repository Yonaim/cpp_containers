#include <iostream>
#include <string>
#include <vector>
#include "iterator_traits.h"
#include "test_config.h"

// ========================= Type name strings =================================

// implement using partial specialization!

// type_name_string
template <class T>
struct type_name_string
{
    static std::string value() { return "unkonwn"; }
};

// preprocessor for register specific type name 'T'
// just for avoid duplicated codes...
#define REGISTER_TYPE_NAME(T)                                                                      \
    template <>                                                                                    \
    struct type_name_string<T>                                                                     \
    {                                                                                              \
        static std::string value() { return #T; }                                                  \
    };

// === 기본 타입 등록 ===
REGISTER_TYPE_NAME(void);
REGISTER_TYPE_NAME(bool);
REGISTER_TYPE_NAME(char);
REGISTER_TYPE_NAME(unsigned char);
REGISTER_TYPE_NAME(short);
REGISTER_TYPE_NAME(unsigned short);
REGISTER_TYPE_NAME(int);
REGISTER_TYPE_NAME(unsigned int);
REGISTER_TYPE_NAME(long);
REGISTER_TYPE_NAME(unsigned long);
REGISTER_TYPE_NAME(long long);
REGISTER_TYPE_NAME(unsigned long long);
REGISTER_TYPE_NAME(float);
REGISTER_TYPE_NAME(double);
REGISTER_TYPE_NAME(long double);

// === 포인터 및 참조 타입 등록 ===
template <class T>
struct type_name_string<T *>
{
    static std::string value() { return type_name_string<T>::value() + "*"; }
};

template <class T>
struct type_name_string<const T *>
{
    static std::string value() { return "const " + type_name_string<T>::value() + "*"; }
};

template <class T>
struct type_name_string<T &>
{
    static std::string value() { return type_name_string<T>::value() + "&"; }
};

template <class T>
struct type_name_string<const T &>
{
    static std::string value() { return "const " + type_name_string<T>::value() + "&"; }
};

// === 표준 iterator category 태그 등록 ===
REGISTER_TYPE_NAME(std::input_iterator_tag);
REGISTER_TYPE_NAME(std::output_iterator_tag);
REGISTER_TYPE_NAME(std::forward_iterator_tag);
REGISTER_TYPE_NAME(std::bidirectional_iterator_tag);
REGISTER_TYPE_NAME(std::random_access_iterator_tag);

// =========================== Print Traits ====================================

template <class Iter>
void print_iterator_traits(void)
{
    typedef typename NAMESPACE::iterator_traits<Iter> traits;
    std::cout << "value_type:          " << type_name_string<typename traits::value_type>::value()
              << '\n';
    std::cout << "difference_type:     "
              << type_name_string<typename traits::difference_type>::value() << '\n';
    std::cout << "pointer:             " << type_name_string<typename traits::pointer>::value()
              << '\n';
    std::cout << "reference:           " << type_name_string<typename traits::reference>::value()
              << '\n';
    std::cout << "iterator_category:   "
              << type_name_string<typename traits::iterator_category>::value() << "\n\n";
}

void test_iterator_traits()
{
    std::cout << "=== " << NAMESPACE_STR << "::vector<int>::iterator ===\n";
    print_iterator_traits<typename std::vector<int>::iterator>();

    std::cout << "=== " << NAMESPACE_STR << "::vector<int>::const_iterator ===\n";
    print_iterator_traits<typename std::vector<int>::const_iterator>();

    std::cout << "=== int* ===\n";
    print_iterator_traits<int *>();

    std::cout << "=== const int* ===\n";
    print_iterator_traits<const int *>();
}
