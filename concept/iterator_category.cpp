/*
iterator 카테고리 (아래로 갈수록 권한이 더 많음)
- input iterator: 한번만 읽기 가능, 단방향 (++it만 됨)
- forward iterator: 여러번 읽기 가능, 단방향 (++it만 됨)
- bidirectional iterator: 여러번 읽기 가능, 양방향(++it, --it 둘다 됨)
- random access iterator: 여러번 읽기 가능, 임의 접근 가능(it + n, it[n], it1 - it2 가능)
*/

#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <vector>

/*
vector의 random access iterator를 다운그레이드???
- iterator category 자체를 변경하는 것은 불가능
- 개념적으로는 낮춰서 사용할 수 있지만...
- 함수 오버로딩이나 enable_if 분기에서 무조건 원래의 카테고리에 들어감
*/
int main()
{
    // ----------------------------------------------------------
    // 1. Input Iterator 예제 : istream_iterator
    // ----------------------------------------------------------
    /*
        - 한 번만 읽는 스트림처럼 동작, 데이터를 한번만 처리하는 경우에 적절 (ex. stream 타입)
        - 여러 개의 독립된 iterator를 병렬로 사용하는 것 불가
        - 동일 요소 다시 읽기 불가 (single-pass)
        - 표준 라이브러리에서는 동일 데이터에 대해 input iterator를 여러 개 생성해 각자 독립적으로
            값에 접근하는 구현은 없음
    */
    {
        std::cout << "[Input Iterator]\n";

        std::istringstream iss("55 66 77");

        std::istream_iterator<int> it_1(iss); // begin (value = 55)
        std::istream_iterator<int> it_2(iss); // 내부 구현에 의해 한번 더 읽힘 (value = 66)
        std::istream_iterator<int> end;       // End-Of-Stream iterator (nullptr)

        /*
        istream_iterator 내부 구현:
        - 단순한 포인터가 아님
        - 내부에 마지막으로 읽었던 데이터가 들어있음
        - 따라서 읽기 실패시에도 value가 남아있어 처리가 가능 (아래 예시)
            - UB이긴 함ㅎㅎ;;
        - it++시
            (1) 값을 읽음
            (2) 한 칸 뒤로 이동
        */

        /*
        // 포인터가 아닌 state machine
        class istream_iterator
        {
            std::istream* _M_stream; // 스트림을 가리킴 (value 읽기 실패시 nullptr로 변환)
            T _M_value;              // 마지막으로 읽은 값
        };
        */

        std::cout << *it_1 << '\n'; // 55
        std::cout << *it_2 << '\n'; // 66
        it_1++;                     // (value = 77)
        std::cout << *it_1 << '\n'; // 77
        // it_1--; // 오직 단방향(++)만 가능, 앞으로 이동(--) 불가

        if (it_1 == end)
            std::cout << "it_1 == end is true (1)" << '\n'; // false

        it_1++; // (value 읽기 실패로 값은 77로 유지, stream = nullptr)
        if (it_1 == end)
            std::cout << "it_1 == end is true (2)" << '\n';
        std::cout << *it_1 << '\n'; // 77 (직전 value) (UB)
        std::cout << '\n';
    }

    // ----------------------------------------------------------
    // 2. Forward Iterator 예제 : forward_list<int>::iterator
    // ----------------------------------------------------------
    /*
        - 여러 개의 독립된 iterator를 병렬로 사용하는 것 가능
        - iterator 복사 가능
        - 동일 요소 다시 읽기 가능 (multiple-pass guarantee)
        - 대표 예시: forward_list -> 단일 연결 리스트 기반 구현 (연속 메모리 아님)
    */
    {
        /*
        forward_list
        - 단일 연결 리스트로 구현된 컨테이너
        - random access는 불가, 오직 단방향 순회만 가능
        - 이중 연결 리스트로 구현된 std::list에 비해 메모리 덜 씀
        - '연결 리스트'로 구현되었기 때문에, iterator invalidation 규칙을 적용할 때:
            한 원소가 삭제되어도 나머지 원소들은 할당된 주소에 그대로 살아있음
            -> 해당 원소들의 이터레이터는 무효화가 되지 않음
        */
        std::cout << "[Forward Iterator]\n";

        std::forward_list<int>           fl = {33, 44, 55, 66};
        std::forward_list<int>::iterator it_1 = fl.begin(); // 33
        std::forward_list<int>::iterator it_2 = it_1;       // 복사 가능
        std::forward_list<int>::iterator it_3 = fl.begin(); // 33

        std::cout << *it_1 << ' ' << *it_2 << '\n'; // 둘 다 33
        std::cout << *it_1 << ' ' << *it_3 << '\n'; // 둘 다 33

        ++it_1; // 이동 가능
        ++it_2; // 이동 가능

        std::cout << *it_1 << ' ' << *it_2 << '\n'; // 둘 다 44
        // it_1--; // 오직 단방향(++)만 가능, 앞으로 이동(--) 불가

        std::cout << '\n';
    }

    // ----------------------------------------------------------
    // 3. Bidirectional Iterator 예제 : list<int>::iterator
    // ----------------------------------------------------------
    /*
    - 대표 예시: list -> 이중 연결 리스트 기반 구현 (연속 메모리 아님)
    */
    {
        std::cout << "[Bidirectional Iterator]\n";

        std::list<int> lst = {11, 22, 33};

        std::list<int>::iterator it = lst.begin();
        std::cout << *it << '\n'; // 11

        ++it;                     // 뒤로 이동 (++)
        std::cout << *it << '\n'; // 22

        --it;                     // 앞으로 이동 (--)
        std::cout << *it << '\n'; // 다시 11

        std::cout << '\n';
    }

    // ----------------------------------------------------------
    // 4. Random Access Iterator 예제 : vector<int>::iterator
    // ----------------------------------------------------------
    /*
        - distance 계산 / random access / 비교 연산을 O(1) 안에 수행하는 것을 보장해야 함
        - 포인터(주소) 기반으로 구현하는 것이 제일 간단
        - 대표 예시: std::vector -> 포인터 기반 (연속 메모리)
    */
    {
        std::cout << "[Random Access Iterator]\n";

        std::vector<int> v;
        for (int i = 0; i < 5; ++i)
            v.push_back(i);
 
        auto it = v.begin();

        std::cout << it[0] << '\n'; // 0
        std::cout << it[3] << '\n'; // 3

        auto it2 = it + 4;
        std::cout << *it2 << '\n'; // 4

        std::cout << (it2 - it) << '\n'; // 4 (거리 계산 가능!)

        std::cout << '\n';
    }
}