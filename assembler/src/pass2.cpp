#include "../include/pass2.h"
#include <iostream>

bool checkJump(int val, int a, int b, int line){
    if (!inRange(val, a, b)){
        std::cout<<"jmp label out of range. label range "<<a<<" - "<<b<<". Line: "<<line<<std::endl;
        return false;
    }
    return true;
}

bool pass2(std::vector<instruction> &insts, const std::unordered_map<std::string, int> &labels){
    int length=insts.size();
    for (int i=0;i<length;i++){
        if (insts[i].keyword==jmp || insts[i].keyword==jc || insts[i].keyword==call){
            if (insts[i].operands[0].opType==iden){
                if (labels.contains(insts[i].operands[0].op)){
                    auto it=labels.find(insts[i].operands[0].op);
                    int jumpTo=it->second-((i*2)+2);
                    if (insts[i].keyword==jmp || insts[i].keyword==call){
                        if (!checkJump(jumpTo, -512, 511, insts[i].line)) return false;
                    }else{
                        if (!checkJump(jumpTo, -64, 63, insts[i].line)) return false;
                    }
                    insts[i].operands[0].imm=jumpTo;
                    insts[i].operands[0].opType=lit;
                }else{
                    std::cout<<"label undefined. Line: "<<insts[i].line<<std::endl;
                    return false;
                }
            }
        }
    }
    return true;
}
