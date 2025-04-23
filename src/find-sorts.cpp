#include "main.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <z3++.h>
#include <z3_api.h>

// Theory of Constructors
void FindDataTypeFields(z3::solver &s, z3::sort data_type) {
  z3::func_decl_vector constructs = data_type.constructors();

  for (unsigned i = 0; i < constructs.size(); ++i) {
    z3::func_decl fn = constructs[i];
    unsigned int fn_args = fn.arity();
    std::cout << "Constructor \033[1;35m" << fn.name() << "\033[0m" << "\n";
    // You can call ctors[i].arity() to get number of fields in that constructor
    // TODO: Change how to build constructors.
    AnalyzeFuncDecl(s, fn, fn_args);
  }
}

// int arg_pos it 0th index
z3::expr FindInt(z3::solver &s, z3::func_decl fn, int arg_pos) {
  const std::vector<int> int_vec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  z3::context &ctx = s.ctx();
  int total_args = fn.arity();
  int assert_int = 0;
  int needed_dums = total_args - (arg_pos + OFFSET_ARG);
  for (int i = 0; i < int_vec.size(); i++) {
    int try_int = int_vec[i];

    // FIXME: The previous arguments for the function that are added when
    // leaving FindInt are not passed onto fn(args). Find a way to verify that
    // the added arguments exist.
    s.push();
    z3::context &fn_ctx = fn.ctx();
    z3::expr_vector args(fn_ctx);
    z3::expr try_expr = ctx.int_val(try_int);
    args.push_back(try_expr);

    if (needed_dums > 0) {
      int start = total_args - needed_dums;
      std::vector<z3::expr> dummy_expr = GetDummies(ctx, fn, start);

      for (int j = 0; j < dummy_expr.size(); j++) {
        args.push_back(dummy_expr[j]);
        std::cout << "DUMMY EXPR: " << dummy_expr[j]
                  << "TYPE:" << dummy_expr[j].get_sort() << "\n";
      }
    }
    // Build fun assertion
    z3::expr test_fn = fn(args);
    z3::expr test_res = ctx.constant("test_res", fn.range());
    s.add(test_fn == test_res);

    z3::check_result rr = s.check();
    s.pop();
    if (rr == z3::sat) {
      assert_int = try_int;
      break;
    }
  }

  // Found SAT int
  return ctx.int_val(assert_int);
}

std::vector<z3::expr> GetDummies(z3::context &ctx, z3::func_decl fn,
                                 int start) {
  std::vector<z3::expr> dummies;

  for (int i = start; i < fn.arity(); ++i) {
    std::string d_name = "dummy_" + std::to_string(i);
    z3::symbol d_sym = ctx.str_symbol(d_name.c_str());
    dummies.push_back(ctx.constant(d_sym, fn.domain(i)));
  }

  return dummies;
}
