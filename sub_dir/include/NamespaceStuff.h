#ifndef NAMESPACE_STUFF_H
#define NAMESPACE_STUFF_H

#include <string>

#include "Rtypes.h"

namespace base {
namespace othernamespace {

class A
{
    public:
        A() = default;
        ~A() = default;
        int foo(int i, int j);
        void bar(std::string const& s);

    private:
        std::string mMember;

    ClassDefNV(A, 1)
};

void func(int& i);

}
}

#endif
