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

void CheckZ3Interp(z3::func_decl interp);

std::optional<z3::expr> FindConstant(smt_func decl_result);
