#include "main.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <z3++.h>
#include <z3_api.h>

void FindAssertion(z3::solver &s, Z3_sort_kind type, z3::func_decl fn,
                   z3::sort sort_type) {
  printf("Type: ");
  switch (type) {
  case Z3_UNINTERPRETED_SORT:
    printf("Z3_UNINTERPRETED_SORT\n");
    break;
  case Z3_BOOL_SORT:
    printf("Z3_BOOL_SORT\n");
    break;
  case Z3_INT_SORT:
    printf("Z3_INT_SORT\n");
    FindIntAssertion(s, fn, sort_type);
    break;
  case Z3_REAL_SORT:
    printf("Z3_REAL_SORT\n");
    break;
  case Z3_BV_SORT:
    printf("Z3_BV_SORT\n");
    break;
  case Z3_ARRAY_SORT:
    printf("Z3_ARRAY_SORT\n");
    break;
  case Z3_DATATYPE_SORT:
    printf("Z3_DATATYPE_SORT\n");
    FindDataTypeFields(s, sort_type);
    break;
  case Z3_RELATION_SORT:
    printf("Z3_RELATION_SORT\n");
    break;
  case Z3_FINITE_DOMAIN_SORT:
    printf("Z3_FINITE_DOMAIN_SORT\n");
    break;
  case Z3_FLOATING_POINT_SORT:
    printf("Z3_FLOATING_POINT_SORT\n");
    break;
  case Z3_ROUNDING_MODE_SORT:
    printf("Z3_ROUNDING_MODE_SORT\n");
    break;
  case Z3_SEQ_SORT:
    printf("Z3_SEQ_SORT\n");
    break;
  case Z3_RE_SORT:
    printf("Z3_RE_SORT\n");
    break;
  case Z3_CHAR_SORT:
    printf("Z3_CHAR_SORT\n");
    break;
  case Z3_TYPE_VAR:
    printf("Z3_TYPE_VAR\n");
    break;
  case Z3_UNKNOWN_SORT:
    printf("Z3_UNKNOWN_SORT\n");
    break;
  }
}

void AnalyzeFuncDecl(z3::solver &s, z3::func_decl fn, unsigned int fn_args) {
  std::cout << "\033[1;32m" << fn.name() << "\033[0m\n";
  printf("\033[4;33mDomain Arguments: %d\033[0m\n", fn_args);
  for (int j = 0; j < fn_args; j++) {
    Z3_sort_kind type = fn.domain(j).sort_kind();
    FindAssertion(s, type, fn, fn.domain(j));
  }
}

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
