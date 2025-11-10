/*
conversion constructor:
- 오직 1개의 인자를 받는 생성자 (default 인자 제외)
- 기본적으로 implicit conversion에 의한 호출이 가능함
- 만약 explicit 키워드를 붙인다면, implicit conversion을 막을 수 있음
*/
#include <iostream>

// ============================ explicit keyword ===============================

struct MyType1
{
    // conversion constructor (implicit OK)
    MyType1(int x) { std::cout << "MyType1 constructor" << std::endl; }
};

struct MyType2
{
    // conversion constructor (implicit NO, only explicit)
    explicit MyType2(int x) { std::cout << "MyType2 constructor" << std::endl; }
};

// implicit conversion이 발동되는 함수 (암묵적 타입 변환을 수행)
void need_to_my_type_1(MyType1 p) { return; }
void need_to_my_type_2(MyType2 p) { return; }

// ============= contiguous implicit conversion experiment =====================

struct MyType3
{
    // conversion constructor
    MyType3(int x) { std::cout << "MyType3 (x) constructor" << std::endl; }
    // no-conversion constructor (Non single-argument)
    MyType3(int x, int y) { std::cout << "MyType3 (x,y) constructor" << std::endl; }
};

struct MyType4
{
    // conversion constructor
    MyType4(int x) { std::cout << "MyType4 (x) constructor" << std::endl; }
    // no-conversion constructor (Non single-argument)
    MyType4(int x, int y) { std::cout << "MyType4 (x,y) constructor" << std::endl; }
};

void need_to_my_type_3_4(MyType3 t1, MyType4 t2) { return; }

// ================================= main ======================================

int main()
{
    // explicit keyword
    need_to_my_type_1(42);
    // need_to_my_type_2(42); // implicit conversion impossible
    need_to_my_type_2(MyType2(42)); // only explicit possible

    // contiguous implicit conversion experiment
    need_to_my_type_3_4(1, 2);
    // need_to_my_type_3_4(1, 2, 3);
    // need_to_my_type_3_4(1, 2, 3, 4);
    return 0;
}
