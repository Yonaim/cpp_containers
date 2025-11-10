#include <iostream>
#include <memory>

int main()
{
    const int           n_objects = 5;
    std::allocator<int> alloc;

    // 메모리 확보
    int *p = alloc.allocate(n_objects);

    // 객체 생성자 호출
    for (int i = 0; i < n_objects; i++)
        alloc.construct(p + i, i + 1);

    // 객체 값 확인용 출력
    for (int i = 0; i < n_objects; i++)
        std::cout << p[i] << ' ';
    std::cout << '\n';

    // 객체 소멸자 호출
    for (int i = 0; i < n_objects; i++)
        alloc.destroy(p + i);

    // 메모리 반납 (free)
    alloc.deallocate(p, 5);
    return 0;
}