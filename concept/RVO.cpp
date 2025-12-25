/*
    RVO (Return Value Optimization)
    : callee가 리턴할 값을 callee의 스택 메모리가 아니라
      함수 스코프 밖의 "caller의 Return slot"에 직접 생성하는 컴파일러 최적화.

    1) 임시 객체 반환 → RVO
    2) 이름 있는 지역 변수 반환 → Named RVO (NRVO)

    RVO/NRVO 모두 copy 생성자를 "아예 호출하지 않는다".
    만약 copy가 보이면 -> RVO 실패한 것임.
*/

/*
    반환할 때 괄호로 감싸서 반환하면 RVO 적용이 안됨!!!!
    (*근데 이론상으로만 그렇고 실제로는 컴파일러가 처리해주게 구현되어있다)

    - RVO의 규칙은 엄격하다. "컴파일러가 임의로 하는 위험한 최적화"에서 출발했기 때문
        - 극도로 보수적인 규칙: "이름 있는 지역 변수 그대로 반환하는 경우만 최적화 어용"
                            즉, id-expression만 허용
        - 컴파일러의 표현식 문법 구조상, 괄호를 씌우면 parenthesized expression가 됨
        - 따라서 괄호를 씌우면 RVO가 적용되지 않는다.
    - 과거 C++ 표준은 괄호에 대한 identity 추론을 허용하지 않았는데 이게 현재까지 유지됨
    - 괄호 자체가 의미를 바꾸지는 않지만 copy 생략 규칙은 '문법 형태 기반 규칙'이기 때문에 이렇게 됨
*/

#include <iostream>
#include <string>

struct Tracer
{
    std::string tag;

    Tracer(const char *t) : tag(t) { std::cout << "Construct: " << tag << "\n"; }

    Tracer(const Tracer &other) : tag(other.tag) { std::cout << "Copy: " << tag << "\n"; }

    ~Tracer() { std::cout << "Destruct: " << tag << "\n"; }
};

// 1. RVO: temporary 반환
Tracer make_rvo() { return Tracer("RVO_TEMP"); }

// 2. NRVO: 이름 있는 지역 변수 반환
Tracer make_nrvo()
{
    Tracer t("NRVO_LOCAL");
    return t;
}

// 3. NRVO 실패 (분기)
Tracer make_nrvo_fail(bool cond)
{
    Tracer a("NRVO_FAIL_A");
    Tracer b("NRVO_FAIL_B");

    if (cond)
        return a; // NRVO 불가 → copy 발생
    else
        return b;
}

// 4. 괄호로 NRVO 깨짐
Tracer make_nrvo_paren()
{
    Tracer x("NRVO_PAREN");
    return (x); // NRVO 불가 → copy 발생
}

int main()
{
    std::cout << "\n========= RVO =========\n\n";
    Tracer r1 = make_rvo();

    std::cout << "\n========= NRVO =========\n\n";
    Tracer r2 = make_nrvo();

    std::cout << "\n========= NRVO FAIL (branch) =========\n\n";
    Tracer r3 = make_nrvo_fail(true);

    std::cout << "\n========= NRVO FAIL (parentheses) =========\n\n";
    Tracer r4 = make_nrvo_paren();

    std::cout << "\n============ End ============\n\n";
}
