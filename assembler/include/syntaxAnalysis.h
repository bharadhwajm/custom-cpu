#pragma once

#include "util.h"

struct parser{
    std::vector<token> tokens;
    int length;
    int pos;
};

const token eolTok={"eol",eol};

inline token peek(const parser &p){
    if (p.pos<p.length) return p.tokens[p.pos];
    return eolTok;
}

inline token peekNext(const parser &p){
    if (p.pos+1<p.length) return p.tokens[p.pos+1];
    return eolTok;
}

inline int next(parser &p){
    if (p.pos<p.length){
        p.pos+=1;
        return p.pos;
    }else{
        return -1;
    }
}

bool syntaxAnalysis(const std::vector<token> &list, instruction &inst, std::string &label);
