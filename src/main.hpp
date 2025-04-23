#pragma once

#include <vector>
#include <z3++.h>

#define OFFSET_ARG 1

typedef struct {
  std::string func_name;
  std::vector<std::string> variables;
} satFunc_t;

z3::expr GetFuncRangeExpr(z3::context ctx, Z3_sort_kind sort_t);
z3::expr FindInt(z3::solver &s, z3::func_decl fn, int arg_n);
std::vector<z3::expr> GetDummies(z3::context &ctx, z3::func_decl fn, int start);

void FindDataTypeFields(z3::solver &s, z3::sort expr_sort);
void FindAssertion(z3::solver &s, Z3_sort_kind type, z3::func_decl fn,
                   z3::sort sort_type);
void AnalyzeFuncDecl(z3::solver &s, z3::func_decl fn, unsigned int fn_args);
void FindIntAssertion(z3::solver &s, z3::func_decl fn, unsigned int fn_arg,
                      z3::sort expr_int);
void AnalyzeFuncDecl(z3::solver &s, z3::func_decl fn, unsigned int fn_args);
