namespace base {
namespace below {

class B : public Base
{
  public:
    B() = default;
    ~B() = default;

    void doSomething() override
    {
      Base::doSomething();
      //base::othernamespace::A any;
      base::othernamespace::func(mInt);
      std::cout << "Derived\nmInt=" << mInt << "\n";
    }


};

}
}

base::below::Base* myFunc()
{
    auto myObject = new base::below::B();
    return myObject;
}
