#ifndef PAIR_H
#define PAIR_H

namespace ft
{
    // template<class T1>, template<typename T1>
    // C++98에서는 둘다 가능! 원래는 전자만 됐음.
    template <class T1, class T2>
    struct pair
    {
        // 템플릿 파라미터는 외부에서 접근할 수가 없으므로, 외부에서 접근할 수 있도록 typedef
        // 템플릿 파라미터는 템플릿 정의 내에서만 유효한 타입 이름
        typedef T1 first_type;
        typedef T2 second_type;

        T1 first;
        T2 second;

        // ============ constructor =============
        // rvalue reference, move, forward : C+11부터
        // initialize list를 활용 (생성자 직접 호출)
        pair() // T1, T2의 default contructor들이 호출됨
        {
        }
        pair(const pair<T1, T2> &p) : first(p.first), second(p.second)
        // T1, T2의 copy contructor들이 호출됨
        {
        }
        pair(const T1 &x, const T2 &y) : first(x), second(y)
        // T1, T2의 copy contructor들이 호출됨
        {
        }
        // ============ copy assignment operator =============
        pair &operator=(const pair &other)
        {
            first = other.first;   // copy assignment operator 호출
            second = other.second; // copy assignment operator 호출
            return *this;
        }
        // ============ conversion constructor =============
        template <class U1, class U2>
        pair(const pair<U1, U2> &p) : first(p.first), second(p.second)
        {
        }
        // ============ conversion assignment operator =============
        template <class U1, class U2>
        pair &operator=(const pair<U1, U2> &other)
        {
            first = other.first;
            second = other.second;
            return *this;
        }
        // ============ destructor =============
        ~pair() // T1, T2의 default destructor들이 호출됨
        {
        }
    };

    template <class T1, class T2>
    pair<T1, T2> make_pair(T1 x, T2 y)
    {
        return pair<T1, T2>(x, y);
    }

    // ============ operators (==, !=, <, <=, >, >=) =============
    template <class T1, class T2, class U1, class U2>
    bool operator==(const pair<T1, T2> &lhs, const pair<U1, U2> &rhs)
    {
        return (lhs.first == rhs.first && lhs.second == rhs.second);
    }
    template <class T1, class T2, class U1, class U2>
    bool operator!=(const pair<T1, T2> &lhs, const pair<U1, U2> &rhs)
    {
        return (lhs.first != rhs.first || lhs.second != rhs.second);
    }

    // lexicographical(사전식) 비교
    template <class T1, class T2, class U1, class U2>
    bool operator<(const pair<T1, T2> &lhs, const pair<U1, U2> &rhs)
    {
        if (lhs.first < rhs.first)
            return true;
        else if (lhs.first > rhs.first)
            return false;
        else
            return (lhs.second < rhs.second);
    }
    template <class T1, class T2, class U1, class U2>
    bool operator<=(const pair<T1, T2> &lhs, const pair<U1, U2> &rhs)
    {
        if (lhs.first < rhs.first)
            return true;
        else if (lhs.first > rhs.first)
            return false;
        else
            return (lhs.second <= rhs.second);
    }
    template <class T1, class T2, class U1, class U2>
    bool operator>(const pair<T1, T2> &lhs, const pair<U1, U2> &rhs)
    {
        if (lhs.first > rhs.first)
            return true;
        else if (lhs.first < rhs.first)
            return false;
        else
            return (lhs.second > rhs.second);
    }
    template <class T1, class T2, class U1, class U2>
    bool operator>=(const pair<T1, T2> &lhs, const pair<U1, U2> &rhs)
    {
        if (lhs.first > rhs.first)
            return true;
        else if (lhs.first < rhs.first)
            return false;
        else
            return (lhs.second >= rhs.second);
    }
} // namespace ft

#endif
