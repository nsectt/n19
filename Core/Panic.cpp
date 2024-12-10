/*
* Copyright (c) 2024 Diago Lima
* All rights reserved.
*
* This software is licensed under the BSD 3-Clause "New" or "Revised" license
* found in the LICENSE file in the root directory of this project's source tree.
*/

#include <Core/Panic.hpp>
#include <Core/ConManip.hpp>
#include <cstdlib>
#include <string>
BEGIN_NAMESPACE(n19);

union test {
  uint32_t z;
  double x;

  test(double d) : x(d) {}
  ~test() = default;
};

auto panic_impl_(
  const std::string &file,
  const int line,
  const std::string &msg ) -> void
{
  set_console(Con::Red, Con::Bold);
  std::println("PANIC :: {}", msg);

  // Location details
  set_console(Con::Reset);
  std::println("In file \"{}\" at line {}.", file, line);
  ::exit(1);
}

auto fatal_impl_(const std::string &msg) -> void {
  set_console(Con::Red, Con::Bold);
  std::println("FATAL :: {}", msg);
  ::exit(1);
}

END_NAMESPACE(n19);