#include "../include/createBin.h"
#include <cstdint>
#include <iostream>
#include <fstream>

#define MAX_SIZE 65535

bool createBin(std::vector<instruction> &insts){
    int length=insts.size();
    std::vector<uint16_t> instructions;
    for (int i=0;i<length;i++){
        uint16_t inst=0x0000;
        inst|=(insts[i].keyword<<11);
        if (insts[i].keyword==store || insts[i].keyword==storeb){
            std::string temp=insts[i].operands[0].op;
            insts[i].operands[0].op=insts[i].operands[1].base;
            insts[i].operands[1].base=temp;
        }
        for (int j=0;j<insts[i].operands.size();j++){
            if (insts[i].operands[j].opType==reg){
                auto k=registers.find(insts[i].operands[j].op);
                inst|=((k->second)<<(8-(3*j)));
            }
            if (insts[i].operands[j].opType==mem){
                auto k=registers.find(insts[i].operands[j].base);
                inst|=((k->second)<<5);
                if (insts[i].operands[j].off.type==reg){
                    k=registers.find(insts[i].operands[j].off.reg);
                    inst|=((k->second)<<2);
                }
            }
        }
        if (!insts[i].operands.empty() && (insts[i].operands.back().opType==lit || insts[i].operands.back().off.type==lit)){
            switch (insts[i].keyword){
                case add:
                case sub:
                case andGate:
                case orGate:
                case xorGate:{
                    int num=insts[i].operands[2].imm;
                    inst|=((num&0xf)<<1);
                    inst|=0x01;
                    break;
                }
                case load:
                case loadb:
                case loadbu:
                case store:
                case storeb:{
                    int num=insts[i].operands[1].off.imm;
                    inst|=((num&0xf)<<1);
                    inst|=0x01;
                    break;
                }
                case mov:{
                    int num=insts[i].operands[1].imm;
                    inst|=((num&0x7f)<<1);
                    inst|=0x01;
                    break;
                }
                case movh:
                case movl:{
                    int num=insts[i].operands[1].imm;
                    inst|=(num&0xff);
                    break;
                }
                case jmp:
                case call:{
                    int num=insts[i].operands[0].imm;
                    inst|=((num&0x3ff)<<1);
                    inst|=0x01;
                    break;
                }
                case jc:{
                    int num=insts[i].operands[0].imm;
                    inst|=((num&0x7f)<<4);
                    break;
                }
                case shl:
                case shr:
                case ashr:
                case ror:{
                    int num=insts[i].operands[1].imm;
                    inst|=((num&0xf)<<4);
                    break;
                }
            }
        }
        switch (insts[i].keyword){
            case load:
            case loadb:
            case loadbu:
            case store:
            case storeb:
                if (insts[i].operands[1].sign=='-' && insts[i].operands[1].off.type==reg){
                    inst|=0x02;
                }
                break;
            case mul:
            case dvrm:
                if (insts[i].signedUnsign){
                    inst|=0x02;
                }
                if (insts[i].highLowDivRem){
                    inst|=0x01;
                }
                break;
            case jc:
                inst|=(insts[i].condition&0xf);
                break;
        }
        instructions.push_back(inst);
    }
    if ((instructions.size()*2)>MAX_SIZE){
        std::cout<<"File should be less than "<<MAX_SIZE<<" bytes long. current size: "<<instructions.size()*2<<" bytes"<<std::endl;
        return false;
    }
    std::ofstream file("bin/program.bin", std::ios::out|std::ios::binary);
    file.write(reinterpret_cast<char *>(instructions.data()), instructions.size()*2);
    return true;
}
