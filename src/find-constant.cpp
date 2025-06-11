#include "bsq-gen.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <z3++.h>
#include <z3.h>
#include <z3_api.h>

#define MIN_ASCII 0
#define MAX_ASCII 255
#define MIN_UTF8 0
#define MAX_UTF8 512

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

z3::expr FindFunc(smt_func vexf) {
  z3::expr result = vexf.sol.ctx().real_const("Func: N/A");

  vexf.sol.push();
  z3::expr_vector args(vexf.sol.ctx());
  z3::expr arg1 = vexf.sol.ctx().int_val(1);
  args.push_back(arg1);
  z3::expr arg2 = vexf.sol.ctx().int_val(0);
  args.push_back(arg2);

  vexf.sol.add(vexf.decl() == vexf.decl(args));
  z3::check_result rr = vexf.sol.check();
  vexf.sol.pop();

  return result;
}

z3::expr FindDatatype(smt_func vex) {
  z3::expr result = vex.sol.ctx().real_const("Datatype: N/A");

  z3::func_decl_vector vex_dt = vex.decl.range().constructors();
  vex.sol.push();

  z3::constructors constructs_tmp(vex.sol.ctx());

  for (int i = 0; i < vex_dt.size(); i++) {
    smt_func vex_entity = InitFunc(vex_dt[i], vex.sol);
    z3::expr construct_tmp = FindFunc(vex_entity);
    // constructs_tmp.push_back(construct_tmp);
  }

  z3::symbol tmp_sym = vex.sol.ctx().str_symbol("tmp_sym");
  z3::sort vex_tmp = vex.sol.ctx().datatype(tmp_sym, constructs_tmp);

  z3::func_decl tmp_func = vex.sol.ctx().function(tmp_sym, 0, 0, vex_tmp);

  vex.sol.add(vex.decl() == tmp_func);

  z3::check_result rr = vex.sol.check();
  vex.sol.pop();

  if (rr == z3::sat) {
    return vex.sol.ctx().string_val("Worked");
  } else {
    return vex.sol.ctx().string_val("Under Construction");
  }
};

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

// Return SAT char
std::optional<char> BinSearchChar(smt_func vex, z3::expr index, int min,
                                  int max) {
  while (min < max) {
    char mid = (max / 2) + (min / 2) + (((max % 2) + (min % 2)) / 2);
    printf("CURR:-> %c\n", mid);

    vex.sol.push();

    z3::expr char_tmp = MakeChar(vex, mid);
    vex.sol.add(vex.decl().nth(index).char_to_int() <= char_tmp.char_to_int());
    z3::check_result rr = vex.sol.check();

    vex.sol.pop();

    if (rr == z3::check_result::sat) {
      max = mid;
    } else if (rr == z3::check_result::unsat) {
      min = mid + 1;
    } else {
      return std::nullopt;
    }
  }
  char const_char = max;
  printf("---RES: %c----", const_char);

  z3::expr char_const = MakeChar(vex, const_char);
  vex.sol.add(vex.decl().nth(index) == char_const);

  return const_char;
}

z3::expr FindCString(smt_func vex) {
  z3::expr result = vex.sol.ctx().real_const("String: N/A");
  vex.sol.ctx().string_val("TEST");
  z3::expr str_len = FindStringLen(vex);

  std::string str_tmp("");

  for (int i = 0; i < str_len.get_numeral_int(); i++) {
    z3::expr index = vex.sol.ctx().int_val(i);
    std::optional<char> sat_char =
        BinSearchChar(vex, index, MIN_ASCII, MAX_ASCII);
    if (sat_char.has_value()) {
      str_tmp += sat_char.value();
    } else {
      str_tmp.append("{}");
    }
  }

  vex.sol.push();
  z3::expr str_expr = vex.sol.ctx().string_val(str_tmp);
  vex.sol.add(vex.decl() == str_expr);
  z3::check_result rr = vex.sol.check();
  vex.sol.pop();

  if (rr == z3::sat) {
    result = str_expr;
  }

  return result;
}
