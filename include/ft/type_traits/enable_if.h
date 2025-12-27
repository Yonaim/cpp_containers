#ifndef FT_ENABLE_IF
#define FT_ENABLE_IF

/*
    "SFINAE (Substitution failure is not an error)를 위한 도구"
    "enable_if<T>::type이 존재하느냐/없느냐로 후보가 살아남거나 죽음"

    - B가 true이면 "type"이라는 이름의 typedef이 존재하고 false면 존재하지 않음
        (partial template specialization을 이용)
    - 보통 B에 참/거짓 평가식을 두고 스코프의 ::type에 접근 시도하여 평가식의 참을 강제
    - 평가식이 거짓이면 진행할 수 없다. 그래서 이름이 "enable, if ..."
*/

/*
    - SFINAE는 일반적인 하드 에러(hard error)와는 달리 조용히 넘어간다
    - enable_if 자체는 SFINAE를 위한 도구이지만 bool 평가식에서 하드 에러가 발생할 수 있음
*/

namespace ft
{

    /*
        - 멤버 타입(alias member)로 'type'을 가지며, 두번째 인자인 T의 alias이다.
        - 첫번째 인자 B가 true일 경우 type이 T가 된다.
        - 첫번째 인자 B가 false일 경우 type은 존재하지 않는다. (후보에서 탈락)
        - 리턴형, 매개변수, 템플릿 인자에서 모두 활용 가능
        - T의 기본 값은 void (c++11부터)
    */
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
