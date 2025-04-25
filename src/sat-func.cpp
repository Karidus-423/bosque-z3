#include "main.hpp"
#include <iostream>
#include <z3++.h>

z3::expr TrySortExpr(z3::solver &s, z3::func_decl fn, int arg_pos,
                     z3::expr_vector &known_args) {
  z3::context &ctx = s.ctx();
  Z3_sort_kind arg_t = fn.domain(arg_pos).sort_kind();

  switch (arg_t) {
  case Z3_INT_SORT: {
    z3::expr try_int = FindInt(s, fn, arg_pos, known_args);
    return try_int;
  }
  case Z3_BOOL_SORT:
    return ctx.bool_val(false);
  case Z3_SEQ_SORT:
    return ctx.string_val("Test");
  case Z3_DATATYPE_SORT: {
    FindDataType(s, fn, arg_pos, known_args);
    return ctx.constant("tmp_const", fn.domain(arg_pos));
  }
  default:
    std::cout << "None for " << arg_t << "\n";
    return ctx.int_const("not found");
  }
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
  // i = 0; 0th arg will have val and rest dummy.
  // If SAT continue, add to ctx/print and continue to next arg.
  for (int i = 0; i < fn_args; i++) {
    std::cout << "ARG " << i;
    z3::expr test_arg = TrySortExpr(s, fn, i, args);
    args.push_back(test_arg);
    std::cout << " |VALUE " << test_arg << "\n";
  }

  z3::expr func_assert = fn(args);
  s.add(func_assert == result);

  z3::check_result rr = s.check();
  s.pop();

  if (rr == z3::sat) {
    std::cout << "All values found for func" << "\n";
  } else {
    std::cout << "Unable to find sat values for func" << "\n";
  }
}
