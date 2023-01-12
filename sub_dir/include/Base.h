#ifndef BASE_H_
#define BASE_H_

#include "Rtypes.h"

namespace base {
namespace below {

class Base
{
  public:
    Base() = default;
    ~Base() = default;

    virtual void doSomething();

  protected:
    int mInt{};

  ClassDef(Base, 1)
};

}
}

#endif
