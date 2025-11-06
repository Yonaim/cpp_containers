#include <vector>
#include "test_config.h"
#include "test_print.h"

#ifdef STD_MODE
    #include <iterator>
#else
    #include "reverse_iterator.h"
#endif

void test_reverse_iterator()
{
    FILE_BANNER();

    int              arr[] = {10, 20, 30, 40};
    std::vector<int> v(arr, arr + 4);

    ft::reverse_iterator<std::vector<int>::iterator> rbegin(v.end());
    ft::reverse_iterator<std::vector<int>::iterator> rend(v.begin());

    // ===========================================================
    // 1. 기본 순회
    // ===========================================================
    print_section("Basic reverse iteration");
    for (ft::reverse_iterator<std::vector<int>::iterator> it = rbegin; it != rend; ++it)
        std::cout << *it << " ";
    std::cout << '\n';

    // ===========================================================
    // 2. 연산자 테스트
    // ===========================================================
    print_section("Operator tests");
    ft::reverse_iterator<std::vector<int>::iterator> it1 = rbegin;
    ft::reverse_iterator<std::vector<int>::iterator> it2 = it1 + 2;
    std::cout << "*(it1 + 2): " << *it2 << '\n';

    it1++;
    std::cout << "After it1++: " << *it1 << '\n';
    it1--;
    std::cout << "After it1--: " << *it1 << '\n';

    print_bool(it1 == rbegin);
    print_bool(it2 > it1);
}
