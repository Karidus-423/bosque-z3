#include "bsq-gen.h"
#include <cstdio>
#include <iostream>
#include <optional>
#include <vector>
#include <z3++.h>
#include <z3_api.h>

z3::expr FindDatatype(smt_func decl_result);
z3::expr FindInt(smt_const decl_result);

void CheckZ3Interp(z3::func_decl interp) {
  // TODO:Find a way to detect which interps are usable and which are not.
}

std::optional<z3::expr> FindConstant(smt_result decl_result) {
  z3::expr result = decl_result.sol.ctx().real_const("TEMP");

  switch (decl_result.decl.range().sort_kind()) {
  case Z3_INT_SORT:
    result = FindInt(decl_result);
    break;
  case Z3_DATATYPE_SORT:
    result = FindDatatype(decl_result);
    break;
  default:
    break;
  }

  return result;
}

// std::optional<z3::expr> FindFuncArgs(z3::solver &s, z3::func_decl c_decl) {}

// Define
z3::expr FindDatatype(smt_result ds) {
  z3::expr result = ds.sol.ctx().real_const("DATATYPE: N/A");

  z3::func_decl_vector constructs = ds.decl.range().constructors();

  for (int i = 0; i < constructs.size(); i++) {
    z3::expr decl = ds.decl();
    z3::func_decl decl_temp = constructs[i];

    ds.sol.push();
    z3::expr_vector assert_args = ds.decl().args();

    for (int j = 0; j < decl_temp.arity(); j++) {
      smt_result construct = {
          .sol = ds.sol,
          .decl = decl_temp,
          .from = 0,
          .to = decl_temp.arity(),
      };
      // Look for UNSAT ARG value.
      std::optional<z3::expr> arg = FindConstant(construct);
      if (arg.has_value()) {
        assert_args.push_back(arg.value());
      } else {
        z3::sort na_type = decl_temp.domain(i);
        z3::expr na_const = ds.sol.ctx().constant("N/A", na_type);
        assert_args.push_back(na_const);
      }
    }

    z3::expr temp_expr = decl_temp(assert_args);
    ds.sol.add(decl == temp_expr);
    z3::check_result rr = ds.sol.check();
    if (rr == z3::unsat) {
      result = temp_expr;
    }
    ds.sol.pop();
  }
  return result;
};

z3::expr FindInt(smt_result ds) {
  std::vector<int> choices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  z3::expr int_decl = ds.sol.ctx().constant("INT N/A", );
  for (int attempt : choices) {
  }
}
