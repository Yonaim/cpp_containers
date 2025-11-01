#include <iostream>
#include <string>
#include <utility>
#include "SomeTypes.h"
#include "pair.h"
#include "test_config.h"
#include "utils/print.h"


// generic function
template <class P>
static void member_type_check(P &parm)
{
    typename P::first_type  first = parm.first;
    typename P::second_type second = parm.second;

    std::cout << first << '\n';
    std::cout << second << '\n';
}

void test_pair()
{
    print_title("generic programming");

    NAMESPACE::pair<int, std::string> p0;
    p0.first = 42;
    p0.second = "Hello world";
    member_type_check(p0);

    print_title("constructor");

    NAMESPACE::pair<SomeType1, SomeType2> p1;
    SomeType1                             some_1(123);
    SomeType2                             some_2(456);
    p1.first = some_1;
    p1.second = some_2;
    NAMESPACE::pair<SomeType1, SomeType2> p1_copy(p1);
    std::cout << "p1_copy.first: " << p1_copy.first.value << '\n';
    std::cout << "p1_copy.second: " << p1_copy.second.value << '\n';

    print_title("make_pair");

    NAMESPACE::pair<int, std::string> p2 = NAMESPACE::make_pair<int, std::string>(393, "Pengdori");
    std::cout << "first: " << p2.first << '\n';

    print_title("operators (==, !=, <, <=, >, >=)");

    NAMESPACE::pair<int, int> p3(1, 2);
    NAMESPACE::pair<int, int> p4(3, 4);
    NAMESPACE::pair<int, int> p5(1, 7);
    NAMESPACE::pair<int, int> p3_same(1, 2);

    // ==
    std::cout << "p3 == p4: " << (p3 == p4 ? "true" : "false") << '\n';
    std::cout << "p3 == p3_same: " << (p3 == p3_same ? "true" : "false") << '\n';
    std::cout << '\n';

    // !=
    std::cout << "p3 != p4: " << (p3 != p4 ? "true" : "false") << '\n';
    std::cout << "p3 != p3_same: " << (p3 != p3_same ? "true" : "false") << '\n';
    std::cout << '\n';

    // <
    std::cout << "p3 < p4: " << (p3 < p4 ? "true" : "false") << '\n';
    std::cout << "p4 < p3: " << (p4 < p3 ? "true" : "false") << '\n';
    std::cout << "p3 < p5: " << (p3 < p5 ? "true" : "false") << '\n';
    std::cout << "p5 < p3: " << (p5 < p3 ? "true" : "false") << '\n';
    std::cout << "p3 < p3_same: " << (p3 < p3_same ? "true" : "false") << '\n';
    std::cout << '\n';

    // <=
    std::cout << "p3 <= p4: " << (p3 <= p4 ? "true" : "false") << '\n';
    std::cout << "p4 <= p3: " << (p4 <= p3 ? "true" : "false") << '\n';
    std::cout << "p3 <= p5: " << (p3 <= p5 ? "true" : "false") << '\n';
    std::cout << "p5 <= p3: " << (p5 <= p3 ? "true" : "false") << '\n';
    std::cout << "p3 <= p3_same: " << (p3 <= p3_same ? "true" : "false") << '\n';
    std::cout << '\n';

    // >
    std::cout << "p3 > p4: " << (p3 > p4 ? "true" : "false") << '\n';
    std::cout << "p4 > p3: " << (p4 > p3 ? "true" : "false") << '\n';
    std::cout << "p3 > p5: " << (p3 > p5 ? "true" : "false") << '\n';
    std::cout << "p5 > p3: " << (p5 > p3 ? "true" : "false") << '\n';
    std::cout << "p3 > p3_same: " << (p3 > p3_same ? "true" : "false") << '\n';
    std::cout << '\n';

    // >=
    std::cout << "p3 >= p4: " << (p3 >= p4 ? "true" : "false") << '\n';
    std::cout << "p4 >= p3: " << (p4 >= p3 ? "true" : "false") << '\n';
    std::cout << "p3 >= p5: " << (p3 >= p5 ? "true" : "false") << '\n';
    std::cout << "p5 >= p3: " << (p5 >= p3 ? "true" : "false") << '\n';
    std::cout << "p3 >= p3_same: " << (p3 >= p3_same ? "true" : "false") << '\n';
    std::cout << '\n';
}
