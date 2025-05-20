// TODO: Make a macro to make sure that sol.check() is inside push() pop(); Or
// something similar that confirms this action.
#include "bsq-gen.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <z3++.h>
#include <z3_api.h>

z3::expr FindConstant(smt_func vex) {
  z3::expr result = vex.sol.ctx().real_const("N/A");

  switch (vex.sort) {
  case Z3_INT_SORT:
    result = FindInt(vex);
    break;
  case Z3_DATATYPE_SORT:
    result = FindDatatype(vex);
    break;
  case Z3_SEQ_SORT:
    result = FindCString(vex);
    break;
  default:
    break;
  }

  return result;
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

z3::expr FindFunc(smt_func func) {
  z3::expr result = func.sol.ctx().real_const("Func: N/A");
  z3::expr_vector args = func.sol.ctx();
  args.push_back(func.sol.ctx().int_val(1));
  args.push_back(func.sol.ctx().int_val(0));
  result = func.decl(args);
  return result;
}

z3::expr FindDatatype(smt_func vex) {
  z3::expr result = vex.sol.ctx().real_const("DATATYPE: N/A");
  z3::func_decl_vector constructs = vex.decl.range().constructors();

  for (int i = 0; i < constructs.size(); i++) {
    smt_func construct = InitFunc(constructs[i], vex.sol);

    vex.sol.push();

    z3::expr construct_tmp = FindFunc(construct);
    z3::check_result rr = vex.sol.check();

    vex.sol.pop();
    if (rr == z3::sat) {
      result = construct_tmp;
    }
  }
  return result;
};

// NOTE: Could Add flags to be used for FindStringLen. Since they are the same.
z3::expr FindInt(smt_func vex) {
  std::vector<int> choices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  z3::expr result = vex.sol.ctx().real_const("Int: N/A");

  for (int i : choices) {
    vex.sol.push();
    z3::expr int_tmp = vex.sol.ctx().int_val(i);

    vex.sol.add(vex.decl() == int_tmp);

    z3::check_result rr = vex.sol.check();
    vex.sol.pop();
    if (rr == z3::sat) {
      result = int_tmp;
      break;
    }
  }
  return result;
}

z3::expr FindStringLen(smt_func vex) {
  std::vector<int> choices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  z3::expr result = vex.sol.ctx().real_const("Int String: N/A");

  for (int i : choices) {
    vex.sol.push();
    z3::expr len_tmp = vex.sol.ctx().int_val(i);

    vex.sol.add(vex.decl().length() == len_tmp);

    z3::check_result rr = vex.sol.check();
    vex.sol.pop();
    if (rr == z3::sat) {
      result = len_tmp;
      break;
    }
  }

  return result;
}

z3::expr MakeChar(smt_func vex, char c) {
  Z3_ast r = Z3_mk_char(vex.sol.ctx(), c);
  return z3::expr(vex.sol.ctx(), r);
}

z3::expr FindCString(smt_func vex) {
  z3::expr result = vex.sol.ctx().real_const("String: N/A");
  vex.sol.ctx().string_val("TEST");
  z3::expr str_len = FindStringLen(vex);

  for (int i = 0; i < str_len; i++) {
    vex.sol.push();
    z3::expr try_char = MakeChar(vex, 'M');
    z3::expr index = vex.sol.ctx().int_val(i);

    z3::expr assert_nth(vex.decl().nth(index) == try_char);

    vex.sol.add(assert_nth);

    z3::check_result rr = vex.sol.check();
    vex.sol.pop();
    // TODO: If found char is SAT. Add it as a fact and move to the next.
    if (rr == z3::sat) {
      result = str_tmp;
      break;
    }
  }

  return result;
}
