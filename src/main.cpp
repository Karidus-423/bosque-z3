#include "main.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <z3++.h>
#include <z3_api.h>

z3::expr TrySortExpr(z3::context &ctx, Z3_sort_kind arg_t) {
  switch (arg_t) {
  case Z3_INT_SORT:
    return ctx.int_val(28);
    break;
  case Z3_BOOL_SORT:
    return ctx.bool_val(false);
    break;
  default:
    printf("Wait Please\n");
  }
  return ctx.int_const("not found");
}

void AnalyzeFuncDecl(z3::solver &s, z3::func_decl fn, unsigned int fn_args) {
  std::cout << "\033[1;32mFunction: \033[0m" << fn.name() << "\n";
  printf("\033[4;33mDomain Arguments: %d\033[0m\n", fn_args);
  // 1)Get the Type of the arguments.
  // 2) Find values for each arg, assert the return type
  // 3) assert a use of the fun to == res

  s.push();
  z3::context &ctx = fn.ctx();
  z3::expr_vector args(ctx);
  z3::expr result = ctx.constant("result", fn.range());

  for (int i = 0; i < fn_args; i++) {
    z3::expr test_arg = TrySortExpr(ctx, fn.domain(i).sort_kind());
    args.push_back(test_arg);
  }

  z3::expr func_assert = fn(args);
  s.add(func_assert == result);

  std::cout << s.check() << "\n";
  std::cout << s.get_model() << "\n";
  s.pop();
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
