#ifndef FT_BOOL_TYPE_H
#define FT_BOOL_TYPE_H

// type trait: 타입의 특성을 알려주는 구조체
// constexpr을 사용해 런타임이 아닌 컴파일 타임에서 평가하게끔 한다.
// ... 근데 c++98에서는 constexpr가 없어서 static const로 흉내.

namespace ft
{
    struct true_type
    {
        static const bool value = true;
    };
    
    struct false_type
    {
        static const bool value = false;
    };
} // namespace ft

#endif
