
/*
Implicit conversion
- 암묵적으로 타입을 자동 변환
- 변환 대상 타입에 따라 built-in 그리고 user-defined로 구분
- user-defined 암묵적 변환은 explicit 키워드로 막아둘 수 있다 (클래스 선언 스코프 내부 함수만 가능)
- built-in 암묵적 변환은 막을 방법이 없다
*/

#include <iostream>

// ========================= built-in conversion ===============================

void need_to_int(int p)
{
    std::cout << __func__ << ": " << p << std::endl;
    return;
}

/*
정의 자체가 안된다!
explicit 키워드는 class declaration 내부에서만 사용 가능
즉,
*/
// explicit void need_to_explicit_int(int p)
// {
//     std::cout << __func__ << ": " << p << std::endl;
//     return;
// }

// ======================= user-defined conversion =============================

struct MyType1
{
    MyType1(int x) { std::cout << "MyType1 constructor" << std::endl; }
};

struct MyType2
{
    explicit MyType2(int x) { std::cout << "MyType2 constructor" << std::endl; }
};

// implicit conversion이 발동되는 함수
void need_to_my_type_1(MyType1 p) { return; }
void need_to_my_type_2(MyType2 p) { return; }

// ================================= main ======================================

int main()
{
    // implicit conversion (built-in)
    need_to_int(3.14);
    // need_to_explicit_int(3.14);

    // implicit conversion (user-defined)
    need_to_my_type_1(42);
    // need_to_my_type_2(42);
    return 0;
}
