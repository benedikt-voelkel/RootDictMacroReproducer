// In order to reproduce, taken from https://github.com/AliceO2Group/AliceO2/blob/dev/Common/Utils/include/CommonUtils/ConfigurationMacroHelper.h#L47

#ifndef MACRO_HELPER_H
#define MACRO_HELPER_H

#include <iostream>

#include "TROOT.h"
#include "TSystem.h"
#include "TGlobal.h"
#include "TFunction.h"
#include <string>


template <typename T>
T GetFromMacro(const std::string& file, const std::string& funcname, const std::string& type, const std::string& unique)
{

  /** tweak the string to get the required global function **/
  auto func = funcname;
  if (func.empty()) {
    auto size = file.size();
    auto firstindex = file.find_last_of("/") + 1;
    auto lastindex = file.find_last_of(".");
    func = file.substr(firstindex < size ? firstindex : 0,
                       lastindex < size ? lastindex - firstindex : size - firstindex) +
           "()";
  }
  auto gfunc = func.substr(0, func.find_first_of('('));

  /** load macro is global function is not already defined **/
  if (!gROOT->GetGlobalFunction(gfunc.c_str())) {
    if (gROOT->LoadMacro(file.c_str()) != 0) {
      std::cerr << "Cannot find " << file;
      return nullptr;
    }
    if (!gROOT->GetGlobalFunction(gfunc.c_str())) {
      std::cerr << "Global function '" << gfunc << "' not defined";
      return nullptr;
    }
  }

  /** check the return type matches the required one **/
  if (strcmp(gROOT->GetGlobalFunction(gfunc.c_str())->GetReturnTypeName(), type.c_str())) {
    std::cout << "Global function '" << gfunc << "' does not return a '" << type << "' type";
    return nullptr;
  }

  /** process function and retrieve pointer to the returned type **/
  gROOT->ProcessLine(Form("%s __%s__ = %s;", type.c_str(), unique.c_str(), func.c_str()));
  auto ptr = (T*)gROOT->GetGlobal(Form("__%s__", unique.c_str()))->GetAddress();


  /** success **/
  return *ptr;
}

#endif
