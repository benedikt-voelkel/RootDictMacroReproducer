int* myFunc()
{
    //namespaceA::namespaceB::A object;
    hello();
    auto i = new int(5);
    std::cout << *i << "\n";
    namespaceA::namespaceB::func(*i);
    std::cout << *i << "\n";
    return i;
}
