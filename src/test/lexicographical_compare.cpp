#include <algorithm>
#include <iostream>
#include <vector>
#include "lexicographical_compare.h"
#include "test_config.h"
#include "utils/print.h"

// named requirements의 일종인 'Compare'를 만족해야 함
// https://en.cppreference.com/w/cpp/named_req/Compare.html
bool comp(int a, int b) { return (a + 1 == b); }

void test_lexicographical_compare()
{
    int              arr_1[] = {1, 2, 3};
    int              arr_2[] = {1, 3, 3};
    std::vector<int> v1(arr_1, arr_1 + 3);
    std::vector<int> v2(arr_2, arr_2 + 3);
    std::vector<int> v1_same(arr_1, arr_1 + 3);
    std::vector<int> v1_shorter(arr_1, arr_1 + 2);

    print_title("lexicographical_compare test");

    // ===========  4-parameters (first1, end1, first2, end2) ===========
    print_title("4-parameters (no Compare)");
    print_bool(
        NAMESPACE::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end())); // true
    print_bool(
        NAMESPACE::lexicographical_compare(v2.begin(), v2.end(), v1.begin(), v1.end())); // false
    print_bool(NAMESPACE::lexicographical_compare(v1.begin(), v1.end(), v1_shorter.begin(),
                                                  v1_shorter.end())); // false
    print_bool(NAMESPACE::lexicographical_compare(v1_shorter.begin(), v1_shorter.end(), v1.begin(),
                                                  v1.end())); // true

    // =========== 4-parameters with Compare ===========
    print_title("4-parameters with Compare");
    // Compare는 호출 가능한 객체이면 무엇이든 가능 (함수 포인터, 람다, functor 등)
    print_bool(NAMESPACE::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(),
                                                  comp)); // true
}
