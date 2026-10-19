#include "../include/pseudoInst.h"

void pseudoInst(std::vector<instruction> &insts){
    operand reg6;
    reg6.opType=reg;
    reg6.op="r6";

    operand reg0;
    reg0.opType=reg;
    reg0.op="r0";

    operand lit2;
    lit2.opType=lit;
    lit2.imm=2;

    operand stackMem;
    stackMem.opType=mem;
    stackMem.base=reg6.op;
    stackMem.sign='+';
    stackMem.off.type=lit;
    stackMem.off.imm=0;

    operand destSrcReg;
    destSrcReg.opType=reg;

    switch (insts.back().keyword){
        case push:{
            instruction decSP;
            decSP.keyword=sub;
            decSP.operands={reg6, reg6, lit2};
            instruction storeElem;
            storeElem.keyword=store;
            destSrcReg.op=insts.back().operands[0].op;
            storeElem.operands={destSrcReg, stackMem};
            insts.back()=decSP;
            insts.push_back(storeElem);
            break;
        }
        case pop:{
            instruction incSP;
            incSP.keyword=add;
            incSP.operands={reg6, reg6, lit2};
            instruction loadElem;
            loadElem.keyword=load;
            destSrcReg.op=insts.back().operands[0].op;
            loadElem.operands={destSrcReg, stackMem};
            insts.back()=loadElem;
            insts.push_back(incSP);
            break;
        }
        case cmp:{
            instruction subInst;
            subInst.keyword=sub;
            operand op1, op2;
            op1.opType=reg;
            op1.op=insts.back().operands[0].op;
            op2.opType=insts.back().operands[1].opType;
            if (op2.opType==lit){
                op2.imm=insts.back().operands[1].imm;
            }else{
                op2.op=insts.back().operands[1].op;
            }
            subInst.operands={reg0, op1, op2};
            insts.back()=subInst;
            break;
        }
    }
}
