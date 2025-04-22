#include "main.hpp"
#include <cstdio>
#include <iostream>
#include <z3++.h>
#include <z3_api.h>

// Create a constructor
void FindDataTypeFields(z3::solver &s, z3::sort data_type) {
  z3::func_decl_vector constructs = data_type.constructors();

  for (unsigned i = 0; i < constructs.size(); ++i) {
    z3::func_decl fn = constructs[i];
    unsigned int fn_args = fn.arity();
    std::cout << "Constructor \033[1;35m" << fn.name() << "\033[0m" << "\n";
    // You can call ctors[i].arity() to get number of fields in that constructor
    AnalyzeFuncDecl(s, fn, fn_args);
  }
}
