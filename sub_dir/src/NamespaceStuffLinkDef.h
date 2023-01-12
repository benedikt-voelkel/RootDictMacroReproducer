#ifdef __ROOTCLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class namespaceA::namespaceB::A + ;

#pragma link C++ namespace namespaceA::namespaceB;
#pragma link C++ function namespaceA::namespaceB::func;

#pragma link C++ function hello;

#endif
