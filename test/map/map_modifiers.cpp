#include <iomanip>
#include <iostream>
#include <map>
#include "map.h"
#include "pair.h"
#include "test_config.h"
#include "test_print.h"

void test_map_modifiers()
{
    FILE_BANNER();

    // ===========================================================
    // 1. insert + balancing (간단 검증)
    // ===========================================================
    print_section("Insert & balancing");

    NAMESPACE::map<int, std::string> m;
    m.insert(NAMESPACE::make_pair(20, "Twenty"));
    m.insert(NAMESPACE::make_pair(10, "Ten"));
    m.insert(NAMESPACE::make_pair(30, "Thirty"));
    m.insert(NAMESPACE::make_pair(5, "Five"));
    m.insert(NAMESPACE::make_pair(15, "Fifteen"));
    m.insert(NAMESPACE::make_pair(25, "Twenty-Five"));
    m.insert(NAMESPACE::make_pair(40, "Forty"));
    print_tree(m, "After insertions");

    // ===========================================================
    // 2. erase (단일 노드)
    // ===========================================================
    print_section("Erase single node (iterator)");
    NAMESPACE::map<int, std::string>::iterator it = m.find(15);
    if (it != m.end())
    {
        m.erase(it);
        print_tree(m, "After erase(15)");
    }

    // ===========================================================
    // 3. erase (key overload)
    // ===========================================================
    print_section("Erase by key");
    size_t removed = m.erase(5);
    std::cout << "erase(5) removed count: " << removed << '\n';
    removed = m.erase(100);
    std::cout << "erase(100) removed count: " << removed << " (non-existing)\n";
    print_tree(m, "After erase by key");

    // ===========================================================
    // 4. erase (range)
    // ===========================================================
    print_section("Erase range");
    if (!m.empty())
    {
        NAMESPACE::map<int, std::string>::iterator first = m.begin();
        NAMESPACE::map<int, std::string>::iterator last = m.begin();
        std::advance(last, 2);
        m.erase(first, last);
        print_tree(m, "After erase(first, last)");
    }

    // ===========================================================
    // 5. count / find
    // ===========================================================
    print_section("count() and find()");
    std::cout << "count(20): " << m.count(20) << '\n';
    std::cout << "count(5):  " << m.count(5) << '\n';
    NAMESPACE::map<int, std::string>::iterator f = m.find(25);
    if (f != m.end())
        std::cout << "find(25): " << f->first << " -> " << f->second << '\n';
    else
        std::cout << "find(25): not found\n";

    // ===========================================================
    // 6. swap
    // ===========================================================
    print_section("swap() test");

    NAMESPACE::map<int, std::string> other;
    other.insert(NAMESPACE::make_pair(1, "One"));
    other.insert(NAMESPACE::make_pair(2, "Two"));

    std::cout << "Before swap:\n";
    print_tree(m, "map m");
    print_tree(other, "map other");

    m.swap(other);

    std::cout << "After swap:\n";
    print_tree(m, "map m (after)");
    print_tree(other, "map other (after)");

    // ===========================================================
    // 7. clear
    // ===========================================================
    print_section("clear() test");
    m.clear();
    std::cout << "After clear(): size=" << m.size() << '\n';
    print_bool(m.empty());
}
