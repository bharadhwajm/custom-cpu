#pragma once

#include <cstdint>
#include <string>

#define MEM_SIZE 65536

class Memory{
    uint8_t memory[MEM_SIZE];
    public:
        Memory();
        bool loadProgram(const std::string &fileName);
        void showMem();
        uint16_t readWord(uint16_t addr);
        uint8_t readByte(uint16_t addr);
        void writeWord(uint16_t data, uint16_t addr);
        void writeByte(uint8_t data, uint16_t addr);
        uint8_t *getMemory();
        void dump();
};
