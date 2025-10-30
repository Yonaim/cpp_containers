
/*
Substitution Failure Is Not An Error (SFINAE)
- 함수 오버로드시, 대입 (substitution)이 실패하더라도 에러로 취급하지 않고 다른 후보를 시도
- This rule applies during overload resolution of function templates
- This feature is used in template metaprogramming.
*/
// C++98버전에서는 없었다. Boost팀이 만들었다가 역수입됨.

#include <iostream>
// #include <type_traits>
// type_traits 헤더 자체가 C++11부터
#include "enable_if.h"
#include "is_floating_point.h"
#include "is_integral.h"

using namespace ft;

// =========================== use in Return Type ==============================

template <typename T>
typename enable_if<is_integral<T>::value, void>::type printType(T val)
{
    std::cout << val << " is integral (Return Type)\n";
}

template <typename T>
typename enable_if<is_floating_point<T>::value, void>::type printType(T val)
{
    std::cout << val << " is floating-point (Return Type)\n";
}

// 여러 후보가 동시에 true면 안됨!
/*
template <typename T>
typename enable_if<true, void>::type printType(T val)
{
    std::cout << val << " Always true\n"; // Is This Possible?
}
*/

// ========================= use in Parameter Type =============================

// void 포인터인 건, 단순히 기본값(0)을 주기 위한 트릭
template <typename T>
void printTypeParam(T val, typename enable_if<is_integral<T>::value, void>::type * = 0)
{
    std::cout << val << " is integral (Parameter Type)\n";
}

// ====================== use in Template Argument Type ========================

// C++98에서 Default template argument는 불가하다. (여기에서 Enable)
// template <typename T, typename Enable = typename enable_if<is_integral<T>::value, void>::type>
// void printTypeTemplate(T val)
// {
//     std::cout << val << " is integral (Template Argument Type)\n";
// }

// =============================================================================

void test_enable_if()
{
    // 리턴 타입
    printType(42);
    printType((float)3.14);

    // 파라미터 타입
    printTypeParam(42);
    // printTypeParam((float)42.42);

    // 템플릿 인자 타입
    // printTypeTemplate((int)42);
    // printTypeTemplate((float)42.42);
}
