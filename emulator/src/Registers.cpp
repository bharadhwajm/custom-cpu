#include <iostream>
#include <iomanip>
#include "../include/Registers.h"

Registers::Registers(){
    for (int i=0;i<NUM_GPR;i++){
        gpr[i]=0x00;
    }
    pc=0x00;
    flags=0x00;
}

void Registers::showRegs(){
    for (int i=0;i<NUM_GPR;i++){
        std::cout<<"r"<<i<<": "<<std::setw(4)<<std::setfill('0')<<std::hex<<gpr[i]<<std::endl;
    }
    std::cout<<"PC: "<<std::setw(4)<<std::setfill('0')<<std::hex<<pc<<std::endl;
    std::cout<<"flags: "<<std::setw(2)<<std::setfill('0')<<std::hex<<static_cast<int>(flags)<<std::endl;
}

uint16_t Registers::readReg(uint8_t reg){
    return gpr[reg];
}

void Registers::writeReg(uint16_t data, uint8_t reg){
    if (reg==0){
        return;
    }
    gpr[reg]=data;
}

uint16_t Registers::readPC(){
    return pc;
}

void Registers::writePC(uint16_t data){
    pc=data;
}

uint8_t Registers::readFlags(){
    return flags;
}

void Registers::writeFlags(uint8_t data){
    flags=data;
}
