// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace re6::bytecode {

struct State {
  size_t cursor;
  size_t state;
};

enum OperandKind : uint8_t {
  INT,
  STATE,
};

struct StackOperand {
  explicit StackOperand(OperandKind kind) : kind(kind) {}
  OperandKind kind;
  virtual ~StackOperand() {}
};

template <typename T>
bool isa(StackOperand *operand) {
  return operand->kind == T::KIND;
}

template <typename T>
T *cast(StackOperand *operand) {
  // FIXME: Rely on abi
  return reinterpret_cast<T *>(operand);
}

struct IntOperand : public StackOperand {
  static constexpr OperandKind KIND = INT;
  explicit IntOperand(int operand) : StackOperand(INT), operand(operand) {}
  int const operand;
};

struct StateOperand : public StackOperand {
  static constexpr OperandKind KIND = STATE;
  explicit StateOperand(State state) : StackOperand(KIND), state(state) {}
  State const state;
};

struct Label {};

enum Opcode : uint8_t {
  IFEOI,
  IFEMPTYSTACK,
  IFEQ,
  PUSH,
  POP,
  NEXTCHAR,
  RET,
};

struct Instruction {
  explicit Instruction(Opcode op) : op(op) {}
  Opcode const op;
  virtual ~Instruction() {}
};

template <typename T>
bool isa(Instruction *inst) {
  return inst->op == T::OP;
}

template <typename T>
T *cast(Instruction *inst) {
  // FIXME: Rely on abi
  return reinterpret_cast<T *>(inst);
}

struct IfEOI : public Instruction {
  static constexpr Opcode OP = IFEOI;
  IfEOI(Label label) : Instruction(OP), label(label) {}
  Label const label;
};

struct IfEmptyStack : public Instruction {
  static constexpr Opcode OP = IFEMPTYSTACK;
  explicit IfEmptyStack(Label label) : Instruction(OP), label(label) {}
  Label const label;
};

struct IfEq : public Instruction {
  static constexpr Opcode OP = IFEQ;
  explicit IfEq(int operand, Label label)
      : Instruction(OP), operand(operand), label(label) {}
  int const operand;
  Label const label;
};

struct Push : public Instruction {
  static constexpr Opcode OP = PUSH;
  explicit Push(State state) : Instruction(OP), state(state) {}
  State state;
};

struct Pop : public Instruction {
  static constexpr Opcode OP = POP;
  explicit Pop() : Instruction(OP) {}
};

} // namespace re6::bytecode
