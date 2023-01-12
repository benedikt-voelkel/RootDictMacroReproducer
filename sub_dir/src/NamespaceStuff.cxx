#include <iostream>

#include "NamespaceStuff.h"

namespace base {
namespace othernamespace {

void func(int& i)
{
    i++;
}

void A::bar(std::string const& s)
{
    std::cout << s << "\n";
}

int A::foo(int i, int j)
{
    return i + j;
}

}
}

