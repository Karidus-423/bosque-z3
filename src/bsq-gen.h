#pragma once
#include <optional>
#include <z3++.h>

typedef struct smt_const {
  z3::solver &sol;
  z3::func_decl decl;
  Z3_sort_kind sort;
  std::string results;
} smt_const;

typedef struct smt_func {
  z3::solver &sol;
  z3::func_decl decl;
  Z3_sort_kind sort;
  std::string results;
  uint from;
  uint to;
} smt_func;

void CheckZ3Interp(z3::func_decl interp);

std::optional<z3::expr> FindConstant(smt_const decl_result);
