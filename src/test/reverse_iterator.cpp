#include <iostream>
#include <iterator>
#include <vector>
#include "reverse_iterator.h"
#include "test_config.h"
#include "type_name_string.h"

// =========================== Print Reverse Iterator Traits ===========================

template <class Iter>
void print_reverse_iterator_info(const std::string &name)
{
    typedef NAMESPACE::reverse_iterator<Iter>          riter;
    typedef typename NAMESPACE::iterator_traits<riter> traits;

    std::cout << "====== " << name << " ======\n";
    std::cout << "base type:           " << type_name_string<Iter>::value() << '\n';
    std::cout << "value_type:          " << type_name_string<typename traits::value_type>::value()
              << '\n';
    std::cout << "difference_type:     "
              << type_name_string<typename traits::difference_type>::value() << '\n';
    std::cout << "pointer:             " << type_name_string<typename traits::pointer>::value()
              << '\n';
    std::cout << "reference:           " << type_name_string<typename traits::reference>::value()
              << '\n';
    std::cout << "iterator_category:   "
              << type_name_string<typename traits::iterator_category>::value() << "\n\n";
}

// ============================== Behavior Test ========================================

void test_reverse_iterator_behavior()
{
    std::vector<int> v;
    for (int i = 1; i <= 5; ++i)
        v.push_back(i); // [1,2,3,4,5]

    typedef std::vector<int>::iterator   it_type;
    NAMESPACE::reverse_iterator<it_type> rbegin(v.end());
    NAMESPACE::reverse_iterator<it_type> rend(v.begin());

    std::cout << "Container: [1, 2, 3, 4, 5]\n";
    std::cout << "Reverse iteration: ";
    for (NAMESPACE::reverse_iterator<it_type> it = rbegin; it != rend; ++it)
        std::cout << *it << " ";
    std::cout << "\n\n";

    // 테스트: ++, --, base(), operator*, operator->
    NAMESPACE::reverse_iterator<it_type> rit(v.end());
    std::cout << "Initial (*rit):      " << *rit << '\n';
    ++rit;
    std::cout << "After ++rit:         " << *rit << '\n';
    ++rit;
    std::cout << "After ++rit:         " << *rit << '\n';
    --rit;
    std::cout << "After --rit:         " << *rit << '\n';
    std::cout << "rit.base() points to value: " << *(rit.base() - 1) << "\n\n";
}

// ============================== Entry Function =======================================

void test_reverse_iterator()
{
    print_reverse_iterator_info<typename std::vector<int>::iterator>("vector<int>::iterator");
    print_reverse_iterator_info<typename std::vector<int>::const_iterator>(
        "vector<int>::const_iterator");
    print_reverse_iterator_info<int *>("int*");
    print_reverse_iterator_info<const int *>("const int*");

    test_reverse_iterator_behavior();
}
