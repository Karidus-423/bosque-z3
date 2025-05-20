#pragma once
#include <optional>
#include <z3++.h>

typedef struct smt_func {
  z3::solver &sol;
  z3::func_decl decl;
  Z3_sort_kind sort;
  uint from;
  uint to;
  std::string result;
} smt_func;

// Return SAT constant expr.
z3::expr FindConstant(smt_func decl_result);
// Return SAT Datatype expr for res.
z3::expr FindDatatype(smt_func vex);
// Return SAT Func expr with arguments from func_decl.
z3::expr FindFunc(smt_func vex);
// Return SAT Integer expr for res.
z3::expr FindInt(smt_func vex);
// Get SAT Seq of ASCII Char.
z3::expr FindCString(smt_func vex);
