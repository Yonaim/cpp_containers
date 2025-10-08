#include <algorithm>
#include <iostream>
#include <vector>
#include "equal.h"
#include "utils/print.h"

// named requirements의 일종인 'BinaryPredicate'를 만족해야함!!
// https://en.cppreference.com/w/cpp/named_req/BinaryPredicate.html
static bool pred(int a, int b) { return (a + 3 == b); }

void test_equal()
{
    int              arr_1[] = {1, 2, 3};
    int              arr_2[] = {4, 5, 6};
    std::vector<int> v1(arr_1, arr_1 + 3);
    std::vector<int> v2(arr_2, arr_2 + 3);
    std::vector<int> v1_same(arr_1, arr_1 + 3);
    std::vector<int> v1_shorter(arr_1, arr_1 + 2);

    // =========== 3-parameters (first1, end1, first2) ===========
    print_title("3-parameters (first1, end1, first2)");
    print_bool(ft::equal(v1.begin(), v1.end(), v1_same.begin())); // true
    print_bool(ft::equal(v1.begin(), v1.end(), v2.begin()));      // false
    // UB Error!!!
    // std::cout << (ft::equal(v1.begin(), v1.end(), v1_shorter.begin()) ? "true" : "false") <<
    // '\n';

    // =========== 4-parameters (first1, end1, first2, end2) (C++14) ===========
    print_title("4-parameters (first1, end1, first2, end2) (C++14)");
    // No UB. It's safe!
    // print_bool(ft::equal(v1.begin(), v1.end(), v1_shorter.begin(), v1_shorter.end()));

    // =========== 3-parameters with BinaryPred ===========
    print_title("3-parameters with BinaryPred");

    // BinaryPred = Binary Predicate (이항 논리식)
    // BinaryPred는 호출할 수 있는 것이면 아무거나 가능 (함수 포인터, 람다, functor)
    print_bool(ft::equal(v1.begin(), v1.end(), v2.begin(), pred)); // true
}
