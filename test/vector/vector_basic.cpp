#include <iostream>
#include <string>
#include <vector>
#include "test_config.h"
#include "test_print.h"
#include "vector.h"

void test_vector_basic()
{
    FILE_BANNER();

    // ===========================================================
    // 1. 기본 생성자 / 크기 지정 생성자 / 값 채운 생성자
    // ===========================================================
    print_section("Constructors");

    NAMESPACE::vector<int> v_default;
    NAMESPACE::vector<int> v_filled(5, 42);
    int                    arr[] = {1, 2, 3, 4};
    NAMESPACE::vector<int> v_range(arr, arr + 4);
    NAMESPACE::vector<int> v_copy(v_range);

    std::cout << "v_default.size(): " << v_default.size() << '\n';
    std::cout << "v_filled: ";
    for (size_t i = 0; i < v_filled.size(); ++i)
        std::cout << v_filled[i] << " ";
    std::cout << '\n';
    std::cout << "v_range: ";
    for (size_t i = 0; i < v_range.size(); ++i)
        std::cout << v_range[i] << " ";
    std::cout << '\n';
    std::cout << "v_copy == v_range ? " << (v_copy == v_range ? "true" : "false") << '\n';

    // ===========================================================
    // 2. 대입 연산자 (operator=)
    // ===========================================================
    print_section("Assignment operator");

    NAMESPACE::vector<int> v_assign;
    v_assign = v_filled;
    std::cout << "v_assign.size(): " << v_assign.size() << '\n';
    std::cout << "v_assign[2]: " << v_assign[2] << '\n';

    // ===========================================================
    // 3. Capacity 관련 테스트
    // ===========================================================
    print_section("Capacity test");

    std::cout << "v_filled.size():     " << v_filled.size() << '\n';
    std::cout << "v_filled.capacity(): " << v_filled.capacity() << '\n';
    std::cout << "v_filled.max_size(): " << v_filled.max_size() << '\n';
    std::cout << "v_default.empty():   " << v_default.empty() << '\n';

    // ===========================================================
    // 4. at(), operator[], front(), back()
    // ===========================================================
    print_section("Element access");

    NAMESPACE::vector<std::string> v_str(3, "KU");
    v_str[1] = "CSE";
    v_str.at(2) = "ft_containers";
    std::cout << "front: " << v_str.front() << '\n';
    std::cout << "back:  " << v_str.back() << '\n';
    std::cout << "All elements: ";
    for (size_t i = 0; i < v_str.size(); ++i)
        std::cout << v_str[i] << " ";
    std::cout << '\n';

    // ===========================================================
    // 5. at() 예외 처리
    // ===========================================================
    print_section("Exception test for at()");
    try
    {
        v_str.at(10);
    }
    catch (const std::out_of_range &e)
    {
        std::cout << "Caught exception: " << e.what() << '\n';
    }

    std::cout << '\n';
}
