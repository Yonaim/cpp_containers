#include <iostream>
#include <map>
#include <string>
#include "map.h"
#include "pair.h"
#include "test_config.h"
#include "test_print.h"


void test_map_lookup_compare()
{
    FILE_BANNER();

    // ===========================================================
    // 1. 기본 데이터 구성
    // ===========================================================
    print_section("Setup data");

    NAMESPACE::map<int, std::string> m;
    m.insert(NAMESPACE::make_pair(10, "Ten"));
    m.insert(NAMESPACE::make_pair(20, "Twenty"));
    m.insert(NAMESPACE::make_pair(30, "Thirty"));
    m.insert(NAMESPACE::make_pair(40, "Forty"));
    m.insert(NAMESPACE::make_pair(50, "Fifty"));

    for (NAMESPACE::map<int, std::string>::iterator it = m.begin(); it != m.end(); ++it)
        std::cout << "(" << it->first << ", " << it->second << ") ";
    std::cout << "\n\n";

    // ===========================================================
    // 2. lower_bound / upper_bound
    // ===========================================================
    print_section("lower_bound / upper_bound");

    typedef NAMESPACE::map<int, std::string>::iterator it_t;
    it_t                                               low = m.lower_bound(25);
    it_t                                               up = m.upper_bound(30);

    print_it("lower_bound(25)", low, m.end());
    print_it("upper_bound(30)", up, m.end());

    it_t low_eq = m.lower_bound(10);
    it_t up_eq = m.upper_bound(10);
    print_it("lower_bound(10)", low_eq, m.end());
    print_it("upper_bound(10)", up_eq, m.end());

    // ===========================================================
    // 3. equal_range
    // ===========================================================
    print_section("equal_range");

    NAMESPACE::pair<it_t, it_t> range = m.equal_range(30);
    print_it("equal_range.first (30)", range.first, m.end());
    print_it("equal_range.second (30)", range.second, m.end());

    // ===========================================================
    // 4. key_compare / value_compare
    // ===========================================================
    print_section("key_compare / value_compare");

    NAMESPACE::map<int, std::string>::key_compare key_comp = m.key_comp();
    std::cout << std::boolalpha;
    std::cout << "key_comp(20, 30): " << key_comp(20, 30) << '\n';
    std::cout << "key_comp(30, 20): " << key_comp(30, 20) << '\n';

    NAMESPACE::map<int, std::string>::value_compare val_comp = m.value_comp();
    NAMESPACE::pair<const int, std::string>         v1(10, "Ten");
    NAMESPACE::pair<const int, std::string>         v2(40, "Forty");
    std::cout << "val_comp((10,Ten),(40,Forty)): " << val_comp(v1, v2) << '\n';
    std::cout << "val_comp((40,Forty),(10,Ten)): " << val_comp(v2, v1) << '\n';

    // ===========================================================
    // 5. 비교 연산자
    // ===========================================================
    print_section("Comparison operators (==, !=, <, <=, >, >=)");

    NAMESPACE::map<int, std::string> m_same(m);
    NAMESPACE::map<int, std::string> m_diff(m);
    m_diff.erase(40);
    m_diff.insert(NAMESPACE::make_pair(60, "Sixty"));

    print_bool(m == m_same);
    print_bool(m != m_diff);
    print_bool(m < m_diff);
    print_bool(m_diff > m);
    print_bool(m <= m_same);
    print_bool(m_diff >= m_same);

    // ===========================================================
    // 6. 경계 상황 테스트
    // ===========================================================
    print_section("Edge cases (out of bound)");

    it_t it1 = m.lower_bound(100);
    it_t it2 = m.upper_bound(100);
    print_it("lower_bound(100)", it1, m.end());
    print_it("upper_bound(100)", it2, m.end());

    it_t it3 = m.lower_bound(1);
    it_t it4 = m.upper_bound(1);
    print_it("lower_bound(1)", it3, m.end());
    print_it("upper_bound(1)", it4, m.end());
}
