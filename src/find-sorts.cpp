#include "main.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <z3++.h>
#include <z3_api.h>

void FindDataType(z3::solver &s, z3::func_decl fn, int arg_pos,
                  z3::expr_vector &known_args) {
  printf("\nFinding Data type\n");
  z3::context &ctx = s.ctx();
  s.push();
  // Adding found argument expressions to current function context
  z3::context &fn_ctx = fn.ctx();
  z3::expr_vector args(fn_ctx);

  // Datatype Main@Foo
  z3::sort main_foo = fn.domain(arg_pos);
  std::cout << main_foo << "\n";
  printf("Constructors\n");
  unsigned n = Z3_get_datatype_sort_num_constructors(ctx, main_foo);
  z3::func_decl_vector main_foo_fields = main_foo.constructors();
  for (int i = 0; i < main_foo_fields.size(); i++) {
    AnalyzeFuncDecl(s, main_foo_fields[i], main_foo_fields[i].arity());
  }
  std::cout << main_foo.constructors() << ": " << n << "\n";
  std::cout << main_foo_fields[0] << ": " << n << "\n";

  z3::check_result rr = s.check();
  s.pop();

  if (rr == z3::sat) {
    // Return found expression of completed constructor
    // return data_type;
  }
}

// Find sat assertion for arg type int
z3::expr FindInt(z3::solver &s, z3::func_decl fn, int arg_pos,
                 z3::expr_vector &known_args) {
  const std::vector<int> int_vec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  z3::context &ctx = s.ctx();
  int total_args = fn.arity();
  int assert_int = 0;
  int needed_dums = total_args - (arg_pos + OFFSET_ARG);
  for (int i = 0; i < int_vec.size(); i++) {
    int try_int = int_vec[i];

    s.push();
    // Adding found argument expressions to current function context
    z3::context &fn_ctx = fn.ctx();
    z3::expr_vector args(fn_ctx);
    for (unsigned j = 0; j < known_args.size(); ++j) {
      args.push_back(known_args[j]);
    }
    z3::expr try_expr = ctx.int_val(try_int);
    args.push_back(try_expr);

    if (needed_dums > 0) {
      int start = total_args - needed_dums;
      std::vector<z3::expr> dummy_expr = GetDummies(ctx, fn, start);

      for (int j = 0; j < dummy_expr.size(); j++) {
        args.push_back(dummy_expr[j]);
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
