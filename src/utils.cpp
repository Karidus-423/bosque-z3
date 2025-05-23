#include "bsq-gen.h"
#include <iostream>
#include <z3++.h>

std::optional<z3::model> InitModel(const char *smt_file, z3::solver &s) {
  s.add(s.ctx().parse_string(smt_file));

  switch (s.check()) {
  case z3::sat:
    break;
  case z3::unsat:
    return std::nullopt;
  case z3::unknown:
    return std::nullopt;
  }

  z3::model m = s.get_model();
  return m;
}

smt_func InitFunc(z3::func_decl func, z3::solver &s) {
  smt_func vex = {
      .sol = s,
      .decl = func,
      .sort = func.range().sort_kind(),
      .from = 0,
      .to = func.arity(),
      .result = "N/A",
  };

  return vex;
}

z3::expr MakeChar(smt_func vex, char c) {
  Z3_ast r = Z3_mk_char(vex.sol.ctx(), c);
  return z3::expr(vex.sol.ctx(), r);
}
