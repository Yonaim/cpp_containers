#include <functional>
#include <iostream>
#include <algorithm>

/*
    Function Object type (함수 객체 타입)
    - operator()를 오버로딩한 클래스
    - callable(호출 가능 객체)의 일종이다.
        - callable : functor, function pointer, lambda
*/

/*
    왜 굳이 함수 객체 타입의 객체를 생성해서 operator()를 호출하는가?
    static 함수면 되는거 아냐?

    1) static 함수는 state를 가질 수 없다.
    2) 템플릿 인자에 함수 포인터를 전달할 수가 없다. 타입을 넘겨야한다.

*/

// functor1
struct Ssimple
{
    // operator()를 오버로딩 -> function object type이 됨
    void operator()(int num) { std::cout << "number : " << num << std::endl; }
    void operator()() { std::cout << "Ssimple void" << std::endl; }
};

// functor2
struct Scomplex
{
    int sum;
    Scomplex() { sum = 0; }
    void operator()(int num)
    {
        sum += num;
        std::cout << "sum : " << sum << std::endl;
    }
};

int main()
{
    Ssimple simple;
    simple(42);
    // 이렇게 객체를 생성하고 호출하는 것도 가능
    Ssimple()(5);
    Ssimple()();

    Scomplex complex;
    complex(42);   // 누적: 42
    complex(7);    // 누적: 49
    Scomplex()(1); // 누적: 1

    std::vector<int> vec{1, 2, 3, 4};

    std::for_each(vec.begin(), vec.end(), Ssimple());  // functor1
    std::for_each(vec.begin(), vec.end(), Scomplex()); // functor2

    Scomplex complex2;

    // complex2를 내부에서 복사하기 때문에 sum의 값은 그대로임
    std::for_each(vec.begin(), vec.end(), complex2);
    std::cout << "Now sum is " << complex2.sum << "\n\n";

    // std::ref()를 사용하면 레퍼런스로 넘길 수 있다. (원래의 sum 값이 바뀜)
    std::for_each(vec.begin(), vec.end(), std::ref(complex2));
    std::cout << "Now sum is " << complex2.sum << "\n\n";

    // 혹은 for_each가 계산 결과의 복사본을 반환한다는 점을 이용한다
    Scomplex complex3 = std::for_each(vec.begin(), vec.end(), complex2);
    std::cout << "Complex 3's sum is " << complex3.sum << "\n\n";
}
