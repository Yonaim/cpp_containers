// test for pair

#include <iostream>
#include <string>
#include <utility>
#include "SomeTypes.h"
#include "pair.h"
#include "utils/print.h"

// generic function
template <class P> static void member_type_check(P &parm)
{
    // 컴파일러 입장에서는 P::first_type이 타입 네임인지 멤버 변수인지 알 수 없으므로
    // typename 키워드로 명시해준다
    // 빌드 과정 중 - 컴파일 단계에서 처리됨.
    typename P::first_type  first = parm.first;
    typename P::second_type second = parm.second;

    std::cout << first << '\n';
    std::cout << second << '\n';
}

void test_pair()
{
    // ============ generic programming ============
    print_title("generic programming");

    std::pair<int, std::string> p0;
    p0.first = 42;
    p0.second = "Hello world";
    member_type_check(p0);

    // ============ constructor ============
    print_title("constructor");

    std::pair<SomeType1, SomeType2> p1;
    SomeType1                       some_1(123);
    SomeType2                       some_2(456);
    p1.first = some_1;
    p1.second = some_2;
    std::pair<SomeType1, SomeType2> p1_copy(p1);
    std::cout << "p1_copy.first: " << p1_copy.first.value << '\n';
    std::cout << "p1_copy.second: " << p1_copy.second.value << '\n';

    // ============ make_pair ============
    print_title("make_pair");

    std::pair<int, std::string> p2 = std::make_pair<int, std::string>(393, "Pengdori");
    // C++17부터는 deduce 가능
    // std::pair<int, std::string> p3 = std::make_pair(393, "Pengdori");
    std::cout << "first: " << p2.first << '\n';

    // ============ operators (==, !=, <, <=, >, >=) =============
    print_title("operators (==, !=, <, <=, >, >=)");

    std::pair<int, int> p3(1, 2);
    std::pair<int, int> p4(3, 4); // first one is higher than p3
    std::pair<int, int> p5(1, 7); // second one is higher than p3
    std::pair<int, int> p3_same(1, 2);

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
