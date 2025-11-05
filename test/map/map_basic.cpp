#include "test_config.h"
#include "test_print.h"

#ifdef STD_MODE
    #include <map>
    #include <utility>
#else
    #include <map.h>
    #include <pair.h>
#endif

void test_map_basic()
{
    FILE_BANNER();

    // ===========================================================
    // 1. 기본 생성자 / empty / insert
    // ===========================================================
    print_section("Constructor / empty / insert");

    ft::map<int, std::string> m;
    print_bool(m.empty());
    m.insert(ft::make_pair(10, "Ten"));
    m.insert(ft::make_pair(5, "Five"));
    m.insert(ft::make_pair(20, "Twenty"));
    m.insert(ft::make_pair(15, "Fifteen"));
    m.insert(ft::make_pair(30, "Thirty"));

    print_bool(!m.empty());
    print_tree(m, "After insertions");

    // ===========================================================
    // 2. insert (중복 키 테스트)
    // ===========================================================
    print_section("Duplicate key insert");
    ft::pair<typename ft::map<int, std::string>::iterator, bool> ret;
    ret = m.insert(ft::make_pair(10, "Ten (again)"));
    std::cout << "insert(10) success? " << (ret.second ? "true" : "false") << '\n';
    std::cout << "key=10 value: " << ret.first->second << '\n';

    // ===========================================================
    // 3. find() / operator[]
    // ===========================================================
    print_section("find() / operator[]");

    typename ft::map<int, std::string>::iterator it;
    it = m.find(15);
    if (it != m.end())
        std::cout << "find(15): " << it->first << " => " << it->second << '\n';
    else
        std::cout << "find(15): not found\n";

    std::cout << "operator : " << m[20] << '\n';
    std::cout << "operator : " << m[40] << '\n'; // 새 노드 삽입 발생
    print_tree(m, "After operator[] (insert 40)");

    // ===========================================================
    // 4. iterator 순회 (중위 순회)
    // ===========================================================
    print_section("Iterator traversal (in-order)");

    for (typename ft::map<int, std::string>::iterator iter = m.begin(); iter != m.end(); ++iter)
        std::cout << iter->first << " -> " << iter->second << '\n';

    // ===========================================================
    // 5. const_iterator 확인
    // ===========================================================
    print_section("Const iterator check");

    const ft::map<int, std::string> cm(m);
    for (typename ft::map<int, std::string>::const_iterator cit = cm.begin(); cit != cm.end();
         ++cit)
        std::cout << cit->first << ":" << cit->second << " ";
    std::cout << '\n';

    // ===========================================================
    // 6. size / max_size
    // ===========================================================
    print_section("Size / max_size");
    std::cout << "size: " << m.size() << '\n';
    std::cout << "max_size: " << m.max_size() << '\n';

    std::cout << '\n';
}
