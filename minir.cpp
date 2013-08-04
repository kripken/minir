
/*

minir - Mini IR
===============

  * Infinite register machine
  * i32

Instructions:

  * Binary(reg op1, reg op2) returns value
    * Add +
  * Const(i32 value) returns value
  * Input() returns value from input stream
  * Return(reg value) halts and defines program output

*/

#include <stdio.h>
#include <vector>

struct Instruction {
  enum Type {
    Input,
    Return,
    Binary,
    Const
  };
  enum SubType {
    Add
  };

  Type type;
  SubType subType;
  int op1, op2;
  int target;
};

struct Program {
  std::vector<Instruction> instructions;

  int Execute(int input);
};

//

int Program::Execute(int input) {
  struct State {
    std::vector<int> regs;
    inline void EnsureReg(int reg) {
      while (regs.size() < reg+1) {
        regs.push_back(0);
      }
    }
  } state;

  for (int i = 0; i < instructions.size(); i++) {
    Instruction &inst = instructions[i];
    switch (inst.type) {
      case Instruction::Type::Input:
        state.EnsureReg(inst.target);
        state.regs[inst.target] = input;
        break;
      case Instruction::Type::Return:
        state.EnsureReg(inst.target);
        return state.regs[inst.op1];
      case Instruction::Type::Binary:
        state.EnsureReg(inst.op1);
        state.EnsureReg(inst.op2);
        state.EnsureReg(inst.target);
        switch (inst.subType) {
          case Instruction::SubType::Add:
            state.regs[inst.target] = state.regs[inst.op1] + state.regs[inst.op2];
          default:
            printf("invalid subtype\n");
        }
        break;
      case Instruction::Type::Const:
        state.EnsureReg(inst.target);
        state.regs[inst.target] = inst.op1;
        break;
      default:
        printf("invalid type\n");
    }
  }
}

