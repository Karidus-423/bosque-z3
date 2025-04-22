#pragma once

#include <vector>
#include <z3++.h>

typedef struct {
  std::string func_name;
  std::vector<std::string> variables;
} satFunc_t;

z3::expr GetFuncRangeExpr(z3::context ctx, Z3_sort_kind sort_t);

void FindDataTypeFields(z3::solver &s, z3::sort expr_sort);
void FindAssertion(z3::solver &s, Z3_sort_kind type, z3::func_decl fn,
                   z3::sort sort_type);
void AnalyzeFuncDecl(z3::solver &s, z3::func_decl fn, unsigned int fn_args);
void FindIntAssertion(z3::solver &s, z3::func_decl fn, unsigned int fn_arg,
                      z3::sort expr_int);
