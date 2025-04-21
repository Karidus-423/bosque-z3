#include <cassert>
#include <optional>
#include <z3++.h>

std::optional<bool> FindBool(z3::solver &s) {
  // s.push();
  // z3::expr bsq_bool = s.ctx().bool_const("bsq_bool");
  // z3::ass
}

std::optional<int> FindInt(z3::solver &s, z3::expr &e);
std::optional<unsigned int> FindNat(z3::solver &s, z3::expr &e);
std::optional<const char *> FindCString(z3::solver &s, z3::expr &e);
