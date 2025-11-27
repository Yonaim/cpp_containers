#include <functional>
#include <iostream>
#include <algorithm>

/*
    Function Object (functor)
    - operator()를 가진 객체
    - callable(호출 가능 객체)의 일종
        (callable : functor, function pointer, lambda)
    - 상태(state)를 가질 수 있는 함수 객체
    - 함수의 동작 자체를 타입으로 표현할 수 있다 (템플릿 인자, generic programming)

    1) state가 없을 경우 -> 최적화가 극대화.
        - '객체'라고는 하지만, 컴파일 단계에서 사실상 객체가 아니게 됨
        - 즉 empty type이며 런타임 객체는 생성되지 않음
        - 메모리 오버헤드 사실상 0 (1바이트 혹은 EBO에 의해 0바이트)
        - operator() 호출에 대해 inline 가능성 매우 높음
    
    2) state가 있을 경우 -> 최적화 제한
        - 런타임 객체가 생성됨
        - inline 최적화 되더라도 state에 대한 메모리 접근 필요
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
