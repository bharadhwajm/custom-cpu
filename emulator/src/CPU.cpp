#include "../include/CPU.h"
#include "../include/Decoder.h"
#include <iostream>

CPU::CPU(Memory &memory):mem(memory){}

uint16_t CPU::opSel(){
    uint16_t op;
    if (instructionInfo.regImm){
        op=instructionInfo.immediate;
    }else{
        switch (instructionInfo.opcode){
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
                op=regs.readReg(instructionInfo.reg3);
                break;
            case mov:
                op=regs.readReg(instructionInfo.reg2);
                break;
            case jmp:
            case call:
                op=regs.readReg(instructionInfo.reg1);
                break;
        }
    }
    return op;
}

uint16_t CPU::offsetCalc(){
    uint16_t res;
    uint16_t op1;
    if (instructionInfo.opcode==load ||instructionInfo.opcode==loadb || instructionInfo.opcode==loadbu){
        op1=regs.readReg(instructionInfo.reg2);
    }else{
        op1=regs.readReg(instructionInfo.reg1);
    }
    uint16_t op2=opSel();
    if (!instructionInfo.regImm){
        if (instructionInfo.addSub){
            res=op1-op2;
        }else{
            res=op1+op2;
        }
    }else{
        res=op1+op2;
    }
    return res;
}

void CPU::addFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t op1=regs.readReg(instructionInfo.reg2);
    uint16_t op2=opSel();
    uint16_t res=op1+op2;
    regs.writeReg(res, dest);
}

void CPU::subFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t op1=regs.readReg(instructionInfo.reg2);
    uint16_t op2=opSel();
    uint16_t res=op1-op2;
    if (res==0){
        regs.writeFlags(regs.readFlags()|0x1);
    }else{
        regs.writeFlags(regs.readFlags()&0xFE);
    }
    if (op1<op2){
        regs.writeFlags(regs.readFlags()|0x8);
    }else{
        regs.writeFlags(regs.readFlags()&0xF7);
    }
    if ((int16_t)res<0){
        regs.writeFlags(regs.readFlags()|0x4);
    }else{
        regs.writeFlags(regs.readFlags()&0xFB);
    }
    if (((int16_t)op2>0 && (int16_t)op1-(int16_t)op2<INT16_MIN)||
    ((int16_t)op2<0 && (int16_t)op1-(int16_t)op2>INT16_MAX)){
        regs.writeFlags(regs.readFlags()|0x2);
    }else{
        regs.writeFlags(regs.readFlags()&0xFD);
    }
    regs.writeReg(res, dest);
}

void CPU::andFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t op1=regs.readReg(instructionInfo.reg2);
    uint16_t op2=opSel();
    uint16_t res=op1&op2;
    regs.writeReg(res, dest);
}

void CPU::orFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t op1=regs.readReg(instructionInfo.reg2);
    uint16_t op2=opSel();
    uint16_t res=op1|op2;
    regs.writeReg(res, dest);
}

void CPU::xorFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t op1=regs.readReg(instructionInfo.reg2);
    uint16_t op2=opSel();
    uint16_t res=op1^op2;
    regs.writeReg(res, dest);
}

void CPU::notFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t op=regs.readReg(instructionInfo.reg1);
    uint16_t res=~op;
    regs.writeReg(res, dest);
}

void CPU::shlFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t op=regs.readReg(instructionInfo.reg1);
    uint8_t shamt=instructionInfo.shamt;
    uint16_t res=op<<shamt;
    regs.writeReg(res, dest);
}

void CPU::shrFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t op=regs.readReg(instructionInfo.reg1);
    uint8_t shamt=instructionInfo.shamt;
    uint16_t res=op>>shamt;
    regs.writeReg(res, dest);
}

void CPU::ashrFunc(){
    uint8_t dest=instructionInfo.reg1;
    int16_t op=regs.readReg(instructionInfo.reg1);
    uint8_t shamt=instructionInfo.shamt;
    int16_t res=op>>shamt;
    regs.writeReg((uint16_t)res, dest);
}

void CPU::rorFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t op=regs.readReg(instructionInfo.reg1);
    uint8_t shamt=instructionInfo.shamt;
    uint16_t res=(op>>shamt)|(op<<(16-shamt));
    regs.writeReg(res, dest);
}

void CPU::mulFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t op1=regs.readReg(instructionInfo.reg2);
    uint16_t op2=regs.readReg(instructionInfo.reg3);
    uint32_t res;
    if (instructionInfo.highLow){
        res=(uint32_t)op1*op2;
    }else{
        if (instructionInfo.signUnsign){
            res=(uint32_t)op1*op2;
            res>>=16;
        }else{
            res=(uint32_t)((int32_t)(int16_t)op1*(int32_t)(int16_t)op2);
            res>>=16;
        }
    }
    regs.writeReg((uint16_t)res, dest);
}

void CPU::dvrmFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t op1=regs.readReg(instructionInfo.reg2);
    uint16_t op2=regs.readReg(instructionInfo.reg3);
    uint16_t res;
    if (op2==0){
        regs.writeReg(0, dest);
        return;
    }
    if (!instructionInfo.divRem){
        if (instructionInfo.signUnsign){
            res=op1/op2;
        }else{
            res=(int16_t)op1/(int16_t)op2;
        }
    }else{
        if (instructionInfo.signUnsign){
            res=op1%op2;
        }else{
            res=(int16_t)op1%(int16_t)op2;
        }
    }
    regs.writeReg(res, dest);
}

void CPU::movFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t data=opSel();
    regs.writeReg(data, dest);
}

void CPU::movhFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t data=regs.readReg(dest)|(instructionInfo.immediate)<<8;
    regs.writeReg(data, dest);
}

void CPU::movlFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t data=instructionInfo.immediate;
    regs.writeReg(data, dest);
}

void CPU::loadFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t addr=offsetCalc();
    uint16_t data=mem.readWord(addr);
    regs.writeReg(data, dest);
}

void CPU::loadbFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t addr=offsetCalc();
    int8_t data=mem.readByte(addr);
    regs.writeReg((int16_t)data, dest);
}

void CPU::loadbuFunc(){
    uint8_t dest=instructionInfo.reg1;
    uint16_t addr=offsetCalc();
    uint16_t data=mem.readByte(addr);
    regs.writeReg(data, dest);
}

void CPU::storeFunc(){
    uint16_t data=regs.readReg(instructionInfo.reg2);
    uint16_t addr=offsetCalc();
    mem.writeWord(data, addr);
}

void CPU::storebFunc(){
    uint16_t data=regs.readReg(instructionInfo.reg2);
    uint16_t addr=offsetCalc();
    mem.writeByte(data, addr);
}

void CPU::jmpFunc(){
    uint16_t pc=regs.readPC();
    uint16_t op=opSel();
    if (instructionInfo.regImm){
        regs.writePC(pc+op);
    }else{
        regs.writePC(op);
    }
}

void CPU::jcFunc(){
    uint8_t condition=instructionInfo.condition;
    uint16_t pc=regs.readPC();
    uint8_t flags=regs.readFlags();
    int16_t offset=instructionInfo.immediate;
    bool jump;
    switch (condition){
        case je:
            jump=flags&0x1;
            break;
        case jne:
            jump=!(flags&0x1);
            break;
        case jg:
            jump=(((flags&0x1)==0)&&(((flags&0x2)>>1)==((flags&0x4)>>2)));
            break;
        case ujg:
            jump=(((flags&0x1)==0)&&(((flags&0x8)>>3)==0));
            break;
        case jge:
            jump=((flags&0x1)||(((flags&0x2)>>1)==((flags&0x4)>>2)));
            break;
        case ujge:
            jump=((flags&0x1)||(((flags&0x8)>>3)==0));
            break;
        case jl:
            jump=(((flags&0x1)==0)&&(((flags&0x2)>>1)!=((flags&0x4)>>2)));
            break;
        case ujl:
            jump=(((flags&0x1)==0)&&((flags&0x8)>>3));
            break;
        case jle:
            jump=((flags&0x1)||(((flags&0x2)>>1)!=((flags&0x4)>>2)));
            break;
        case ujle:
            jump=((flags&0x1)||((flags&0x8)>>3));
            break;
    }
    if (jump){
        regs.writePC(pc+offset);
    }
}

void CPU::callFunc(){
    uint16_t pc=regs.readPC();
    uint16_t op=opSel();
    regs.writeReg(pc, ADDR_REG);
    uint16_t newPC=pc;
    if (instructionInfo.regImm){
        newPC+=op;
    }else{
        newPC=op;
    }
    regs.writePC(newPC);
}

void CPU::retFunc(){
    uint16_t addr=regs.readReg(ADDR_REG);
    regs.writePC(addr);
}

void CPU::exec(){
    (this->*opfuncs[instructionInfo.opcode])();
}

void CPU::fetch(){
    uint16_t pc=regs.readPC();
    instruction=mem.readWord(pc);
    pc+=2;
    regs.writePC(pc);
}

void CPU::decode(){
    instructionInfo=Decoder::decode(instruction);
}

void CPU::decode(uint16_t instruction){
    instructionInfo=Decoder::decode(instruction);
}
