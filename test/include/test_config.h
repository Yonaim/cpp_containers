#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

#ifndef NAMESPACE
#define NAMESPACE ft
#endif

#if NANESPACE == std
    namespace ft = std;
#endif

// 전처리기는 한 단계 치환만 수행하기 때문에 이렇게 단계를 거쳐 수행을 원하는 경우 단계별로 매크로를 둬야함
// 안 두면 그냥 "NAMESPACE"가 된다.
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define NAMESPACE_STR STR(NAMESPACE)

#endif // TEST_CONFIG_H
