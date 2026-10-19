#include "../include/util.h"
#include <iostream>
#include "../include/syntaxAnalysis.h"

int getVal(std::string &val, int line){
    int num;
    bool negative=false;
    if (val[0]=='-') negative=true;
    try{
        if (negative) val.erase(0, 1);
        if (val[1]=='x' || val[1]=='X'){
            num=std::stoi(val.substr(2), nullptr, 16);
        }else if (val[1]=='b' || val[1]=='B'){
            num=std::stoi(val.substr(2), nullptr, 2);
        }else{
            num=std::stoi(val);
        }
        if (negative){
            return -num;
        }
        return num;
    }catch (const std::exception &){
        std::cout<<"Invalid literal. Line: "<<line<<std::endl;
        return INT_MIN;
    }
}

bool syntaxAnalysis(const std::vector<token> &list, instruction &inst, std::string &label){
    parser p={list, static_cast<int>(list.size()), 0};
    if (peek(p).type==iden && peekNext(p).type==colon){
        label=peek(p).token;
        next(p);
        next(p);
    }
    if (peek(p).type==eol){
        return true;
    }
    if (peek(p).type==keyword){
        auto i=keywords.find(peek(p).token);
        inst.keyword=i->second;
        if (inst.keyword==jc){
            auto j=condCodes.find(peek(p).token);
            inst.condition=j->second;
        }
        if (peek(p).token=="umulh" || peek(p).token=="udiv" || peek(p).token=="urem"){
            inst.signedUnsign=true;
        }
        if (peek(p).token=="mul" || peek(p).token=="umul" || peek(p).token=="rem" || peek(p).token=="urem"){
            inst.highLowDivRem=true;
        }
        next(p);
    }else{
        std::cout<<"Incorrect syntax. Expected label:, label:keyword [operands] or keyword [operands]. Line: "<<inst.line<<std::endl;
        return false;
    }
    while (p.pos<p.length){
        if (peek(p).type==reg || peek(p).type==iden || peek(p).type==lit){
            operand op;
            op.opType=peek(p).type;
            if (peek(p).type==lit){
                std::string s=peek(p).token;
                int num=getVal(s, inst.line);
                if (num==INT_MIN) return false;
                op.imm=num;
            }else{
                op.op=peek(p).token;
            }
            inst.operands.push_back(op);
            next(p);
        }else if(peek(p).type==lparan){
            next(p);
            operand op;
            if (peek(p).type==reg){
                op.opType=mem;
                op.base=peek(p).token;
                next(p);
            }else{
                std::cout<<"Incorrect syntax. Expected register. Line: "<<inst.line<<std::endl;
                return false;
            }
            if (peek(p).type==plus || peek(p).type==minus){
                op.sign=peek(p).token[0];
                if (peekNext(p).type==reg){
                    next(p);
                    op.off.type=peek(p).type;
                    op.off.reg=peek(p).token;
                    next(p);
                }else if (peekNext(p).type==lit){
                    std::string s=peekNext(p).token;
                    int num=getVal(s, inst.line);
                    if (num==INT_MIN) return false;
                    if (peek(p).type==minus){
                        num=-num;
                    }
                    op.off.type=peekNext(p).type;
                    op.off.imm=num;
                    next(p);
                    next(p);
                }else{
                    std::cout<<"Incorrect syntax. Expected register or literal. Line: "<<inst.line<<std::endl;
                    return false;
                }
            }
            if (peek(p).type==rparan){
                inst.operands.push_back(op);
                next(p);
            }else{
                std::cout<<"Incorrect syntax. expected [reg +/- reg/imm]. Line: "<<inst.line<<std::endl;
                return false;
            }
        }else if ((peek(p).type==plus || peek(p).type==minus) && peekNext(p).type==lit){
            std::string s=peekNext(p).token;
            int num=getVal(s, inst.line);
            if (num==INT_MIN) return false;
            if (peek(p).type==minus){
                num=-num;
            }
            operand op;
            op.opType=lit;
            op.imm=num;
            inst.operands.push_back(op);
            next(p);
            next(p);
        }else{
            std::cout<<"Incorrect syntax. Expected operand. Line: "<<inst.line<<std::endl;
            return false;
        }
        if (peek(p).type==comma && peekNext(p).type!=eol){
            next(p);
            continue;
        }
        if (peek(p).type!=eol){
            std::cout<<"Incorrect syntax. Expected op, op or op, op, op. Line: "<<inst.line<<std::endl;
            return false;
        }
    }
    return true;
}
