#include "bsq-gen.h"
#include <cstring>
#include <z3++.h>

const char *TestCString(const char *test_smt_CString) {
  z3::context ctx;
  z3::solver s(ctx);

  z3::model m = InitModel(test_smt_CString, s);
  std::string result;

  for (int i = 0; i < m.num_consts(); i++) {
    z3::func_decl smt_const = m.get_const_decl(i);
    if (smt_const.range().is_seq()) {
      smt_func vex = {
          .sol = s,
          .decl = m.get_const_decl(i),
          .sort = m.get_const_decl(i).range().sort_kind(),
          .from = 0,
          .to = 0,
      };
      z3::expr sat_string = FindCString(vex);
      result = sat_string.to_string().c_str();
    }
  }
  return strdup(result.c_str());
}
