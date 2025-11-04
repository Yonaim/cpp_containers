/*
Substitution Failure Is Not An Error (SFINAE)
- 함수 오버로드 시, 대입(substitution)이 실패하더라도 에러로 취급하지 않고 다른 후보를 시도
- This rule applies during overload resolution of function templates
- This feature is used in template metaprogramming.
*/
// C++98 버전에서는 없었다. Boost 팀이 처음 구현했다가 이후 표준에 포함됨.

#include <iostream>
#include "enable_if.h"
#include "is_floating_point.h"
#include "is_integral.h"
#include "test_config.h"
#include "test_print.h"

// =========================== use in Return Type ==============================

template <typename T>
typename NAMESPACE::enable_if<NAMESPACE::is_integral<T>::value, void>::type printType(T val)
{
    std::cout << val << " is integral (Return Type)\n";
}

template <typename T>
typename NAMESPACE::enable_if<NAMESPACE::is_floating_point<T>::value, void>::type printType(T val)
{
    std::cout << val << " is floating-point (Return Type)\n";
}

// 여러 후보가 동시에 true면 안 됨!
// template <typename T>
// typename NAMESPACE::enable_if<true, void>::type printType(T val)
// {
//     std::cout << val << " Always true\n";
// }

// ========================= use in Parameter Type =============================

// void 포인터는 기본값(0)을 주기 위한 트릭
template <typename T>
void printTypeParam(
    T val, typename NAMESPACE::enable_if<NAMESPACE::is_integral<T>::value, void>::type * = 0)
{
    std::cout << val << " is integral (Parameter Type)\n";
}

// ====================== use in Template Argument Type ========================

// C++98에서는 Default template argument가 제한되어 있었음
// template <typename T, typename Enable = typename
// NAMESPACE::enable_if<NAMESPACE::is_integral<T>::value, void>::type>
// void printTypeTemplate(T val)
// {
//     std::cout << val << " is integral (Template Argument Type)\n";
// }

// =============================================================================

void test_enable_if()
{
    FILE_BANNER();

    // ===========================================================
    // Return type specialization
    // ===========================================================
    print_section("Return type SFINAE");
    printType(42);
    printType((float)3.14);

    // ===========================================================
    // Parameter type specialization
    // ===========================================================
    print_section("Parameter type SFINAE");
    printTypeParam(42);
    // printTypeParam((float)42.42); // Error: not integral

    // ===========================================================
    // Template argument type specialization
    // ===========================================================
    print_section("Template argument SFINAE (commented out)");
    // printTypeTemplate((int)42);
    // printTypeTemplate((float)42.42);
}
