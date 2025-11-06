#ifndef TYPE_NAME_STRING_H
#define TYPE_NAME_STRING_H

// ========================= Type name strings =================================

// implement using partial specialization!

// type_name_string
template <class T>
struct type_name_string
{
    static std::string value() { return "unkonwn"; }
};

// preprocessor for register specific type name 'T'
// just for avoid duplicated codes...
#define REGISTER_TYPE_NAME(T)                                                                      \
    template <>                                                                                    \
    struct type_name_string<T>                                                                     \
    {                                                                                              \
        static std::string value() { return #T; }                                                  \
    };

// === 기본 타입 등록 ===
REGISTER_TYPE_NAME(void);
REGISTER_TYPE_NAME(bool);
REGISTER_TYPE_NAME(char);
REGISTER_TYPE_NAME(unsigned char);
REGISTER_TYPE_NAME(short);
REGISTER_TYPE_NAME(unsigned short);
REGISTER_TYPE_NAME(int);
REGISTER_TYPE_NAME(unsigned int);
REGISTER_TYPE_NAME(long);
REGISTER_TYPE_NAME(unsigned long);
REGISTER_TYPE_NAME(long long);
REGISTER_TYPE_NAME(unsigned long long);
REGISTER_TYPE_NAME(float);
REGISTER_TYPE_NAME(double);
REGISTER_TYPE_NAME(long double);

// === 포인터 및 참조 타입 등록 ===
template <class T>
struct type_name_string<T *>
{
    static std::string value() { return type_name_string<T>::value() + "*"; }
};

template <class T>
struct type_name_string<const T *>
{
    static std::string value() { return "const " + type_name_string<T>::value() + "*"; }
};

template <class T>
struct type_name_string<T &>
{
    static std::string value() { return type_name_string<T>::value() + "&"; }
};

template <class T>
struct type_name_string<const T &>
{
    static std::string value() { return "const " + type_name_string<T>::value() + "&"; }
};

// === 표준 iterator category 태그 등록 ===
REGISTER_TYPE_NAME(std::input_iterator_tag);
REGISTER_TYPE_NAME(std::output_iterator_tag);
REGISTER_TYPE_NAME(std::forward_iterator_tag);
REGISTER_TYPE_NAME(std::bidirectional_iterator_tag);
REGISTER_TYPE_NAME(std::random_access_iterator_tag);

#endif
