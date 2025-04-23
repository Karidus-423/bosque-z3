#include "main.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <z3++.h>
#include <z3_api.h>

void AnalyzeModel(z3::solver &s, z3::model m) {
  unsigned n_const = m.num_consts();
  printf("--------\033[1;35mTotal Constants: %d\033[0m-----------\n", n_const);
  for (int i = 0; i < n_const; i++) {
    std::cout << i << ":\t" << m.get_const_decl(i) << "\n";
  }

  unsigned n_func = m.num_funcs();
  printf("--------\033[1;35mTotal Functions: %d\033[0m-----------\n", n_func);
  for (int i = 0; i < n_func; i++) {
    z3::func_decl fn = m.get_func_decl(i);
    unsigned int fn_args = fn.arity();

    std::cout << i << ":\t" << fn << "\n";
    AnalyzeFuncDecl(s, fn, fn_args);
  }
};

int main(int argc, char **argv) {
  const char *filename = argv[1];
  if (filename == NULL) {
    printf("USAGE: %s <FILENAME>\n", argv[0]);
    exit(1);
  }

  z3::context c;
  z3::expr_vector expressions = c.parse_file(filename);
  z3::solver s(c);
  s.add(expressions);

  if (s.check() == z3::sat) {
    printf("--------\033[1;34mSAT\033[0m-------------\n");
    z3::model m = s.get_model();
    printf("--------\033[1;35mMODEL\033[0m-----------\n");
    std::cout << m << "\n";
    AnalyzeModel(s, m);
  } else {
    printf("UNSAT\n");
  }
}
