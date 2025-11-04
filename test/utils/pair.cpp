#include <iostream>
#include <string>
#include <utility>
#include "SomeTypes.h"
#include "pair.h"
#include "test_config.h"
#include "test_print.h"

// generic function: 타입에 상관없이 동작하는 일반화된 함수 (자동으로 타입 P 추론)
template <class P>
static void member_type_check(P &parm)
{
    // P::n_type 타입으로 변수 선언
    typename P::first_type  first = parm.first;
    typename P::second_type second = parm.second;

    std::cout << "first:  " << first << '\n';
    std::cout << "second: " << second << '\n';
}

void test_pair()
{
    FILE_BANNER();

    // ===========================================================
    // 1. Generic programming test (member type aliases)
    // ===========================================================
    print_section("Generic programming");

    NAMESPACE::pair<int, std::string> p0;
    p0.first = 42;
    p0.second = "Hello world";
    member_type_check(p0);

    // ===========================================================
    // 2. Constructor / Copy constructor
    // ===========================================================
    print_section("Constructors");

    NAMESPACE::pair<SomeType1, SomeType2> p1;
    SomeType1                             some_1(123);
    SomeType2                             some_2(456);
    p1.first = some_1;
    p1.second = some_2;

    NAMESPACE::pair<SomeType1, SomeType2> p1_copy(p1);

    std::cout << "p1_copy.first.value:  " << p1_copy.first.value << '\n';
    std::cout << "p1_copy.second.value: " << p1_copy.second.value << '\n';

    // ===========================================================
    // 3. make_pair
    // ===========================================================
    print_section("make_pair");

    NAMESPACE::pair<int, std::string> p2 = NAMESPACE::make_pair<int, std::string>(393, "Pengdori");

    std::cout << "p2.first:  " << p2.first << '\n';
    std::cout << "p2.second: " << p2.second << '\n';

    // ===========================================================
    // 4. Relational operators
    // ===========================================================
    print_section("Operators (==, !=, <, <=, >, >=)");

    NAMESPACE::pair<int, int> p3(1, 2);
    NAMESPACE::pair<int, int> p4(3, 4);
    NAMESPACE::pair<int, int> p5(1, 7);
    NAMESPACE::pair<int, int> p3_same(1, 2);

    // == operator
    print_section("== operator");
    print_bool(p3 == p4);
    print_bool(p3 == p3_same);
    std::cout << '\n';

    // != operator
    print_section("!= operator");
    print_bool(p3 != p4);
    print_bool(p3 != p3_same);
    std::cout << '\n';

    // < operator
    print_section("< operator");
    print_bool(p3 < p4);
    print_bool(p4 < p3);
    print_bool(p3 < p5);
    print_bool(p5 < p3);
    print_bool(p3 < p3_same);
    std::cout << '\n';

    // <= operator
    print_section("<= operator");
    print_bool(p3 <= p4);
    print_bool(p4 <= p3);
    print_bool(p3 <= p5);
    print_bool(p5 <= p3);
    print_bool(p3 <= p3_same);
    std::cout << '\n';

    // > operator
    print_section("> operator");
    print_bool(p3 > p4);
    print_bool(p4 > p3);
    print_bool(p3 > p5);
    print_bool(p5 > p3);
    print_bool(p3 > p3_same);
    std::cout << '\n';

    // >= operator
    print_section(">= operator");
    print_bool(p3 >= p4);
    print_bool(p4 >= p3);
    print_bool(p3 >= p5);
    print_bool(p5 >= p3);
    print_bool(p3 >= p3_same);
    std::cout << '\n';
}
