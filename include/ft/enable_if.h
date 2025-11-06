#ifndef FT_ENABLE_IF
#define FT_ENABLE_IF

/*
template <bool B, class T = void>
struct enable_if<B, T>
- 멤버 타입(alias member)로 'type'을 가지며, 두번째 인자인 T의 alias이다.
- 첫번째 인자 B가 true일 경우 type이 T가 된다.
- 첫번째 인자 B가 false일 경우 type은 존재하지 않는다. (후보에서 탈락)
- 리턴형, 매개변수, 템플릿 인자에서 모두 활용 가능
- T의 기본 값은 void (c++11부터)
*/

// partial specialization을 이용해 구현한다.

namespace ft
{
    template <bool B, class T>
    struct enable_if
    {
    };

    template <class T>
    struct enable_if<true, T>
    {
        typedef T type;
    };
} // namespace ft

#endif
