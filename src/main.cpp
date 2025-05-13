#include "main.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <print>
#include <vector>
#include <z3++.h>
#include <z3_api.h>

std::string AnalyzeConstDecl(z3::solver &s, z3::func_decl const_find) {
  // Find its type
  z3::sort const_type = const_find.range();
  // Find sat values for the const
  return "test";
}

std::vector<std::string> AnalyzeModel(z3::solver &s, z3::model m) {
  std::vector<std::string> found_values;

  unsigned n_const = m.num_consts();
  printf("--------\033[1;35mTotal Constants: %d\033[0m-----------\n", n_const);
  for (int i = 0; i < n_const; i++) {
    z3::func_decl curr_const = m.get_const_decl(i);
    std::string found_val = AnalyzeConstDecl(s, curr_const);
  }

  unsigned n_func = m.num_funcs();
  printf("--------\033[1;35mTotal Functions: %d\033[0m-----------\n", n_func);
  for (int i = 0; i < n_func; i++) {
    z3::func_decl fn = m.get_func_decl(i);
    unsigned int fn_args = fn.arity();

    std::cout << i << ":\t" << fn << "\n";
    std::string found_val = AnalyzeFuncDecl(s, fn, fn_args);
    found_values.push_back(found_val);
  }

  return found_values;
};

int main(int argc, char **argv) {
  const char *filename = argv[1];
  if (filename == NULL) {
    printf("USAGE: %s <FILENAME>\n", argv[0]);
    exit(1);
  }

  // Injecting SMTfile to Z3 API.
  z3::context c;
  z3::expr_vector expressions = c.parse_file(filename);
  z3::solver s(c);
  s.add(expressions);

  std::vector<std::string> found_values;

  if (s.check() == z3::sat) {
    printf("--------Formula:\033[1;34mSAT\033[0m-------------\n");
    z3::model m = s.get_model();
    printf("--------\033[1;35mMODEL\033[0m-----------\n");
    std::cout << m << "\n";
    found_values = AnalyzeModel(s, m);
  } else {
    printf("--------Formula:\033[1;31m UNSAT\033[0m-------------\n");
  }
}
