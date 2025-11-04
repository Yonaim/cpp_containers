#include <iostream>
#include <string>
#include "test_config.h"
#include "test_print.h"

#if NANESPACE == std
    #include <vector>
#else
    #include "vector.h"
#endif

void test_vector_modifiers()
{
    FILE_BANNER();

    // ===========================================================
    // 1. push_back / pop_back
    // ===========================================================
    print_section("push_back / pop_back");

    ft::vector<int> v;
    for (int i = 1; i <= 5; ++i)
    {
        v.push_back(i * 10);
        std::cout << "push_back(" << i * 10 << "), size: " << v.size()
                  << ", capacity: " << v.capacity() << '\n';
    }

    print_section("After push_back");
    for (size_t i = 0; i < v.size(); ++i)
        std::cout << v[i] << " ";
    std::cout << '\n';

    v.pop_back();
    v.pop_back();
    print_section("After pop_back x2");
    for (size_t i = 0; i < v.size(); ++i)
        std::cout << v[i] << " ";
    std::cout << "\nsize: " << v.size() << ", capacity: " << v.capacity() << "\n\n";

    // ===========================================================
    // 2. insert / erase
    // ===========================================================
    print_section("insert / erase");

    ft::vector<int>::iterator it = v.begin();
    it = v.insert(it + 1, 99); // 중간 삽입
    std::cout << "After insert(99) at index 1:\n";
    for (size_t i = 0; i < v.size(); ++i)
        std::cout << v[i] << " ";
    std::cout << "\nInserted element: " << *it << '\n';

    v.erase(v.begin() + 2);
    std::cout << "After erase(index 2):\n";
    for (size_t i = 0; i < v.size(); ++i)
        std::cout << v[i] << " ";
    std::cout << '\n';

    // ===========================================================
    // 3. clear / resize / reserve
    // ===========================================================
    print_section("clear / resize / reserve");

    std::cout << "Before clear(): size = " << v.size() << ", capacity = " << v.capacity() << '\n';
    v.clear();
    std::cout << "After clear(): size = " << v.size() << ", capacity = " << v.capacity() << '\n';

    v.resize(3, 7);
    std::cout << "After resize(3, 7): ";
    for (size_t i = 0; i < v.size(); ++i)
        std::cout << v[i] << " ";
    std::cout << " (size=" << v.size() << ", cap=" << v.capacity() << ")\n";

    v.reserve(10);
    std::cout << "After reserve(10): capacity = " << v.capacity() << '\n';

    // ===========================================================
    // 4. 반복자 (begin, end, rbegin, rend)
    // ===========================================================
    print_section("Iterators (begin, end, rbegin, rend)");

    ft::vector<std::string> vs;
    vs.push_back("A");
    vs.push_back("B");
    vs.push_back("C");

    std::cout << "Forward iteration: ";
    for (ft::vector<std::string>::iterator it = vs.begin(); it != vs.end(); ++it)
        std::cout << *it << " ";
    std::cout << '\n';

    std::cout << "Reverse iteration: ";
    for (ft::vector<std::string>::reverse_iterator rit = vs.rbegin(); rit != vs.rend(); ++rit)
        std::cout << *rit << " ";
    std::cout << '\n';

    // ===========================================================
    // 5. 비교 연산자
    // ===========================================================
    print_section("Comparison operators (==, !=, <, <=, >, >=)");

    ft::vector<int> v1(3, 10);
    ft::vector<int> v2(3, 10);
    ft::vector<int> v3(3, 20);
    ft::vector<int> v4(2, 10);

    print_bool(v1 == v2);
    print_bool(v1 != v3);
    print_bool(v1 < v3);
    print_bool(v3 > v1);
    print_bool(v4 <= v1);
    print_bool(v3 >= v2);
}
