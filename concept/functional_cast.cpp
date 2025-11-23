#include <iostream>
using namespace std;

/*
    casting 방식 3가지

    1) C-style cast (C 스타일 캐스트)
    - 형식: (Type)ptr
    - 강제로 형변환하는 강력한 캐스팅

    2) functional cast (함수형 캐스트)
    - 형식: Type(ptr)
    - 형식만 보면 생성자 호출 같기도한데 생성자 호출 유무는 Type따라 다름
        - 클래스 타입일 경우 생성자 호출
        - 포인터 타입일 경우 포인터 캐스팅 시도 (static -> reinterpret순)
        - 그 외 타입일시 static_cast와 동일

    3) C++ explicit cast (명시적 캐스트)
    - static_cast
    - reinterpret_cast
    - const_cast
    - dynamic_cast
*/

/*
    STL에서 functional cast를 사용하는 이유?
    - reinterpret_cast보다 가독성이 나음
    - C 스타일보다 안전
*/

struct Base
{
    int base_value = 10;
};

struct Derived : public Base
{
    int derived_value = 20;
};

// Derived 포인터 자체를 새로운 타입으로 define
// Functional cast에서 사용할 수 있다
typedef Derived *Derived_ptr;

int main()
{
    // 목적: Base 포인터 -> Derived 포인터로 캐스팅
    // 객체 자체는 Derived 타입
    Base *p0 = new Derived();

    // 1) C-style cast (C 스타일 캐스트)
    Derived *p1 = (Derived *)p0;

    // 2) functional cast (함수형 캐스트)

    // functional cast의 형식은 맞지만, 생성자 호출이 되므로 틀림 (해당되는 생성자 존재하지 않음)
    // Derived *p2 = Derived(p0);

    // Derived *p2 = Derived * (base_ptr); // 틀린 문법

    // 별도로 정의한 Derived_ptr 타입을 이용한 캐스팅
    Derived *p3 = Derived_ptr(p0);

    // 3) C++ explicit cast (명시적 캐스트)
    Derived *p4 = static_cast<Derived *>(p0); // 안전한 경우에만 가능
    Derived *p5 = reinterpret_cast<Derived *>(p0);

    delete p0;
    return 0;
}
