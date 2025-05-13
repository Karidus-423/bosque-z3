#pragma once
#include <optional>
#include <z3++.h>
void CheckZ3Interp(z3::func_decl interp);
std::optional<z3::expr> FindValues(z3::solver &s, z3::func_decl c_decl);
