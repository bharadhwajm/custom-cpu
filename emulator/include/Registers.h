#pragma once

#include <cstdint>

#define NUM_GPR 8
#define SP_REG 6
#define ADDR_REG 7

class Registers{
    uint16_t gpr[NUM_GPR];
    uint16_t pc;
    uint8_t flags;
    public:
        Registers();
        void showRegs();
        uint16_t readReg(uint8_t reg);
        void writeReg(uint16_t data, uint8_t reg);
        uint16_t readPC();
        void writePC(uint16_t data);
        uint8_t readFlags();
        void writeFlags(uint8_t data);
};
