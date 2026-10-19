#include <iostream>
#include <fstream>
#include "../include/lexicalAnalysis.h"
#include "../include/syntaxAnalysis.h"
#include "../include/semanticAnalysis.h"
#include "../include/pseudoInst.h"
#include "../include/pass2.h"
#include "../include/createBin.h"

int main(int argc, char **argv){
    if (argc!=2){
        std::cout<<"Incorrect usage. ex: asm <filename>.asm"<<std::endl;
        return 1;
    }
    std::ifstream file(argv[1]);
    if (!file){
        std::cout<<"file not found"<<std::endl;
        return 1;
    }
    std::string line;
    std::vector<instruction> insts;
    std::unordered_map<std::string, int> labels;
    int lineNo=0;
    while (std::getline(file, line)){
        lineNo+=1;
        std::vector<token> list;
        if (!lexicalAnalysis(line, list, lineNo)) return 1;
        if (list.empty()) continue;
        instruction inst;
        inst.line=lineNo;
        std::string label;
        if (!syntaxAnalysis(list, inst, label)) return 1;
        if (!label.empty()){
            if (labels.contains(label)){
                std::cout<<"Label already exists. Line: "<<inst.line<<std::endl;
                return 1;
            }
            labels[label]=insts.size()*2;
        }
        if (inst.keyword==-1) continue;
        if (!semanticAnalysis(inst)) return 1;
        insts.push_back(inst);
        pseudoInst(insts);
    }
    if (!pass2(insts, labels)) return 1;
    if (!createBin(insts)) return 1;
    return 0;
}
