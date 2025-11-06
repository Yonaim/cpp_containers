#ifndef TEST_PRINT_H
#define TEST_PRINT_H

#include <iostream>
#include <string>

#define FILE_BANNER() print_file_banner(__FILE__)
#define PRINT_WIDTH 60

inline void print_module_banner(const std::string &module_name)
{
    std::string label = " MODULE: " + module_name + " ";
    size_t      total = PRINT_WIDTH;
    size_t      len = label.length();

    // 폭보다 긴 경우 잘라서 출력
    if (len > total - 4)
        label = label.substr(0, total - 7) + "... ";

    size_t padding = (total - label.length()) / 2;

    std::cout << "\n\n"
              << std::string(total, '=') << "\n"
              << std::string(padding, '=') << label << std::string(padding, '=') << "\n"
              << std::string(total, '=') << "\n\n";
}

inline void print_file_banner(const std::string &filepath)
{
    std::string filename = filepath;
    size_t      last_slash = filename.find_last_of("/\\");
    if (last_slash != std::string::npos)
        filename = filename.substr(last_slash + 1);

    size_t dot = filename.find_last_of('.');
    if (dot != std::string::npos)
        filename = filename.substr(0, dot);

    std::cout << "\n" << std::string(PRINT_WIDTH, '=') << "\n";
    std::cout << ">>> TEST FILE: " << filename << "\n";
    std::cout << std::string(PRINT_WIDTH, '=') << "\n\n";
}

inline void print_section(const char *title)
{
    std::string t(title);
    size_t      total = PRINT_WIDTH;

    // 최대 길이 제한 (너무 긴 제목은 자르기)
    if (t.length() > total - 8)
        t = t.substr(0, total - 8) + "...";

    size_t padding = (total - t.length() - 4) / 2; // [ ] 포함 여백

    std::cout << '\n'
              << std::string(padding, '-') << " [" << t << "] " << std::string(padding, '-')
              << "\n\n";
}

inline void print_bool(bool b) { std::cout << (b ? "true" : "false") << "\n"; }

// Simple tree printer for debugging RBT structure
template <typename Map>
void print_tree(const Map &m, const std::string &title = "Tree")
{
    print_section(title.c_str());
    typedef typename Map::const_iterator const_it;
    for (const_it it = m.begin(); it != m.end(); ++it)
        std::cout << "(" << it->first << ", " << it->second << ") ";
    std::cout << '\n';
}

template <typename Iterator>
void print_it(const std::string &label, Iterator it, Iterator end)
{
    std::cout << label << ": ";
    if (it == end)
        std::cout << "(end)\n";
    else
        std::cout << "(" << it->first << ", " << it->second << ")\n";
}

#endif
