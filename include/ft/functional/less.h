#ifndef FT_LESS_H
#define FT_LESS_H

/*
    왜 static 함수가 아니라 굳이 functor 객체로 만들어서 호출하게 하는가?
    -> 함수 포인터로는 불가능하고 객체 타입 기반으로만 가능한 것들이 있다.

    1) 객체는 state를 가질 수 있다. (함수 포인터는 상태를 가질 수 없다.)
    2) 템플릿 인자에 functor 타입을 넘길 수 있다. (함수 포인터는 템플릿 인자에 넣을 수 없다.)
    3) 함수 포인터 vs 객체 타입의 최적화
        3-1) 함수포인터는 dispatch시 런타임 바인딩을 한다.
            그러나 함수 객체는 컴파일 타임에 바인딩이 이루어진다.
        3-2) fuctor는 inline 최적화가 가능하다. 객체 내부 구조가 이미 결정되어있기 때문.

    위와 같은 이유로, STL 컨테이너는 policy를 '템플릿 타입'으로 받는다.
    (static 함수는 '값', functor는 '타입')
*/

#include "binary_function.h"

namespace ft
{
    template <class T>
    struct less : public binary_function<T, T, bool>
    {
        /*
            C++98 STL functor 대부분은 binary_function이 제공하는 typedef를 그대로 사용한다.
            따라서 functor 내부에 typedef를 직접 작성할 필요는 없다.
        */
        // typedef T    first_argument_type;
        // typedef T    second_argument_type;
        // typedef bool result_type;

        bool operator()(const T &lhs, const T &rhs) const { return lhs < rhs; }
    };
} // namespace ft

#endif
