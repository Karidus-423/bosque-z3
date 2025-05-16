#include <cassert>
#include <cstdio>
#include <iostream>
#include <z3++.h>
#include <z3.h>
#include <z3_api.h>

#include "bsq-gen.h"

int main(int argc, char **argv) {
  if (argc < 1) {
    printf("Usage: <FILENAME>\n");
  }

  const char *filename = argv[1];

  z3::context ctx;
  z3::solver s(ctx);

  s.add(ctx.parse_file(filename));

  switch (s.check()) {
  case z3::sat:
    printf("--------Formula:\033[1;34mSAT\033[0m-------------\n");
    break;
    // TODO: Handle UNSAT and UNKNOWN
  case z3::unsat:
    printf("Got Unsat\n");
    return 1;
  case z3::unknown:
    printf("Got Unknown\n");
    return 1;
  }

  z3::model m = s.get_model();

  for (int i = 0; i < 1 /*m.num_consts()*/; i++) {
    smt_const decl_result = {
        .sol = s,
        .decl = m.get_const_decl(i),
        .sort = m.get_const_decl(i).range().sort_kind(),
    };
    std::optional<z3::expr> found_val;

    // TODO: CheckZ3Interp(m.get_const_decl(c_decl)); if usable, return.
    // Return valid Int, Bool, String, Datatype expr.
    found_val = FindConstant(decl_result);

    for (int j = 0; j < found_val.value().num_args(); j++) {
      std::cout << found_val.value().arg(j).to_string() << "\n";
    }
  }
}
