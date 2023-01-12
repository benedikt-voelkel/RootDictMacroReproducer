#include <iostream>

#include "MacroHelper.h"

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "Need exactly 2 arguments\n";
        return 1;
    }
    auto obj = GetFromMacro<int*>(argv[1], argv[2], "int*", "obj");
    std::cout << obj << "\n";
    return 0;
}
