#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>
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

  // TODO:s.get_model(), check for errors, see if there you should handle them
  // better.
  z3::model m = s.get_model();
  std::vector<std::string> found_values;

  for (int i = 0; i < m.num_consts(); i++) {
    std::string found_val;
    z3::func_decl c_decl = m.get_const_decl(i);
    std::cout << c_decl << "\n";
    // TODO: In progress... (if true return interp values. false use vector.)
    // CheckZ3Interp(m.get_const_decl(c_decl));
    found_val = FindValues(s, c_decl);
    std::cout << found_val << "\n";
  }

  // Find a value for f that is sat.
  // printf("--------\033[1;35mDEBUGGING\033[0m-----------\n");

  // // Get the Constant. Range = Type which is datatype here.
  // z3::func_decl f_const = m.get_const_decl(0);
  // z3::func_decl foo_dt = f_const.range().constructors()[0];
  //
  // s.push();
  // // Datatype
  // z3::expr f_expr = f_const();
  // // Ints
  // z3::expr foo_x = ctx.int_val(0);
  // z3::expr foo_y = ctx.int_val(1);
  // z3::expr_vector foo_fields(ctx);
  // foo_fields.push_back(foo_x);
  // foo_fields.push_back(foo_y);
  // z3::expr foo_expr = foo_dt(foo_fields);
  //
  // s.add(f_expr == foo_expr);
  // switch (s.check()) {
  // case z3::sat:
  //   printf("SAT: ASSERT Failed\n");
  //   break;
  // case z3::unsat:
  //   printf("UNSAT: ALWAYS HOLDS\n");
  //   break;
  // case z3::unknown:
  //   printf("UNKOWN\n");
  //   break;
  // }
  //
  // s.pop();
}

// TODO: Find out what the hell was the output of this. Is there any use?
// EX.(declare-fun Main@main (Main@Foo) (@Result Int)) INTERP-->
// 0x36dcb5e0
