#include "bsq-gen.h"
#include <z3++.h>

z3::model InitModel(const char *smt_file, z3::solver &s) {

  s.add(s.ctx().parse_file(smt_file));

  switch (s.check()) {
  case z3::sat:
    printf("--------Formula:\033[1;34mSAT\033[0m-------------\n");
    break;
    // TODO: Handle UNSAT and UNKNOWN
  case z3::unsat:
    printf("Got Unsat\n");
  case z3::unknown:
    printf("Got Unknown\n");
  }
  return s.get_model();
}
