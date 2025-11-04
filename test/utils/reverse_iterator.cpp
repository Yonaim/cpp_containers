#include <iostream>
#include <vector>
#include "reverse_iterator.h"
#include "test_config.h"
#include "test_print.h"

void test_reverse_iterator()
{
    FILE_BANNER();

    int              arr[] = {10, 20, 30, 40};
    std::vector<int> v(arr, arr + 4);

    NAMESPACE::reverse_iterator<std::vector<int>::iterator> rbegin(v.end());
    NAMESPACE::reverse_iterator<std::vector<int>::iterator> rend(v.begin());

    // ===========================================================
    // 1. 기본 순회
    // ===========================================================
    print_section("Basic reverse iteration");
    for (NAMESPACE::reverse_iterator<std::vector<int>::iterator> it = rbegin; it != rend; ++it)
        std::cout << *it << " ";
    std::cout << '\n';

    // ===========================================================
    // 2. 연산자 테스트
    // ===========================================================
    print_section("Operator tests");
    NAMESPACE::reverse_iterator<std::vector<int>::iterator> it1 = rbegin;
    NAMESPACE::reverse_iterator<std::vector<int>::iterator> it2 = it1 + 2;
    std::cout << "*(it1 + 2): " << *it2 << '\n';

    it1++;
    std::cout << "After it1++: " << *it1 << '\n';
    it1--;
    std::cout << "After it1--: " << *it1 << '\n';

    print_bool(it1 == rbegin);
    print_bool(it2 > it1);
}
