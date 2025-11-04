#include <iostream>
#include <typeinfo>
#include <vector>
#include "iterator_traits.h"
#include "test_config.h"
#include "test_print.h"

void test_iterator_traits()
{
    FILE_BANNER();

    // ===========================================================
    // 1. Pointer specialization
    // ===========================================================
    print_section("Pointer specialization");
    typedef typename NAMESPACE::iterator_traits<int *>::value_type        value_t;
    typedef typename NAMESPACE::iterator_traits<int *>::pointer           ptr_t;
    typedef typename NAMESPACE::iterator_traits<int *>::reference         ref_t;
    typedef typename NAMESPACE::iterator_traits<int *>::difference_type   diff_t;
    typedef typename NAMESPACE::iterator_traits<int *>::iterator_category cat_t;

    std::cout << "value_type: " << typeid(value_t).name() << '\n';
    std::cout << "pointer:    " << typeid(ptr_t).name() << '\n';
    std::cout << "reference:  " << typeid(ref_t).name() << '\n';
    std::cout << "diff_type:  " << typeid(diff_t).name() << '\n';
    std::cout << "category:   " << typeid(cat_t).name() << '\n';

    // ===========================================================
    // 2. const pointer specialization
    // ===========================================================
    print_section("const pointer specialization");
    typedef typename NAMESPACE::iterator_traits<const int *>::value_type const_value_t;
    std::cout << "const pointer value_type: " << typeid(const_value_t).name() << '\n';

    // ===========================================================
    // 3. vector<int>::iterator test
    // ===========================================================
    print_section("vector<int>::iterator traits");
    typedef typename NAMESPACE::iterator_traits<std::vector<int>::iterator>::iterator_category
        cat_vec_t;
    std::cout << "category: " << typeid(cat_vec_t).name() << '\n';
}
