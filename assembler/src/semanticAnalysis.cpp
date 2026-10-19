#include <iostream>
#include "../include/semanticAnalysis.h"

bool checkOpFormat(const instruction &inst){
    std::vector<int> ops;
    for (int i=0;i<inst.operands.size();i++){
        ops.push_back(inst.operands[i].opType);
    }
    auto i=opFormat.find(inst.keyword);
    std::vector<std::vector<int>> format=i->second;
    for (int i=0;i<format.size();i++){
        if (format[i]==ops){
            return true;
        }
    }
    return false;
}

bool checkImmediate(int val, int a, int b, int line){
    if (!inRange(val, a, b)){
        std::cout<<"Literal out of range. Literal range "<<a<<" - "<<b<<". Line: "<<line<<std::endl;
        return false;
    }
    return true;
}

bool semanticAnalysis(const instruction &inst){
    if (inst.operands.size()>MAX_OP){
        std::cout<<"Instructions can take a max of 3 operands. Line: "<<inst.line<<std::endl;
        return false;
    }
    switch (inst.keyword){
        case add:
        case sub:
        case andGate:
        case orGate:
        case xorGate:
            if (!checkOpFormat(inst)){
                    std::cout<<"Incorrect order of operands. the instruction requires reg, reg, reg/imm. Line: "<<inst.line<<std::endl;
                    return false;
            }
            if (inst.operands[2].opType==lit){
                int num=inst.operands[2].imm;
                if (!checkImmediate(num, -8, 7, inst.line)) return false;
            }
            break;
        case mul:
        case dvrm:
            if (!checkOpFormat(inst)){
                    std::cout<<"Incorrect order of operands. the instruction requires reg, reg, reg/imm. Line: "<<inst.line<<std::endl;
                    return false;
            }
            break;
        case load:
        case loadb:
        case loadbu:
        case store:
        case storeb:{
            if (!checkOpFormat(inst)){
                std::cout<<"Incorrect order of operands. the instruction requires reg, [reg] or reg, [reg +/- reg/imm]. Line: "<<inst.line<<std::endl;
                return false;
            }
            if (inst.operands[1].off.type==lit){
                int num=inst.operands[1].off.imm;
                if (!checkImmediate(num, -8, 7, inst.line)) return false;
            }
            break;
        }
        case mov:
            if (!checkOpFormat(inst)){
                    std::cout<<"Incorrect order of operands. the instruction requires reg, reg/imm. Line: "<<inst.line<<std::endl;
                    return false;
            }
            if (inst.operands[1].opType==lit){
                int num=inst.operands[1].imm;
                if (!checkImmediate(num, -64, 63, inst.line)) return false;
            }
            break;
        case movh:
        case movl:
        case shl:
        case shr:
        case ashr:
        case ror:
            if (!checkOpFormat(inst)){
                    std::cout<<"Incorrect order of operands. the instruction requires reg, imm. Line: "<<inst.line<<std::endl;
                    return false;
            }
            if (inst.keyword==movh || inst.keyword==movl){
                int num=inst.operands[1].imm;
                if (!checkImmediate(num, 0, 255, inst.line)) return false;
            }else{
                int num=inst.operands[1].imm;
                if (!checkImmediate(num, 0, 15, inst.line)) return false;
            }
            break;
        case jmp:
        case call:
        if (!checkOpFormat(inst)){
            std::cout<<"Incorrect order of operands. the instruction requires reg/label. Line: "<<inst.line<<std::endl;
            return false;
        }
        break;
        case jc:
            if (!checkOpFormat(inst)){
                std::cout<<"Incorrect order of operands. the instruction requires label. Line: "<<inst.line<<std::endl;
                return false;
            }
            break;
        case notGate:
        case push:
        case pop:
            if (!checkOpFormat(inst)){
                    std::cout<<"Incorrect order of operands. the instruction requires reg. Line: "<<inst.line<<std::endl;
                    return false;
                }
            break;
        case ret:
            if (!checkOpFormat(inst)){
                std::cout<<"Incorrect order of operands. the instruction requires no operands. Line: "<<inst.line<<std::endl;
                return false;
            }
            break;
        case cmp:
            if (!checkOpFormat(inst)){
                std::cout<<"Incorrect order of operands. the instruction requires reg, reg/imm. Line: "<<inst.line<<std::endl;
                return false;
            }
            if (inst.operands[1].opType==lit){
                int num=inst.operands[1].imm;
                if (!checkImmediate(num, -8, 7, inst.line)) return false;
            }
            break;
    }
    return true;
}
