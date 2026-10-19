#pragma once

#include <cstdint>

enum opcodes{
    add, sub, andGate, orGate, xorGate, notGate, 
    shl, shr, ashr, ror, mul, dvrm, mov, movh, movl, 
    load, loadb, loadbu, store, storeb, jmp, jc, 
    call, ret
};

enum jCond{
    je, jne, jg, ujg, jge, ujge, jl, ujl, jle, ujle
};

struct instInfo{
    uint8_t opcode=0;
    uint8_t reg1=0, reg2=0, reg3=0;
    uint16_t immediate=0;
    uint8_t condition=0, shamt=0;
    bool regImm=false, highLow=false, signUnsign=false, divRem=false, addSub=false;
};

inline int16_t signExtend(uint16_t data, uint8_t shamt){
    return ((int16_t)(data<<shamt))>>shamt;
}
