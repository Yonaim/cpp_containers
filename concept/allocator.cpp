#include <iostream>
#include <memory>

/*
The std::allocator class template is the default Allocator used by all standard library containers
if no user-specified allocator is provided.
*/

/*
- custom allocator는 std::allocator를 상속해서 선언하는게 아님
- 필수 allocator API만 갖추면 된다
    - typedef
    - 생성자
    - allocate / deallocate
    - construct / destroy
    - rebind
*/

int main()
{
    const int           n_objects = 5;
    std::allocator<int> alloc;

    // ===============================
    // 1) int 용 메모리/생성/파괴
    // ===============================
    int *p = alloc.allocate(n_objects);

    for (int i = 0; i < n_objects; i++)
        alloc.construct(p + i, i + 1);

    for (int i = 0; i < n_objects; i++)
        std::cout << p[i] << ' ';
    std::cout << '\n';

    for (int i = 0; i < n_objects; i++)
        alloc.destroy(p + i);

    alloc.deallocate(p, n_objects);

    // ===============================
    // 2) rebind 예제
    // ===============================
    /*
        C++98 스타일:
            std::allocator<int>::rebind<double>::other alloc_double;

        C++11 이후 allocator_traits 권장 방식:
            using alloc_double = std::allocator_traits<std::allocator<int>>::rebind_alloc<double>;
    */

    // rebind를 이용하여 기존 policy를 유지하여 새로운 allocator 타입 선언
    typedef std::allocator<int>::rebind<double>::other rebind_alloc_double;
    rebind_alloc_double alloc_d;

    // double 객체 3개를 위한 메모리 확보
    double* q = alloc_d.allocate(3);

    // 생성
    alloc_d.construct(q + 0, 3.14);
    alloc_d.construct(q + 1, 2.71);
    alloc_d.construct(q + 2, 1.414);

    std::cout << "rebind results: ";
    for (int i = 0; i < 3; i++)
        std::cout << q[i] << ' ';
    std::cout << '\n';

    // 파괴
    for (int i = 0; i < 3; i++)
        alloc_d.destroy(q + i);

    // 반납
    alloc_d.deallocate(q, 3);

    return 0;
}
