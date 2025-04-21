#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <z3++.h>
#include <z3_api.h>

// UPLOAD FILE, TODO: Figure out how to extract function signatures from it.
//  const char *filename = argv[1];
//  z3::expr_vector file_expr = ctx.parse_file(filename);
//  s.add(file_expr);

// To extract the function, pass the function and traverse the domain and
// range of it to find the types(sorts) for which to find the reals values
// of.
//

// This function should return all the values that were valid.
// But for that the solver is needed.

// TODO: Change using different methods.
void FindValidInt(z3::func_decl func, unsigned argc, z3::solver *s) {
  int try_val = 20;
  s->push();
  z3::expr use_val = s->ctx().int_val(try_val);
  z3::expr apply_arg = func(use_val);
  s->add(apply_arg == use_val);
  std::cout << s->check() << "\n";
  std::cout << s->get_model() << "\n";
  s->pop();
}

void ExamineFuncDecl(z3::func_decl func, z3::solver *s) {
  unsigned args = func.arity();
  for (unsigned i = 0; i < args; i++) {
    z3::sort domain_arg = func.domain(i);
    switch (domain_arg.sort_kind()) {
    case Z3_INT_SORT:
      FindValidInt(func, i, s);
      break;
    case Z3_BOOL_SORT:
      // FindValidBool()
      break;
    case Z3_CHAR_SORT:
      // FindValidChar()
      break;
    case Z3_SEQ_SORT:
      // FindValidString(unsigned length)
      break;
    default:
      printf("Its a kind\n");
    }
  }
}

void GetValues(z3::solver *s) {
  z3::model m = s->get_model();
  unsigned functions_n = m.num_funcs();

  for (unsigned i = 0; i < functions_n; i++) {
    ExamineFuncDecl(m.get_func_decl(i), s);
  }
}

int main(int argc, char **argv) {
  // Declare a function on the context.
  if (argv[1] == NULL) {
    printf("USAGE: %s <FILENAME>\n", argv[0]);
    exit(1);
  }
  z3::context c;
  z3::expr x = c.int_const("x");
  z3::expr y = c.int_const("y");
  z3::sort I = c.int_sort();
  z3::func_decl g = function("g", I, I);

  z3::solver s(c);
  s.add(x >= 1);
  s.add(y < x + 3);
  s.add(g(y) == x);

  // If unsat then this will never be false, hence always true.
  if (s.check() == z3::sat) {
    printf("Values:\n");
    GetValues(&s);
  } else {
    printf("UNSAT\n");
  }
}
