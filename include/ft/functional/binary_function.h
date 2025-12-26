// Reference: https://en.cppreference.com/w/cpp/utility/functional/binary_function

#ifndef FT_BINARY_FUNCTION_H
#define FT_BINARY_FUNCTION_H

 /*
    - 이 클래스를 상속받은 클래스들은 Arg1, Arg2, Result typename을 갖다쓸 수 있음
    - 이 클래스를 상속받은 타입은 함수 객체 타입일 것으로 기대 (operator() 구현)
    - 대표적으로 equal_to가 이 클래스를 상속받는다
 */
namespace ft
{
    template <class Arg1, class Arg2, class Result>
    struct binary_function
    {
        typedef Arg1   first_argument_type;
        typedef Arg2   second_argument_type;
        typedef Result result_type;
    };
} // namespace ft

#endif
