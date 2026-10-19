#pragma once

#include <string>
#include <unordered_map>
#include <set>
#include <iostream>

#define MAX_OP 3

struct token{
    std::string token;
    int type=-1;
};

struct offset{
    int type=-1;
    std::string reg;
    int imm=0;
};

struct operand{
    int opType=-1;
    std::string op;
    std::string base;
    int imm=0;
    offset off;
    char sign='\0';
};

struct instruction{
    int keyword=-1;
    int condition=-1;
    std::vector<operand> operands;
    bool signedUnsign=false;
    bool highLowDivRem=false;
    int line=-1;
};

enum types{
    keyword, reg, lit, mem, iden, comma, plus, minus, lparan, rparan, colon, eol
};

enum opcodes{
    add, sub, andGate, orGate, xorGate, notGate, 
    shl, shr, ashr, ror, mul, dvrm, mov, movh, movl, 
    load, loadb, loadbu, store, storeb, jmp, jc, 
    call, ret, push=32, pop, cmp
};

const std::unordered_map<int, std::vector<std::vector<int>>> opFormat={
    {add, {{reg, reg, reg}, {reg, reg, lit}}}, {sub, {{reg, reg, reg}, {reg, reg, lit}}}, {andGate, {{reg, reg, reg}, {reg, reg, lit}}}, 
    {orGate, {{reg, reg, reg}, {reg, reg, lit}}}, {xorGate, {{reg, reg, reg}, {reg, reg, lit}}}, {notGate, {{reg}}}, {shl, {{reg, lit}}}, 
    {shr, {{reg, lit}}}, {ashr, {{reg, lit}}}, {ror, {{reg, lit}}}, {mul, {{reg, reg, reg}}}, {dvrm, {{reg, reg, reg}}}, 
    {mov, {{reg, reg}, {reg, lit}}}, {movh, {{reg, lit}}}, {movl, {{reg, lit}}}, {load, {{reg, mem}}}, {loadb, {{reg, mem}}}, 
    {loadbu, {{reg, mem}}}, {store, {{reg, mem}}}, {storeb, {{reg, mem}}}, {jmp, {{reg}, {iden}}}, {jc, {{iden}}}, {call, {{reg}, {iden}}}, 
    {ret, {{}}}, {push, {{reg}}}, {pop, {{reg}}}, {cmp, {{reg, reg}, {reg, lit}}}
};

const std::unordered_map<std::string, int> condCodes={
    {"je", 0}, {"jne", 1}, {"jg", 2}, {"ujg", 3}, {"jge", 4}, {"ujge", 5}, {"jl", 6}, 
    {"ujl", 7}, {"jle", 8}, {"ujle", 9}
};

const std::unordered_map<std::string, int> keywords={
    {"add", add}, {"sub", sub}, {"and", andGate}, {"or", orGate}, {"xor", xorGate}, {"not", notGate}, {"shl", shl}, 
    {"shr", shr}, {"ashr", ashr}, {"ror", ror}, {"mul", mul}, {"mulh", mul}, {"umulh", mul}, {"div", dvrm}, {"udiv", dvrm}, {"rem", dvrm}, 
    {"urem", dvrm}, {"mov", mov}, {"movh", movh}, {"movl", movl}, {"load", load}, {"loadb", loadb}, {"loadbu", loadbu}, 
    {"store", store}, {"storeb", storeb}, {"jmp", jmp}, {"je", jc}, {"jne", jc}, {"jg", jc}, {"ujg", jc}, {"jge", jc}, 
    {"ujge", jc}, {"jl", jc}, {"ujl", jc}, {"jle", jc}, {"ujle", jc}, {"call", call}, {"ret", ret}, {"push", push}, 
    {"pop", pop}, {"cmp", cmp}
};

const std::unordered_map<std::string, int> registers={
    {"r0", 0}, {"r1", 1}, {"r2", 2}, {"r3", 3}, {"r4", 4}, {"r5", 5}, {"r6", 6}, {"r7", 7}
};

const std::set<char> delimiters={
    ',', '[', ']', ':', ' ', '\t', ';'
};

bool inRange(int val, int a, int b);
