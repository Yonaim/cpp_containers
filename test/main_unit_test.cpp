#include "test_config.h"
#include "test_print.h"
#include "test_units.h"

#ifndef TEST_UTILS
    #define TEST_UTILS 0
#endif

#ifndef TEST_VECTOR
    #define TEST_VECTOR 0
#endif

#ifndef TEST_MAP
    #define TEST_MAP 0
#endif

#ifndef TEST_STACK
    #define TEST_STACK 0
#endif

int main()
{
#if TEST_UTILS
    print_module_banner("Utils Tests");
    test_pair();
    test_equal();
    test_lexicographical_compare();
    test_enable_if();
    test_is_integral();
    test_iterator_traits();
    test_reverse_iterator();
#endif

#if TEST_VECTOR
    print_module_banner("Vector Tests");
    test_vector_basic();
    test_vector_modifiers();
#endif

#if TEST_MAP
    print_module_banner("Map Tests");
    test_map_basic();
    test_map_modifiers();
    test_map_lookup_compare();
#endif

#if TEST_STACK
    print_module_banner("Stack Tests");
    test_stack();
#endif

    return 0;
}
