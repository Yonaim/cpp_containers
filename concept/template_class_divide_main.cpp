#include "template_class_divide.h"

int main()
{
    MyBox<int> box(10);
    box.print(); // 출력: MyBox value = 10

    box.set(42);
    box.print(); // 출력: MyBox value = 42

    MyBox<std::string> sbox("Hello");
    sbox.print(); // 출력: MyBox value = Hello

    return 0;
}
