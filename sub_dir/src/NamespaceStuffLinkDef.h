#ifdef __CLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class base::othernamespace::A + ;

#pragma link C++ namespace base::othernamespace;
#pragma link C++ function base::othernamespace::func;

#endif
