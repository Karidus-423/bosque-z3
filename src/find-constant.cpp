#include "bsq-gen.h"
#include <cstdio>
#include <iostream>
#include <optional>
#include <z3++.h>
#include <z3_api.h>

z3::expr FindDatatype(smt_func res);
z3::expr FindFunc(smt_func res);
z3::expr FindInt(smt_func res);
z3::expr FindString(smt_func res);

// Return SAT constant expr.
std::optional<z3::expr> FindConstant(smt_func const_res) {
  z3::expr result = const_res.sol.ctx().real_const("N/A");

  switch (const_res.sort) {
  case Z3_INT_SORT:
    result = FindInt(const_res);
    break;
  case Z3_DATATYPE_SORT:
    result = FindDatatype(const_res);
    break;
  case Z3_SEQ_SORT:
    result = FindString(const_res);
    break;
  default:
    break;
  }

  return result;
}

smt_func InitFunc(z3::func_decl func, z3::solver &s) {
  smt_func res = {
      .sol = s,
      .decl = func,
      .sort = func.range().sort_kind(),
      .from = 0,
      .to = func.arity(),
      .result = "N/A",
  };

  return res;
}

// Return SAT Func expr with arguments from func_decl.
z3::expr FindFunc(smt_func func) {
  z3::expr result = func.sol.ctx().real_const("Func: N/A");
  z3::expr_vector args = func.sol.ctx();
  args.push_back(func.sol.ctx().int_val(1));
  args.push_back(func.sol.ctx().int_val(0));
  result = func.decl(args);
  return result;
}

// Return SAT Datatype expr for res.
z3::expr FindDatatype(smt_func res) {
  z3::expr result = res.sol.ctx().real_const("DATATYPE: N/A");
  z3::func_decl_vector constructs = res.decl.range().constructors();

  for (int i = 0; i < constructs.size(); i++) {
    smt_func construct = InitFunc(constructs[i], res.sol);

    res.sol.push();

    z3::expr construct_tmp = FindFunc(construct);
    z3::check_result rr = res.sol.check();

    res.sol.pop();
    if (rr == z3::sat) {
      result = construct_tmp;
    }
  }
  return result;
};

// Return SAT Integer expr for res.
z3::expr FindInt(smt_func res) {
  z3::expr result = res.sol.ctx().real_const("Int: N/A");
  return result;
}

z3::expr FindString(smt_func res) {
  z3::expr result = res.sol.ctx().real_const("String: N/A");
  z3::expr str_tmp = res.sol.ctx().string_val("Manchester", 10);

  res.sol.push();

  res.sol.add(res.decl() == str_tmp);
  std::cout << res.sol.check() << "\n";

  res.sol.pop();

  return result;
}
