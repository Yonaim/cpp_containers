#ifndef FT_IS_CLASS_H
#define FT_IS_CLASS_H

// class 혹은 struct일시 value = true
namespace ft
{
    // 바이트 수로 yes/no 구분 (바이트 수에 큰 의미는 없음, 단지 구분용)
    typedef char yes_type; // 1바이트
    struct no_type         // 2바이트
    {
        char dummy[2];
    };

    template <class T>
    class is_class
    {
      private:
        // 만약 T가 class 혹은 struct라면 내부 멤버에 접근하려고 시도할 것임
        // static 함수인 이유? -> static이어야 객체를 생성하려고 하지 않음
        static yes_type check(int T::*); // T 내부 멤버 중 int를 가리키는 포인터
        static no_type  check(...);

      public:
        // sizeof()의 피연산자는 evaluation되지 않음
        // 따라서 check()가 호출될 일은 없다 -> 선언만 하고 구현 안 해도됨
        enum
        {
            value = (sizeof(check(0)) == sizeof(yes_type))
        };
    };
} // namespace ft

#endif
