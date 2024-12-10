/*
* Copyright (c) 2024 Diago Lima
* All rights reserved.
*
* This software is licensed under the BSD 3-Clause "New" or "Revised" license
* found in the LICENSE file in the root directory of this project's source tree.
*/

#ifndef NATIVE_HANDLE_HPP
#define NATIVE_HANDLE_HPP
#include <Core/Platform.hpp>
#include <Core/ClassTraits.hpp>
#include <Core/TypeTraits.hpp>
BEGIN_NAMESPACE(n19::sys);

template<typename Value>
class Handle {
N19_MAKE_COMPARABLE_MEMBER(Handle, value_);
static_assert(!IsReference<Value>);
public:
  virtual auto close()      -> void = 0;
  virtual auto invalidate() -> void = 0;
  virtual auto is_invalid() -> bool = 0;
  virtual ~Handle() = default;

  [[nodiscard]] auto value() const -> Value;
  explicit operator bool() const;
protected:
  Value value_{};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Value>
N19_FORCEINLINE auto Handle<Value>::value() const -> Value {
  return value_;
}

template<typename Value>
N19_FORCEINLINE Handle<Value>::operator bool() const {
  return !is_invalid();
}

END_NAMESPACE(n19::sys);
#endif //NATIVE_HANDLE_HPP
