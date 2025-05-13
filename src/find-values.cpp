#include "bsq-gen.h"
#include <cstdio>
#include <iostream>
#include <optional>
#include <z3++.h>
#include <z3_api.h>

void CheckZ3Interp(z3::func_decl interp) {
  // TODO:Find a way to detect which interps are usable and which are not.
}

z3::expr FindDatatype(z3::solver &s, z3::func_decl c_decl);
z3::expr FindInt(z3::solver &s, z3::func_decl c_decl);

std::optional<z3::expr> FindValues(z3::solver &s, z3::func_decl c_decl) {
  z3::expr result = s.ctx().real_const("TEMP");
  switch (c_decl.range().sort_kind()) {
  case Z3_INT_SORT:
    result = FindInt(s, c_decl);
    break;
  case Z3_DATATYPE_SORT:
    result = FindDatatype(s, c_decl);
    break;
  default:
    break;
  }

  return result;
}

std::optional<z3::expr> FindFuncArgs(z3::solver &s, z3::func_decl c_decl) {}

// Define
z3::expr FindDatatype(z3::solver &s, z3::func_decl c_decl) {
  z3::expr result = s.ctx().real_const("TEMP");
  z3::func_decl_vector c_constructs = c_decl.range().constructors();

  for (int i = 0; i < c_constructs.size(); i++) {
    z3::expr decl = c_decl();
    z3::func_decl decl_temp = c_constructs[i];

    s.push();
    z3::expr_vector assert_args(s.ctx());

    for (int j = 0; j < c_decl.arity(); j++) {
      std::optional<z3::expr> arg = FindValues(s, decl_temp);
      if (!arg.has_value()) {
        printf("unable to find value for this\n");
      }
      assert_args.push_back(arg.value());
    }

    z3::expr temp_expr = decl_temp(assert_args);
    s.add(decl == temp_expr);
    z3::check_result rr = s.check();
    s.pop();

    if (rr == z3::unsat) {
      printf("HOLDS\n");
      result = temp_expr;
      return result;
    }
  }
  return result;
};

z3::expr FindInt(z3::solver &s, z3::func_decl c_decl);
