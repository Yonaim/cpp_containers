#include <iostream>

/*
using 키워드의 역할

1) using-directive & using-declaration
2) type alias (C++11부터 추가)
*/

namespace my_scope_1
{
    void print_hello() { std::cout << "hello" << std::endl; }
} // namespace my_scope_1

namespace my_scope_2
{
    void print_world() { std::cout << "world" << std::endl; }
    void print_bark() { std::cout << "bark!!!!" << std::endl; }
} // namespace my_scope_2

int main()
{
    // using-directive
    using namespace my_scope_1;
    print_hello();

    // using-declaration
    using my_scope_2::print_bark;
    // print_world(); // error
    print_bark();
}
