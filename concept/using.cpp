#include <iostream>

/*
using 키워드의 역할

    1) using-directive & using-declaration
    2) type alias (C++11부터 추가)
*/

/*
    using-declaration(멤버 재노출):
        기반 클래스(Base)에 존재하는 멤버를 현재 클래스의 scope로 끌어옴
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

// ===============================
// class 접근: using 으로 재노출 예시
// ===============================

class Base
{
  public:
    void pub() { std::cout << "Base::pub (public)" << std::endl; }
    void pub2() { std::cout << "Base::pub2 (public)" << std::endl; }

  protected:
    void prot() { std::cout << "Base::prot (protected)" << std::endl; }
    void prot2() { std::cout << "Base::prot2 (protected)" << std::endl; }

  private:
    void priv() { std::cout << "Base::priv (private)" << std::endl; }
};

/*
    클래스 내부에서 using 사용시,
    using이 사용된 위치의 접근 지정자에 따라 승격(promote) 혹은 하향(demote)를 시도
*/
class Derived : public Base
{
  public:
    // prot()를 public으로 승격
    using Base::prot;

    // public 그대로 public
    using Base::pub;

    // NOTE: private 멤버는 절대 가져올 수 없음!!!
    // using을 사용하든 뭐든 아무튼 안됨
    // using Base::priv; // error!

  protected:
    // pub2()를 protected로 하향
    using Base::pub2;

  private:
    // prot2()를 private로 하향
    using Base::prot2;
};

int main()
{
    // using-directive
    using namespace my_scope_1;
    print_hello();

    // using-declaration
    using my_scope_2::print_bark;
    // print_world(); // error
    print_bark();

    // =============== class using (접근 제어 변경) ===============

    Derived d;

    d.pub();  // 원래 public → public
    d.prot(); // 원래 protected → using으로 public으로 승격됨

    // d.priv(); // error: private은 상속 + using 모두 불가
}
