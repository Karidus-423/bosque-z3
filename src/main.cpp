// Given the spec produced by Bosque. Which is an SMT formula
//  for a type of that formula. Find passing values.

#include "z3-bosque.hpp"
#include <iostream>
#include <z3++.h>

int main() {
  TempTest temp_test;

  z3::context c;
  z3::expr test_bool = c.bool_const("test_bool");
  z3::expr test_nat = c.int_const("test_nat");
  z3::solver s(c);

  s.add(test_nat >= 0);

  std::cout << s.check() << "\n";
  z3::model result = s.get_model();
  std::cout << result << "\n";

  return 0;
}
