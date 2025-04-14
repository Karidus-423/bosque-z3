// Your tool will take this SMT file in and load it in Z3. From there it will:
//
//     1) Take a function (api) signature and define constants for each of the
//        arguments and return — asserting that f(a, b, c) = res.
//     2) Then it will check that all this is Satisfiable
//     3) From there you do the push/pop as you describe in your 1,2,3 steps.
//
//
// For now we are assuming that these are all simple scalar values. Once you can
// do the 1-3 steps for extracting Bool, Nat, Int, and Strings, then we will
// move on to working with more complex composite types.

#include "utils.hpp"
#include <iostream>
#include <z3++.h>

void Test() {
  z3::context c;
  z3::expr boolean = c.bool_const("boolean");
  z3::expr natural = c.int_const("natural");
  z3::expr real = c.int_const("c");

  z3::sort domain = c.bool_sort();

  z3::func_decl GetUser = c.function("GetUser", b);

  z3::solver s(c);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::printf("Usage: <FILENAME>\n");
    return 1;
  }

  File_t *file = FileOpen(argv[1]);
  if (file == NULL) {
    printf("Retrieving file contents failed\n");
    return 1;
  }

  Test();

  return 0;
}
