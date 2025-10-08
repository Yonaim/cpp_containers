#include <iostream>

using namespace std;

struct SomeType1
{
    int value;
    SomeType1() { cout << "SomeType1: Default constructor" << '\n'; }
    SomeType1(int v) : value(v)
    {
        cout << "SomeType1: Value constructor, " << "value: " << value << '\n';
    }
    SomeType1(const SomeType1 &orig) : value(orig.value)
    {
        cout << "SomeType1: Copy constructor" << '\n';
    }
};

struct SomeType2
{
    int value;
    SomeType2() { cout << "SomeType2: Default constructor" << '\n'; }
    SomeType2(int v) : value(v)
    {
        cout << "SomeType2: Value constructor, " << "value: " << value << '\n';
    }
    SomeType2(const SomeType2 &orig) : value(orig.value)
    {
        cout << "SomeType2: Copy constructor" << '\n';
    }
};
