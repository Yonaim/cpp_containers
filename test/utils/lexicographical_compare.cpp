#include <vector>
#include "test_config.h"
#include "test_print.h"

#if NAMESPACE == std
    #include <algorithm>
#else
    #include "lexicographical_compare.h"
#endif

// Compare 함수
// named requirements의 일종인 'Compare'를 만족해야 함
// https://en.cppreference.com/w/cpp/named_req/Compare.html
bool comp_custom(int a, int b) { return (a + 1 == b); }

void test_lexicographical_compare()
{
    FILE_BANNER();

    int              arr1[] = {1, 2, 3};
    int              arr2[] = {1, 3, 3};
    int              arr3[] = {1, 2, 3, 4};
    std::vector<int> v1(arr1, arr1 + 3);
    std::vector<int> v2(arr2, arr2 + 3);
    std::vector<int> v3(arr3, arr3 + 4);

    // ===========================================================
    // 1. 기본 4-parameter 버전
    // ===========================================================
    print_section("4-parameters (no Compare)");
    print_bool(ft::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end())); // true
    print_bool(ft::lexicographical_compare(v2.begin(), v2.end(), v1.begin(), v1.end())); // false
    print_bool(ft::lexicographical_compare(v1.begin(), v1.end(), v3.begin(), v3.end())); // true
    print_bool(ft::lexicographical_compare(v3.begin(), v3.end(), v1.begin(), v1.end())); // false

    // ===========================================================
    // 2. Compare 함수 버전
    // ===========================================================
    print_section("4-parameters (with Compare)");
    print_bool(
        ft::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), comp_custom));
    print_bool(
        ft::lexicographical_compare(v2.begin(), v2.end(), v3.begin(), v3.end(), comp_custom));

    // ===========================================================
    // 3. Empty range test
    // ===========================================================
    print_section("Empty range");
    std::vector<int> empty;
    print_bool(ft::lexicographical_compare(empty.begin(), empty.end(), v1.begin(),
                                           v1.end())); // true
    print_bool(ft::lexicographical_compare(v1.begin(), v1.end(), empty.begin(),
                                           empty.end())); // false
}