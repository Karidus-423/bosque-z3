#include <cstdio>
#include <z3++.h>

int main() {
  std::cout << "find_model_example1\n";
  context c;
  expr x = c.int_const("x");
  expr y = c.int_const("y");
  solver s(c);

  s.add(x >= 1);
  s.add(y < x + 3);
  std::cout << s.check() << "\n";

  model m = s.get_model();
  std::cout << m << "\n";
  // traversing the model
  for (unsigned i = 0; i < m.size(); i++) {
    func_decl v = m[i];
    // this problem contains only constants
    assert(v.arity() == 0);
    std::cout << v.name() << " = " << m.get_const_interp(v) << "\n";
  }
  // we can evaluate expressions in the model.
  std::cout << "x + y + 1 = " << m.eval(x + y + 1) << "\n";
  return 0;
}
