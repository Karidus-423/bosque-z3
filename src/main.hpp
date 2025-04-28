#pragma once

#include <vector>
#include <z3++.h>

#define OFFSET_ARG 1

// In Progress
std::string AnalyzeFuncDecl(z3::solver &s, z3::func_decl fn,
                            unsigned int fn_args);

void FindDataType(z3::solver &s, z3::func_decl fn, int arg_pos,
                  z3::expr_vector &known_args);

void FindAssertion(z3::solver &s, Z3_sort_kind type, z3::func_decl fn,
                   z3::sort sort_type);
z3::expr FindInt(z3::solver &s, z3::func_decl fn, int arg_pos,
                 z3::expr_vector &known_args);
std::vector<z3::expr> GetDummies(z3::context &ctx, z3::func_decl fn, int start);
