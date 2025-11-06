#include "test_config.h"
#include "test_print.h"

#ifdef STD_MODE
    #include <type_traits>
#else
    #include "is_integral.h"
#endif

void test_is_integral()
{
    FILE_BANNER();

    std::cout << std::boolalpha; // print true / false instead of 1 / 0

    // ===========================================================
    // Integral types
    // ===========================================================
    print_section("Integral types");
    std::cout << "int:               " << ft::is_integral<int>::value << "\n";
    std::cout << "const int:         " << ft::is_integral<const int>::value << "\n";
    std::cout << "volatile int:      " << ft::is_integral<volatile int>::value << "\n";
    std::cout << "unsigned long:     " << ft::is_integral<unsigned long>::value << "\n";
    std::cout << "long long:         " << ft::is_integral<long long>::value << "\n";
    std::cout << "bool:              " << ft::is_integral<bool>::value << "\n";
    std::cout << "char:              " << ft::is_integral<char>::value << "\n";
    std::cout << "wchar_t:           " << ft::is_integral<wchar_t>::value << "\n";

    // ===========================================================
    // Floating-point types
    // ===========================================================
    print_section("Floating-point types");
    std::cout << "float:             " << ft::is_integral<float>::value << "\n";
    std::cout << "double:            " << ft::is_integral<double>::value << "\n";
    std::cout << "long double:       " << ft::is_integral<long double>::value << "\n";

    // ===========================================================
    // Other types
    // ===========================================================
    print_section("Other types");
    std::cout << "void:              " << ft::is_integral<void>::value << "\n";
    std::cout << "int*:              " << ft::is_integral<int *>::value << "\n";
    std::cout << "int&:              " << ft::is_integral<int &>::value << "\n";
    std::cout << "std::string:       " << ft::is_integral<std::string>::value << "\n";
}
