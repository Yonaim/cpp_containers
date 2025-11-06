// ============================================================================
//  Provided by Ecole42 (Modified version with safe macro guards and error logs)
// ============================================================================

#include <deque>
#include <iostream>
#include <string>

#ifdef STD_MDOE
    #include <map>
    #include <stack>
    #include <vector>
    namespace ft = std;
#else
    #include "map.h"
    #include "stack.h"
    #include "vector.h"
#endif

#include <stdlib.h>

#define MAX_RAM 4294967296 // 4GB
#define BUFFER_SIZE 4096

struct Buffer
{
    int  idx;
    char buff[BUFFER_SIZE];
};

#define COUNT (MAX_RAM / (int)sizeof(Buffer)) // about one million

// ====================== TEST FLAG DEFINITIONS ====================== //
#ifndef TEST_VECTOR
    #define TEST_VECTOR 0
#endif

#ifndef TEST_MAP
    #define TEST_MAP 0
#endif

#ifndef TEST_STACK
    #define TEST_STACK 0
#endif

// ====================== MUTANT STACK ====================== //
#if TEST_STACK
template <typename T>
class MutantStack : public ft::stack<T>
{
  public:
    MutantStack() {}
    MutantStack(const MutantStack<T> &src) { *this = src; }
    MutantStack<T> &operator=(const MutantStack<T> &rhs)
    {
        this->c = rhs.c;
        return *this;
    }
    ~MutantStack() {}

    typedef typename ft::stack<T>::container_type::iterator iterator;

    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }
};
#endif // TEST_STACK

// ====================== MAIN ====================== //
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./test <seed>" << std::endl;
        std::cerr << "Provide a seed please" << std::endl;
        std::cerr << "Count value: " << COUNT << std::endl;
        return 1;
    }

    const int seed = atoi(argv[1]);
    srand(seed);

#if TEST_VECTOR
    std::cout << "[RUNNING TEST_VECTOR]" << std::endl;

    ft::vector<std::string>               vector_str;
    ft::vector<int>                       vector_int;
    ft::vector<Buffer>                    vector_buffer;
    ft::stack<Buffer, std::deque<Buffer>> stack_deq_buffer;

    for (int i = 0; i < COUNT; i++)
        vector_buffer.push_back(Buffer());

    for (int i = 0; i < COUNT; i++)
    {
        const int idx = rand() % COUNT;
        vector_buffer[idx].idx = 5;
    }
    ft::vector<Buffer>().swap(vector_buffer);

    try
    {
        for (int i = 0; i < COUNT; i++)
        {
            const int idx = rand() % COUNT;
            vector_buffer.at(idx);
            std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught expected exception: " << e.what() << std::endl;
    }
#endif // TEST_VECTOR

#if TEST_MAP
    std::cout << "[RUNNING TEST_MAP]" << std::endl;

    ft::map<int, int> map_int;
    for (int i = 0; i < COUNT; ++i)
        map_int.insert(ft::make_pair(rand(), rand()));

    int sum = 0;
    for (int i = 0; i < 10000; i++)
    {
        int access = rand();
        sum += map_int[access];
    }
    std::cout << "should be constant with the same seed: " << sum << std::endl;

    {
        ft::map<int, int> copy = map_int;
        (void)copy;
    }
#endif // TEST_MAP

#if TEST_STACK
    std::cout << "[RUNNING TEST_STACK]" << std::endl;

    MutantStack<char> iterable_stack;
    for (char letter = 'a'; letter <= 'z'; letter++)
        iterable_stack.push(letter);

    for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); ++it)
        std::cout << *it;

    std::cout << std::endl;
#endif // TEST_STACK

    return 0;
}
