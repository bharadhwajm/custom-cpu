#pragma once

#include "util.h"

class Decoder{
    public:
        static instInfo decode(uint16_t instruction){
            instInfo inst;
            inst.opcode=instruction>>11;
            inst.reg1=(instruction>>8)&0x7;
            inst.reg2=(instruction>>5)&0x7;
            inst.reg3=(instruction>>2)&0x7;
            inst.shamt=(instruction>>4)&0xF;
            inst.regImm=instruction&0x1;
            inst.signUnsign=instruction&0x2;
            inst.highLow=instruction&0x1;
            inst.divRem=instruction&0x1;
            inst.addSub=instruction&0x2;
            inst.condition=(instruction)&0xF;
            switch (inst.opcode){
                case add:
                case sub:
                case andGate:
                case orGate:
                case xorGate:
                case load:
                case loadb:
                case loadbu:
                case store:
                case storeb:
                    inst.immediate=signExtend((instruction>>1)&0xF, 12);
                    break;
                case mov:
                    inst.immediate=signExtend((instruction>>1)&0x7F, 9);
                    break;
                case movh:
                case movl:
                    inst.immediate=instruction&0xFF;
                    break;
                case jmp:
                case call:
                    inst.immediate=signExtend((instruction>>1)&0x3FF, 6);
                    break;
                case jc:
                    inst.immediate=signExtend((instruction>>4)&0x7F, 9);
                    break;
            }
            return inst;
    }
};
