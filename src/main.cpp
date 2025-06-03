#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <z3++.h>
#include <z3.h>
#include <z3_api.h>

#include "bsq-gen.h"

void BsqCString() {
  const char *actual = TestCString(c_string_smt);
  const char *expect = "Manchester";
  if (strcmp(actual, expect)) {
    std::cout << "CString: PASSED" << "\n";
  } else {
    printf("FAILED: Expected-> %s, Got-> %s", expect, actual);
  };
};

void BsqDataType() {
  const char *actual = TestDataType(data_type_smt);
  std::cout << actual << "\n";
  // const char *expect = "Manchester";
  // if (strcmp(actual, expect)) {
  //   std::cout << "Data Type: Passed" << "\n";
  // } else {
  //   printf("Expected: %s, Got: %s", expect, actual);
  // };
}

int main(int argc, char **argv) {
  BsqCString();
  BsqDataType();
}
