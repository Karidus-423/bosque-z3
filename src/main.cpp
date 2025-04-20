#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <z3++.h>

// Get list of function signatures and type definitions.

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: %s <FILENAME>\n", argv[0]);
    exit(1);
  }

  const char *filename = argv[1];
  z3::context ctx;
  z3::expr_vector expressions = ctx.parse_file(filename);
  for (int i = 0; i < expressions.size(); i++) {
    std::cout << expressions[i] << "\n";
  }

  z3::solver s(ctx);
  s.add(expressions);
  s.check();
  z3::model m = s.get_model();
  std::cout << "MODEL: \n" << m << "\n";

  return 0;
}
