#include <iostream>

#include "MacroHelper.h"
#include "Base.h"

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "Need exactly 2 arguments\n";
        return 1;
    }
    auto obj = GetFromMacro<base::below::Base*>(argv[1], argv[2], "base::below::Base*", "obj");
    std::cout << obj << "\n";
    obj->doSomething();
    return 0;
}
