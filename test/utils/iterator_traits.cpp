#include <vector>
#include "test_config.h"
#include "test_print.h"
#include "type_name_string.h"

#if NAMESPACE == std
    #include <iterator>
#else
    #include "iterator_traits.h"
#endif

void test_iterator_traits()
{
    FILE_BANNER();

    // ===========================================================
    // 1. Pointer specialization
    // ===========================================================
    print_section("Pointer specialization");

    typedef typename ft::iterator_traits<int *>::value_type        value_t;
    typedef typename ft::iterator_traits<int *>::pointer           ptr_t;
    typedef typename ft::iterator_traits<int *>::reference         ref_t;
    typedef typename ft::iterator_traits<int *>::difference_type   diff_t;
    typedef typename ft::iterator_traits<int *>::iterator_category cat_t;

    std::cout << "value_type: " << type_name_string<value_t>::value() << '\n';
    std::cout << "pointer:    " << type_name_string<ptr_t>::value() << '\n';
    std::cout << "reference:  " << type_name_string<ref_t>::value() << '\n';
    std::cout << "diff_type:  " << type_name_string<diff_t>::value() << '\n';
    std::cout << "category:   " << type_name_string<cat_t>::value() << '\n';

    // ===========================================================
    // 2. const pointer specialization
    // ===========================================================
    print_section("const pointer specialization");

    typedef typename ft::iterator_traits<const int *>::value_type const_value_t;
    std::cout << "const pointer value_type: "
              << type_name_string<const_value_t>::value() << '\n';

    // ===========================================================
    // 3. vector<int>::iterator test
    // ===========================================================
    print_section("vector<int>::iterator traits");

    typedef typename ft::iterator_traits<std::vector<int>::iterator>::iterator_category cat_vec_t;
    std::cout << "category: " << type_name_string<cat_vec_t>::value() << '\n';
}
