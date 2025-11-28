#ifndef TEMPLATE_CLASS_DIVIDE_IMPL_H
#define TEMPLATE_CLASS_DIVIDE_IMPL_H

// 어차피 template_class.h의 맨 마지막에 include 될거니까 전방 선언 안 해도 상관없다.

// ---- 구현 시작 ----

template <class T>
MyBox<T>::MyBox(const T &v) : value(v)
{
}

template <class T>
const T &MyBox<T>::get() const
{
    return value;
}

template <class T>
void MyBox<T>::set(const T &v)
{
    value = v;
}

template <class T>
void MyBox<T>::print() const
{
    std::cout << "MyBox value = " << value << std::endl;
}

#endif
