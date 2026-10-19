#include <iomanip>
#include <iostream>
#include <fstream>
#include "../include/Memory.h"

Memory::Memory(){
    for (int i=0;i<MEM_SIZE;i++){
        memory[i]=0x00;
    }
}

bool Memory::loadProgram(const std::string &fileName){
    std::ifstream file(fileName, std::ios::binary|std::ios::ate);
    if (!file){
        std::cout<<"File not found"<<std::endl;
        return false;
    }
    std::streamsize size=file.tellg();
    if (size>MEM_SIZE){
        std::cout<<"File must be at most "<<MEM_SIZE<<" bytes long"<<std::endl;
        return false;
    }
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char *>(memory), size);
    return true;
}

void Memory::showMem(){
    for (int i=0x0000;i<0x00ff;i+=16){
        std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<i<<": ";
        for (int j=0;j<16;j++){
            std::cout<<std::hex<<std::setw(2)<<std::setfill('0')<<static_cast<int>(memory[i+j])<<" ";
        }
        std::cout<<std::endl;
    }
}

uint16_t Memory::readWord(uint16_t addr){
    uint8_t low=memory[addr];
    uint8_t high=memory[(uint16_t)(addr+1)];
    uint16_t data=(high<<8)|low;
    return data;
}

uint8_t Memory::readByte(uint16_t addr){
    return memory[addr];
}

void Memory::writeWord(uint16_t data, uint16_t addr){
    uint8_t low=data&0xFF;
    uint8_t high=(data>>8)&0xFF;
    memory[addr]=low;
    memory[(uint16_t)(addr+1)]=high;
}

void Memory::writeByte(uint8_t data, uint16_t addr){
    memory[addr]=data;
}

uint8_t *Memory::getMemory(){
    return memory;
}

void Memory::dump(){
    std::ofstream file("memoryDump.bin", std::ios::out|std::ios::binary);
    file.write((char *)memory, MEM_SIZE);
}
