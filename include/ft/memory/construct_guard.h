#ifndef FT_CONSTRUCT_GUARD_H
#define FT_CONSTRUCT_GUARD_H

/*
    exception-safe (예외 안전):
    처리 중간에 예외가 발생해도 상태가 안전하게 유지되는 것

    보통 3단계로 나뉨.
    1) Basic guarantee: 예외가 발생해도 객체는 valid (내용은 변했을 수 있음)
    2) Strong guarantee: commit or rollback (예외 발생시 연산이 아예 수행하지 않은 상태 그대로 유지)
    3) No-throw guarantee: 절대 예외를 던지지 않음 (혹은 내부에서 처리하여 밖으로 안 나감)
*/

/*
    C++ 예외 규칙:
    예외(exception) 발생시,
    스택 언와인딩 과정에서 현재 스코프에 있는 모든 자동 객체(=automatic object)의
    소멸자가 호출된다.
    (자동 객체 = 스택 수명 객체:
        스코프에 진입할 때 자동으로 생성되고 블록이 종료되면 자동으로 파괴되는 객체)
*/

/*
    - 보통 자동 객체(automatic object)로 선언하여 사용한다. 즉 스택 프레임의 일부
    - 컴파일러의 최적화 작업을 거치고 나면 스택 프레임에서조차 메모리를 차지하지 않음
        - text 영역 (instruction 리스트)에도 존재하지 않는다
        - 대신 EH 테이블(Exception Table)에 읽기 전용 메타데이터로 들어간다.
    - 예외 발생시에는 EH 테이블을 참조하여 스택 언와인딩과 함께 처리한다.
        - 예외 발생시엔 비용이 비쌈
        - 그러나 try-catch 방식도 예외 발생시 처리 비용이 비싼 것은 마찬가지
*/

/*
    - RAII 기반 Rollback 패턴 코드
        - exception 처리 로직에서 누락되지 않게 묶는 역할
    - 정상적으로 construct된 횟수를 기록한다.
    - 예외 발생시 이 값을 이용해 적절한 횟수의 destroy를 수행할 수 있다.(=롤백)
    - 롤백 처리는 소멸자에서 한다.
        - C++의 예외 규칙을 이용
        - 예외가 발생하지 않고 정상 로직을 타면 release() 호출에 의해
            count=0이 되어 소멸자에서는 아무 작업도 하지 않는다.
*/

namespace ft
{
    template <class _Alloc>
    struct construct_guard
    {
        // allcator 타입에서 value_type을 꺼내와 사용
        typedef typename _Alloc::value_type value_type;

        _Alloc     &alloc; // stateful일 수 있기 때문에 참조로 들고있어야한다
        size_t      count;
        value_type *start;

        construct_guard(_Alloc &a, value_type *s) : alloc(a), count(0), start(s) {}
        ~construct_guard()
        {
            for (size_t i = 0; i < count; ++i)
                alloc.destroy(start + i);
        }

        // 정상 construct 횟수 카운트 증가
        void bump() { ++count; }
        void bump_n(size_t n) { count += n; }

        // 정상 처리 완료
        void release() { count = 0; }
    };
} // namespace ft

#endif
