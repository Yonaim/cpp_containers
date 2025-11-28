#ifndef TEMPLATE_CLASS_DIVIDE_H
#define TEMPLATE_CLASS_DIVIDE_H

#include <iostream>

template <class T>
class MyBox
{
  private:
    T value;

  public:
    MyBox(const T &v);        // 생성자 선언
    const T &get() const;     // getter 선언
    void     set(const T &v); // setter 선언
    void     print() const;   // print 함수 선언
};

// 반드시 선언부 뒤에 둔다!
// 구현부가 여기에서 include 되어야 한다
#include "template_class_divide_impl.h"

#endif
