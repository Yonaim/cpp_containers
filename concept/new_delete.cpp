#include <iostream>

/*
    표현식 new/delete와 함수 new/delete를 구분한다!!!
    - 표현식 new/delete: 메모리 확보 및 해제 + 생성자/소멸자 호출
    - 함수 new/delete: raw 메모리의 할당/해제 (C++스타일의 malloc, free)
*/

// new/delete = 키워드(표현식 전용)
// operator new/delete = 함수명(직접 호출 가능)

/*
    1) new T / delete p (표현식)

    new T
    - 메모리 확보 + 생성자 호출을 묶어서 함
    - 내부적으로 operator new를 찾아서 호출(dynamic dispatch)한 다음 생성자를 호출함

    delete p
    - 소멸자 호출 + 메모리 해제를 묶어서 함

    2) T::operator new/delete vs ::operator new/delete (함수)

    ::operator new(n)
    - 전역 할당 함수
    - 메모리만 확보(생성자 X)

    T::operator new(n)
    - 클래스 전용 할당 함수
    - 메모리만 확보(생성자 X)
*/

/*
    operator new/delete 함수 시그니처는 무조건 아래와 동일해야 함
    - void operator delete(void *p)
    - void *operator new(size_t n)
    - n은 바이트 수를 의미 (객체 개수가 아님)
*/

/*
    소멸자는 명시적 호출이 가능하지만, 생성자는 명시적으로 호출할 수 없다.
    그 대신 사용하는거 -> placement new (위치 지정 new)

    placement new (위치 지정 new): new 키워드를 사용할 때 메모리주소를 같이 명시
    - ex) new (mem) myType;
    - 메모리는 sizeof(myType)만큼 미리 확보가 되어있어야 한다.
    - 메모리 관리와 객체 수명 관리를 분리할 수 있음!!!
    - 큰 raw memory 풀을 쪼개 사용하여 효율 향상 가능
*/

using namespace std;

struct myType
{
    int some1;
    int some2;

    myType() { cout << "myType constructor\n"; }
    ~myType() { cout << "myType destructor\n"; }

    void operator delete(void *p)
    {
        cout << "myType::operator delete\n";
        ::operator delete(p);
    }

    // 오직 메모리 할당만 하고 생성자 호출은 하지 않음
    void *operator new(size_t n)
    {
        cout << "myType::operator new " << n << "\n";
        return ::operator new(n);
    }

    // placement new용 ::operator new
    // 실질적으로 아무것도 하지 않고 주소를 그대로 반환
    void *operator new(size_t n, void *place)
    {
        cout << "myType::operator new (placement) " << n << "\n";
        return place;
    }
};

// 다양한 객체, 생성 파괴를 실험
int main()
{
    cout << "\n============== Create t1 ===============\n";
    // 메모리 할당 + 생성자 호출을 묶어서 수행
    // 메모리 할당 함수(::operator new)는 dynamic dispatch로 호출
    myType *t1 = new myType;

    cout << "\n============== Create t2 ===============\n";
    // 메모리 할당 + 생성자 호출을 별도로 수행
    myType *t2 = (myType *)::operator new(sizeof(myType)); // 메모리 할당
    t2 = new (t2) myType;
    // 단순 예제이며, 일반적으로 이렇게 사용하지 않음 (new 표현식에 비해 득이 없음)

    cout << "\n============== Create t3, t4 ===============\n";

    void *mem = myType::operator new(sizeof(myType) * 2); // t3, t4를 위한 메모리 확보
    // 메모리 할당과 생성자 호출을 별도로 수행
    // 연속된 메모리에 placement new를 수행
    myType *first = static_cast<myType *>(mem);
    myType *t3 = new (first) myType;
    myType *t4 = new (first + 1) myType;

    cout << "\n============== Destroy (automatic) ===============\n";
    // 소멸자 호출 + 메모리 해제를 묶어서 수행
    delete t1;

    cout << "\n============== Destroy (manually) ===============\n";
    // 소멸자 호출 + 메모리 해제를 별도로 수행
    t2->~myType();
    ::operator delete(t2); // myType::operator delete가 아닌 전역으로 직접 지정

    return 0;
}
