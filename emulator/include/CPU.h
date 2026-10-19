#pragma once

#include "util.h"
#include "Registers.h"
#include "Memory.h"

class CPU{
    Registers regs;
    instInfo instructionInfo;
    uint16_t instruction=0;
    void (CPU::*opfuncs[32])()={
        &CPU::addFunc, &CPU::subFunc, &CPU::andFunc, &CPU::orFunc, &CPU::xorFunc, 
        &CPU::notFunc, &CPU::shlFunc, &CPU::shrFunc, &CPU::ashrFunc, &CPU::rorFunc, 
        &CPU::mulFunc, &CPU::dvrmFunc, &CPU::movFunc, &CPU::movhFunc, &CPU::movlFunc, 
        &CPU::loadFunc, &CPU::loadbFunc, &CPU::loadbuFunc, &CPU::storeFunc, &CPU::storebFunc, 
        &CPU::jmpFunc, &CPU::jcFunc, &CPU::callFunc, &CPU::retFunc
    };
    Memory &mem;
    uint16_t opSel();
    void addFunc();
    void subFunc();
    void andFunc();
    void orFunc();
    void xorFunc();
    void notFunc();
    void shlFunc();
    void shrFunc();
    void ashrFunc();
    void rorFunc();
    void mulFunc();
    void dvrmFunc();
    void movFunc();
    void movhFunc();
    void movlFunc();
    void loadFunc();
    void loadbFunc();
    void loadbuFunc();
    void storeFunc();
    void storebFunc();
    void jmpFunc();
    void jcFunc();
    void callFunc();
    void retFunc();
    uint16_t offsetCalc();
    public:
        CPU(Memory &memory);
        void fetch();
        void decode();
        void decode(uint16_t instruction);
        void exec();
        void showRegs(){
            regs.showRegs();
        }
        void writeReg(uint16_t data, uint8_t reg){
            regs.writeReg(data, reg);
        }
};
