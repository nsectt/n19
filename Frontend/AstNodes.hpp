/*
* Copyright (c) 2024 Diago Lima
* All rights reserved.
*
* This software is licensed under the BSD 3-Clause "New" or "Revised" license
* found in the LICENSE file in the root directory of this project's source tree.
*/

#ifndef ASTNODES_HPP
#define ASTNODES_HPP
#include <Frontend/Token.hpp>
#include <Frontend/EntityQualifier.hpp>
#include <Core/Concepts.hpp>
#include <vector>
#include <memory>
#include <concepts>
#include <type_traits>
#include <optional>

#define N19_ASTNODE_TYPE_LIST \
  X(Node)                     \
  X(Vardecl)                  \
  X(ProcDecl)                 \
  X(EntityRef)                \
  X(EntityRefThunk)           \
  X(TypeRef)                  \
  X(TypeRefThunk)             \
  X(ScalarLiteral)            \
  X(AggregateLiteral)         \
  X(BinExpr)                  \
  X(UnaryExpr)                \
  X(Branch)                   \
  X(If)                       \
  X(Else)                     \
  X(Switch)                   \
  X(Case)                     \
  X(Default)                  \
  X(For)                      \
  X(While)                    \
  X(ConstBranch)              \
  X(Where)                    \
  X(Otherwise)                \
  X(ScopeBlock)               \
  X(NamespaceBlock)           \
  X(Call)                     \
  X(Break)                    \
  X(Continue)                 \
  X(Return)                   \
  X(Defer)                    \
  X(DeferIf)                  \
  X(Sizeof)                   \
  X(Subscript)                \

namespace n19 {
  // Class forward decls.
  #define X(NAME) class Ast##NAME;
  N19_ASTNODE_TYPE_LIST
  #undef X
}

class n19::AstNode {
public:
  #define X(NAME) NAME,
  enum class Type : uint16_t {
    N19_ASTNODE_TYPE_LIST
  };
  #undef X

  template<typename T = AstNode> requires IsAstNode<T>
  using Ptr = std::unique_ptr<T>;

  template<typename T = AstNode> requires IsAstNode<T>
  using Children = std::vector<Ptr<T>>;

  virtual auto print(
    uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void = 0;

  template<IsAstNode T>
  static auto create(
    size_t pos,
    uint32_t line,
    AstNode* parent = nullptr,
    const std::string& file = ""
  ) -> Ptr<T>;

  AstNode* parent_ = nullptr;
  size_t pos_      = 0;
  uint32_t line_   = 1;
  std::string file_;
  Type type_;

  virtual ~AstNode() = default;
protected:
  auto _print(
    uint32_t depth,
    const std::string& node_name
  ) const -> void;
};

class n19::AstBinExpr final : public AstNode {
public:
  TokenType op_type_    = TokenType::None;
  TokenCategory op_cat_ = 0;
  AstNode::Ptr<> left_  = nullptr;
  AstNode::Ptr<> right_ = nullptr;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstBinExpr() override = default;
  AstBinExpr() = default;
};

class n19::AstUnaryExpr final : public AstNode {
public:
  TokenType op_type_      = TokenType::None;
  TokenCategory op_cat_   = 0;
  AstNode::Ptr<> operand_ = nullptr;
  bool is_postfix         = false; // only relevant for '--' and '++'

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstUnaryExpr() override = default;
  AstUnaryExpr() = default;
};

class n19::AstScalarLiteral final : public AstNode {
public:
  std::string value_;
  enum : uint8_t {
    None,
    NullLit,
    IntLit,
    StringLit,
    U8Lit,
    FloatLit,
  } type_ = None;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstScalarLiteral() override = default;
  AstScalarLiteral() = default;
};

class n19::AstAggregateLiteral final : public AstNode {
public:
  AstNode::Children<> children_;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstAggregateLiteral() override = default;
  AstAggregateLiteral() = default;
};

class n19::AstEntityRef final : public AstNode {
public:
  Entity::ID id_= N19_INVALID_ENTITY_ID;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstEntityRef() override = default;
  AstEntityRef() = default;
};

class n19::AstEntityRefThunk final : public AstNode {
public:
  std::vector<std::string> name_;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstEntityRefThunk() override = default;
  AstEntityRefThunk() = default;
};

class n19::AstTypeRef final : public AstNode {
public:
  EntityQualifier descriptor_;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstTypeRef() override = default;
  AstTypeRef() = default;
};

class n19::AstTypeRefThunk final : public AstNode {
public:
  EntityQualifierThunk descriptor_;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstTypeRefThunk() override = default;
  AstTypeRefThunk() = default;
};

class n19::AstIf final : public AstNode {
public:
  AstNode::Children<> body_;
  AstNode::Ptr<> condition_ = nullptr;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstIf() override = default;
  AstIf() = default;
};

class n19::AstElse final : public AstNode {
public:
  AstNode::Children<> body_;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstElse() override = default;
  AstElse() = default;
};

class n19::AstWhere final : public AstNode {
public:
  AstNode::Children<> body_;
  AstNode::Ptr<> condition_ = nullptr;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstWhere() override = default;
  AstWhere() = default;
};

class n19::AstOtherwise final : public AstNode {
public:
  AstNode::Children<> body_;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstOtherwise() override = default;
  AstOtherwise() = default;
};

class n19::AstBranch final : public AstNode {
public:
  AstNode::Ptr<AstIf> if_     = nullptr; // If condition + block
  AstNode::Ptr<AstElse> else_ = nullptr; // Can be null!

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstBranch() override = default;
  AstBranch() = default;
};

class n19::AstConstBranch final : public AstNode {
public:
  AstNode::Ptr<AstWhere> where_ = nullptr;
  AstNode::Ptr<AstOtherwise> otherwise_ = nullptr; // Can be null!

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstConstBranch() override = default;
  AstConstBranch() = default;
};

class n19::AstCase final : public AstNode {
public:
  bool is_fallthrough = false;
  AstNode::Ptr<> value_ = nullptr;
  AstNode::Children<> children_;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstCase() override = default;
  AstCase() = default;
};

class n19::AstDefault final : public AstNode {
public:
  AstNode::Children<> children_;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstDefault() override = default;
  AstDefault() = default;
};

class n19::AstSwitch final : public AstNode {
public:
  AstNode::Ptr<> target_         = nullptr;
  AstNode::Ptr<AstDefault> dflt_ = nullptr;
  AstNode::Children<AstCase> cases_;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstSwitch() override = default;
  AstSwitch() = default;
};

class n19::AstScopeBlock final : public AstNode {
public:
  AstNode::Children<> children_;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstScopeBlock() override = default;
  AstScopeBlock() = default;
};

class n19::AstCall final : public AstNode {
public:
  AstNode::Ptr<> target_ = nullptr;
  AstNode::Children<> arguments_;

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstCall() override = default;
  AstCall() = default;
};

class n19::AstDefer final : public AstNode {
public:
  AstNode::Ptr<> call_ = nullptr;       // Should ALWAYS be AstCall under the hood

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstDefer() override = default;
  AstDefer() = default;
};

class n19::AstDeferIf final : public AstNode {
public:
  AstNode::Ptr<> call_ = nullptr;       // Should ALWAYS be AstCall under the hood
  AstNode::Ptr<> condition_ = nullptr;  // The condition on which we call this.

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstDeferIf() override = default;
  AstDeferIf() = default;
};

class n19::AstVardecl final : public AstNode {
public:
  AstNode::Ptr<> name_ = nullptr;  // EntityRef or EntityRefThunk
  AstNode::Ptr<> type_ = nullptr;  // TypeRef or TypeRefThunk

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstVardecl() override = default;
  AstVardecl() = default;
};

class n19::AstProcDecl final : public AstNode {
public:
  AstNode::Ptr<> name_ = nullptr; // EntityRef or EntityRefThunk
  AstNode::Children<> arg_decls_; // The parameter declarations (if any)
  AstNode::Children<> body_;      // The body of the procedure

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstProcDecl() override = default;
  AstProcDecl() = default;
};

class n19::AstReturn final : public AstNode {
public:
  AstNode::Ptr<> value_ = nullptr; // Can be null!

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstReturn() override = default;
  AstReturn() = default;
};

class n19::AstBreak final : public AstNode {
public:
  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstBreak() override = default;
  AstBreak() = default;
};

class n19::AstContinue final : public AstNode {
public:
  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;
  ~AstContinue() override = default;
};

class n19::AstFor final : public AstNode {
public:
  AstNode::Ptr<> body_    = nullptr;
  AstNode::Ptr<> init_    = nullptr; // Can be null!
  AstNode::Ptr<> update_  = nullptr; // Can be null!
  AstNode::Ptr<> cond_    = nullptr; // Can be null!

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstFor() override = default;
  AstFor() = default;
};

class n19::AstWhile final : public AstNode {
public:
  AstNode::Children<> body_;
  AstNode::Ptr<> cond_ = nullptr; // The loop condition
  bool is_dowhile      = false;   // If true: the loop is a do-while.

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstWhile() override = default;
  AstWhile() = default;
};

class n19::AstSubscript final : public AstNode {
public:
  AstNode::Ptr<> operand_ = nullptr; // The thing being subscripted.
  AstNode::Ptr<> value_   = nullptr; // The index value.

  auto print(uint32_t depth,
    const Maybe<std::string> &alias
  ) const -> void override;

  ~AstSubscript() override = default;
  AstSubscript() = default;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<n19::IsAstNode T>
auto n19::AstNode::create(
  const size_t pos,
  const uint32_t line,
  AstNode* parent,
  const std::string& file ) -> Ptr<T>
{
  auto ptr     = std::make_unique<T>();
  ptr->parent_ = parent;
  ptr->pos_    = pos;
  ptr->line_   = line;
  ptr->file_   = file;

  #define X(NAME)                      \
  if constexpr(IsSame<T, Ast##NAME>) { \
    ptr->type_ = Type::NAME;           \
  }

  N19_ASTNODE_TYPE_LIST
  #undef X

  if(parent != nullptr) ptr->parent_ = parent;
  return ptr;
}

#endif //ASTNODES_HPP
