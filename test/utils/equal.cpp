#include <iostream>
#include <vector>
#include "equal.h"
#include "test_config.h"
#include "test_print.h"

// named requirements의 일종인 'BinaryPredicate'를 만족해야함
// https://en.cppreference.com/w/cpp/named_req/BinaryPredicate.html
bool pred_equal(int a, int b) { return a == b; }
bool pred_even(int a, int b) { return (a % 2) == (b % 2); }

void test_equal()
{
    FILE_BANNER();

    // ===========================================================
    // 1. 기본 equal 비교
    // ===========================================================
    print_section("3-parameters (no Predicate)");

    int arr1[] = {1, 2, 3, 4};
    int arr2[] = {1, 2, 3, 4};
    int arr3[] = {1, 2, 3, 9};

    std::vector<int> v1(arr1, arr1 + 4);
    std::vector<int> v2(arr2, arr2 + 4);
    std::vector<int> v3(arr3, arr3 + 4);

    print_bool(NAMESPACE::equal(v1.begin(), v1.end(), v2.begin())); // true
    print_bool(NAMESPACE::equal(v1.begin(), v1.end(), v3.begin())); // false

    // ===========================================================
    // 2. 사용자 정의 Predicate 사용
    // ===========================================================
    print_section("4-parameters (with Predicate)");

    print_bool(NAMESPACE::equal(v1.begin(), v1.end(), v2.begin(), pred_equal)); // true
    print_bool(NAMESPACE::equal(v1.begin(), v1.end(), v3.begin(), pred_even));  // true (짝/홀 비교)

    // ===========================================================
    // 3. Empty range test
    // ===========================================================
    print_section("Empty range");
    std::vector<int> empty;
    print_bool(NAMESPACE::equal(empty.begin(), empty.end(), empty.begin())); // true
}
