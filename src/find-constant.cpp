#include "bsq-gen.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <z3++.h>
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

z3::expr FindFunc(smt_func func) {
  z3::expr result = func.sol.ctx().real_const("Func: N/A");
  z3::expr_vector args = func.sol.ctx();
  args.push_back(func.sol.ctx().int_val(1));
  args.push_back(func.sol.ctx().int_val(0));
  result = func.decl(args);
  return result;
}

z3::expr FindDatatype(smt_func vex) {
  // Get Datatype from constant.
  z3::sort vex_dt = vex.decl.range();
  z3::func_decl_vector dt = vex_dt.constructors();
  for (int i = 0; i < dt.size(); i++) {
    smt_func construct = InitFunc(dt[i], vex.sol);
    vex.sol.push();

    z3::expr sat_construct = FindFunc(construct);

    vex.sol.pop();
  }

  return vex.sol.ctx().string_val("Under Construction");
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

    vex.sol.push();

    z3::expr char_tmp = MakeChar(vex, mid);
    vex.sol.add(vex.decl().nth(index).char_to_int() < char_tmp.char_to_int());
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

  z3::expr char_const = MakeChar(vex, max - 1);
  vex.sol.add(vex.decl().nth(index) == char_const);

  return (char)max - 1;
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
