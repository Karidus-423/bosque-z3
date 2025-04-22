#pragma once

#include <z3++.h>

void FindDataTypeFields(z3::solver &s, z3::sort expr_sort);
void FindAssertion(z3::solver &s, Z3_sort_kind type, z3::func_decl fn,
                   z3::sort sort_type);
void AnalyzeFuncDecl(z3::solver &s, z3::func_decl fn, unsigned int fn_args);
void FindIntAssertion(z3::solver &s, z3::func_decl fn, z3::sort expr_int);
