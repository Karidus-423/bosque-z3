#pragma once
#include <optional>
#include <z3++.h>

typedef struct smt_func {
  z3::solver &sol;
  z3::func_decl decl;
  Z3_sort_kind sort;
  uint from;
  uint to;
} smt_func;

// Utils
std::optional<z3::model> InitModel(const char *smt_file, z3::solver &s);
smt_func InitFunc(z3::func_decl func, z3::solver &s);
z3::expr MakeChar(smt_func vex, char c);

// Return SAT Func with arguments from func_decl.
z3::expr FindFunc(smt_func vex);
// Return SAT constant expr.
z3::expr FindConstant(smt_func decl_result);
// Return SAT Datatype expr for res.
z3::expr FindDatatype(smt_func vex);
// Return SAT Integer expr for res.
z3::expr FindInt(smt_func vex);
// Get SAT Seq of ASCII Char.
z3::expr FindCString(smt_func vex);

// Tests
extern const char *c_string_smt;
extern const char *data_type_smt;

// Test Bosque ASCII only strings.
const char *TestCString(const char *smt_file);
const char *TestInt(const char *smt_file);
const char *TestDataType(const char *smt_file);

// const char *TestNat(const char *smt_file);
// const char *TestBigInt(const char *smt_file);
// const char *TestBigNat(const char *smt_file);
// Test Bosque UTF-8 strings.
// const char *TestString(const char *smt_file);
// const char *TestBool(const char *smt_file);
