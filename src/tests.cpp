#include "bsq-gen.h"
#include <cstring>
#include <iostream>
#include <z3++.h>

const char *TestCString(const char *smt_file) {
  z3::context ctx;
  z3::solver s(ctx);

  auto m = InitModel(smt_file, s);
  if (!m.has_value()) {
    return "\0";
  }

  std::string result;

  for (int i = 0; i < m.value().num_consts(); i++) {
    z3::func_decl smt_const = m.value().get_const_decl(i);

    if (smt_const.range().is_seq()) {
      smt_func vex = InitFunc(m.value().get_const_decl(i), s);

      z3::expr sat_string = FindConstant(vex);

      result = sat_string.to_string().c_str();
    }
  }
  return strdup(result.c_str());
}

const char *TestInt(const char *smt_file) {
  z3::context ctx;
  z3::solver s(ctx);

  auto m = InitModel(smt_file, s);
  if (!m.has_value()) {
    return "\0";
  }

  std::string result;

  for (int i = 0; i < m.value().num_consts(); i++) {
    z3::func_decl smt_const = m.value().get_const_decl(i);

    if (smt_const.range().is_int()) {
      smt_func vex = InitFunc(m.value().get_const_decl(i), s);

      z3::expr sat_string = FindConstant(vex);

      result = sat_string.to_string().c_str();
    }
  }
  return strdup(result.c_str());
}

const char *TestDataType(const char *smt_file) {
  z3::context ctx;
  z3::solver s(ctx);

  auto m = InitModel(smt_file, s);
  if (!m.has_value()) {
    return "\0";
  }

  std::string result;

  for (int i = 0; i < m.value().num_consts(); i++) {
    z3::func_decl smt_const = m.value().get_const_decl(i);

    if (smt_const.range().is_datatype() && i == 0) {
      smt_func vex = InitFunc(m.value().get_const_decl(i), s);
      std::cout << m.value().get_const_decl(i) << "\n";

      z3::expr sat_string = FindConstant(vex);

      result = sat_string.to_string().c_str();
    }
  }
  return strdup(result.c_str());
}
