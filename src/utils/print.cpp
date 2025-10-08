#include <iostream>

void print_title(const char *title)
{
    std::cout << "\n============ " << title << " ============\n\n";
}

void print_bool(bool b)
{
    std::cout << (b ? "true" : "false") << '\n';
}
