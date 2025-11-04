#include <iostream>
#include <stack> // for std::stack comparison
#include "pair.h"
#include "stack.h"
#include "test_config.h"
#include "test_print.h"
#include "vector.h"


void test_stack()
{
    FILE_BANNER();

    // ===========================================================
    // 1. 기본 생성자 / empty / size
    // ===========================================================
    print_section("Constructor / empty / size");

    NAMESPACE::stack<int> s;
    print_bool(s.empty());
    std::cout << "Initial size: " << s.size() << '\n';

    // ===========================================================
    // 2. push / top / pop
    // ===========================================================
    print_section("push / top / pop");

    for (int i = 1; i <= 5; ++i)
    {
        s.push(i * 10);
        std::cout << "push(" << i * 10 << ") → top = " << s.top() << ", size = " << s.size()
                  << '\n';
    }

    print_section("After push()");
    print_bool(!s.empty());
    std::cout << "Top element: " << s.top() << '\n';

    s.pop();
    std::cout << "After pop(): top = " << s.top() << ", size = " << s.size() << '\n';

    // ===========================================================
    // 3. 여러 자료형 테스트
    // ===========================================================
    print_section("Stack of strings");

    NAMESPACE::stack<std::string> s_str;
    s_str.push("Konkuk");
    s_str.push("University");
    s_str.push("CSE");

    std::cout << "Top: " << s_str.top() << '\n';
    s_str.pop();
    std::cout << "After pop(): top = " << s_str.top() << ", size = " << s_str.size() << '\n';

    // ===========================================================
    // 4. 비교 연산자
    // ===========================================================
    print_section("Comparison operators (==, !=, <, <=, >, >=)");

    NAMESPACE::stack<int> s1;
    NAMESPACE::stack<int> s2;
    for (int i = 0; i < 3; ++i)
    {
        s1.push(i);
        s2.push(i);
    }

    print_bool(s1 == s2);
    s2.push(99);
    print_bool(s1 != s2);
    print_bool(s1 < s2);
    print_bool(s2 > s1);
    print_bool(s1 <= s2);
    print_bool(s2 >= s1);

    // ===========================================================
    // 5. std::stack과의 동작 비교
    // ===========================================================
    print_section("Compare behavior with std::stack");

    std::stack<int>       std_s;
    NAMESPACE::stack<int> ft_s;

    for (int i = 1; i <= 3; ++i)
    {
        std_s.push(i);
        ft_s.push(i);
    }

    std::cout << "std::stack top: " << std_s.top() << '\n';
    std::cout << "ft::stack  top: " << ft_s.top() << '\n';
    print_bool(std_s.size() == ft_s.size());
}
