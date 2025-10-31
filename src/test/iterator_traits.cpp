#include <iostream>
#include <string>
#include <vector>
#include "iterator_traits.h"
#include "test_config.h"
#include "type_name_string.h"

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

    std::cout << "====== int* ======\n";
    print_iterator_traits<int *>();

    std::cout << "====== const int* ======\n";
    print_iterator_traits<const int *>();
}
