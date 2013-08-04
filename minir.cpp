
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
#include <stdlib.h>

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

  int Print();

  void GenerateFuzz();
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
      case Instruction::Input:
        state.EnsureReg(inst.target);
        state.regs[inst.target] = input;
        break;
      case Instruction::Return:
        state.EnsureReg(inst.op1);
        return state.regs[inst.op1];
      case Instruction::Binary:
        state.EnsureReg(inst.op1);
        state.EnsureReg(inst.op2);
        state.EnsureReg(inst.target);
        switch (inst.subType) {
          case Instruction::Add:
            state.regs[inst.target] = state.regs[inst.op1] + state.regs[inst.op2];
          default:
            printf("invalid subtype\n");
        }
        break;
      case Instruction::Const:
        state.EnsureReg(inst.target);
        state.regs[inst.target] = inst.op1;
        break;
      default:
        printf("invalid type\n");
    }
  }
  return 0;
}

int Program::Print() {
  for (int i = 0; i < instructions.size(); i++) {
    printf("(%3d)  ", i);
    Instruction &inst = instructions[i];
    switch (inst.type) {
      case Instruction::Input:
        printf("r%d = input", inst.target);
        break;
      case Instruction::Return:
        printf("return %d = input", inst.op1);
        break;
      case Instruction::Binary:
        printf("r%d = binary-", inst.target);
        switch (inst.subType) {
          case Instruction::Add:
            printf("add");
            break;
          default:
            printf("invalid subtype\n");
        }
        printf(" r%d, r%d", inst.op1, inst.op2);
        break;
      case Instruction::Const:
        printf("r%d  %d", inst.target, inst.op1);
        break;
      default:
        printf("invalid type\n");
    }
    printf("\n");
  }
  return 0;
}

void Program::GenerateFuzz() {
  int numInstructions = rand() % 100;
  instructions.resize(numInstructions);
  int numRegs = rand() % 10;
  for (int i = 0; i < numInstructions; i++) {
    Instruction &inst = instructions[i];
    switch (rand() % 4) {
      case 0:
        inst.type = Instruction::Input;
        inst.target = rand() % numRegs;
        break;
      case 1:
        inst.type = Instruction::Return;
        inst.op1 = rand() % numRegs;
        break;
      case 2:
        inst.type = Instruction::Binary;
        inst.op1 = rand() % numRegs;
        inst.op2 = rand() % numRegs;
        inst.target = rand() % numRegs;
        switch (rand() % 1) {
          case 0:
            inst.subType = Instruction::Add;
            break;
          default:
            printf("invalid subtype\n");
        }
        break;
      case 3:
        inst.type = Instruction::Const;
        inst.op1 = rand();
        inst.target = rand() % numRegs;
        break;
      default:
        printf("invalid type\n");
    }
  }
}

