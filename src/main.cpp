#include <cassert>
#include <cstdio>
#include <iostream>
#include <z3++.h>
#include <z3.h>
#include <z3_api.h>

#include "bsq-gen.h"

int main(int argc, char **argv) {
  z3::context ctx;
  z3::solver s(ctx);

  z3::model m = InitModel(argv[1], s);
  std::cout << m << "\n";

  for (int i = 0; i < 1 /*m.num_consts()*/; i++) {
    smt_func i_const = {
        .sol = s,
        .decl = m.get_const_decl(i),
        .sort = m.get_const_decl(i).range().sort_kind(),
        .from = 0,
        .to = 0,
    };

    z3::expr sat_res = FindConstant(i_const);
    std::cout << sat_res << "\n";
  }
}
