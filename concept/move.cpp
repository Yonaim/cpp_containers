/*
    - move: C++11부터 도입
    - RVO와 move의 구분
        - RVO: 복사, 이동 생성자 연산 자체를 없앰
        - move: RVO 실패시, 복사 대신 이동 수행
*/

/*
    목적:
    1) RVO / NRVO 성공 시 : copy / move 모두 안 뜸
    2) NRVO 실패 시 : move가 copy보다 우선적으로 선택되는 걸 확인
*/

#include <iostream>
#include <string>

struct Tracer
{
    std::string tag;

    Tracer(const char *t) : tag(t) { std::cout << "Construct: " << tag << "\n"; }

    // copy 생성자 (출력)
    Tracer(const Tracer &other) : tag(other.tag) { std::cout << "Copy: " << tag << "\n"; }

    // move 생성자 (출력)
    Tracer(Tracer &&other) noexcept : tag(other.tag) { std::cout << "Move: " << tag << "\n"; }

    ~Tracer() { std::cout << "Destruct: " << tag << "\n"; }
};

// ------------------ 1. RVO ------------------
Tracer make_rvo() { return Tracer("RVO_TEMP"); }

// ------------------ 2. NRVO ------------------
Tracer make_nrvo()
{
    Tracer t("NRVO_LOCAL");
    return t;
}

// ------------------ 3. NRVO 실패 (분기) ------------------
Tracer make_nrvo_fail(bool cond)
{
    Tracer a("NRVO_FAIL_A");
    Tracer b("NRVO_FAIL_B");

    if (cond)
        return a; // NRVO 불가 → move가 copy보다 우선
    else
        return b;
}

// ------------------ 4. 괄호 → NRVO 실패 ------------------
Tracer make_nrvo_paren()
{
    Tracer x("NRVO_PAREN");
    return (x); // NRVO 불가 → move 발생
}

int main()
{

    std::cout << "\n========= RVO (no copy/move) =========\n\n";
    Tracer r1 = make_rvo();

    std::cout << "\n========= NRVO (no copy/move) =========\n\n";
    Tracer r2 = make_nrvo();

    std::cout << "\n========= NRVO FAIL (branch) =========\n\n";
    Tracer r3 = make_nrvo_fail(true);

    std::cout << "\n========= NRVO FAIL (parentheses) =========\n\n";
    Tracer r4 = make_nrvo_paren();

    std::cout << "\n============ End ============\n\n";
}
