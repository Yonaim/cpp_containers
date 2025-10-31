#include <iostream>
#include <string>
#include "is_integral.h"
#include "test_config.h"

void test_is_integral()
{
    std::cout << std::boolalpha; // true / false로 출력
    std::cout << "=== Testing " << NAMESPACE_STR << "::is_integral ===\n";

    // 정수형 타입
    std::cout << "int:               " << NAMESPACE::is_integral<int>::value << "\n";
    std::cout << "const int:         " << NAMESPACE::is_integral<const int>::value << "\n";
    std::cout << "volatile int:      " << NAMESPACE::is_integral<volatile int>::value << "\n";
    std::cout << "unsigned long:     " << NAMESPACE::is_integral<unsigned long>::value << "\n";
    std::cout << "long long:         " << NAMESPACE::is_integral<long long>::value << "\n";
    std::cout << "bool:              " << NAMESPACE::is_integral<bool>::value << "\n";
    std::cout << "char:              " << NAMESPACE::is_integral<char>::value << "\n";
    std::cout << "wchar_t:           " << NAMESPACE::is_integral<wchar_t>::value << "\n";

    // 실수형 타입
    std::cout << "float:             " << NAMESPACE::is_integral<float>::value << "\n";
    std::cout << "double:            " << NAMESPACE::is_integral<double>::value << "\n";
    std::cout << "long double:       " << NAMESPACE::is_integral<long double>::value << "\n";

    // 기타 타입
    std::cout << "void:              " << NAMESPACE::is_integral<void>::value << "\n";
    std::cout << "int*:              " << NAMESPACE::is_integral<int *>::value << "\n";
    std::cout << "int&:              " << NAMESPACE::is_integral<int &>::value << "\n";
    std::cout << "std::string:       " << NAMESPACE::is_integral<std::string>::value << "\n";

    std::cout << "=================================\n\n";
}
