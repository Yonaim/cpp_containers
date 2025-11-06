#include <iostream>
#include <string>

// primary template (base, 꼭 있어야 함)
template <class A, class B>
struct myPair
{
    void print() { std::cout << "primary template (fallback)" << std::endl; }
};

// full specialization
template <>
struct myPair<int, std::string>
{
    void print() { std::cout << "full specialization" << std::endl; }
};

// spatial specialization 1
template <class A>
struct myPair<A, int>
{
    void print() { std::cout << "spatial specialization 1" << std::endl; }
};

// spatial specialization 2
template <class A>
struct myPair<A, A *>
{
    void print() { std::cout << "spatial specialization 2" << std::endl; }
};

int main()
{
    myPair<int, std::string> f0;
    f0.print();

    myPair<int, int> s1;
    s1.print();

    myPair<int, int *> s2;
    s2.print();

    myPair<int, double> p0;
    p0.print();
}
