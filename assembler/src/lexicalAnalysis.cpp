#include <iostream>
#include "../include/lexicalAnalysis.h"

bool lexicalAnalysis(const std::string &line, std::vector<token> &list, int lineNo){
    int pos=0;
    while (pos<line.length()){
        while (line[pos]==' ' || line[pos]=='\t'){
            pos+=1;
        }
        if (pos==line.length()) break;
        if((line[pos]>='a' && line[pos]<='z')||(line[pos]>='A' && line[pos]<='Z')||(line[pos]=='_')){
            int start=pos;
            pos+=1;
            while ((line[pos]>='a' && line[pos]<='z')||(line[pos]>='A' && line[pos]<='Z')||(line[pos]=='_')||((line[pos]>='0' && line[pos]<='9'))){
                pos+=1;
            }
            token t;
            t.token=line.substr(start, pos-start);
            if (keywords.contains(t.token)){
                t.type=keyword;
            }else if (registers.contains(t.token)){
                t.type=reg;
            }else{
                t.type=iden;
            }
            list.push_back(t);
            continue;
        }else if (line[pos]>='0' && line[pos]<='9'){
            int start=pos;
            if (line[start]=='0' && (line[pos+1]=='b' || line[pos+1]=='B')){
                pos+=2;
                while (pos<line.length()){
                    if (delimiters.contains(line[pos])) break;
                    if (line[pos]=='0' || line[pos]=='1'){
                        pos+=1;
                    }else{
                        std::cout<<"Invalid literal: "<<line.substr(start, pos-start+1)<<" at line: "<<lineNo<<std::endl;
                        return false;
                    }
                }
            }else if (line[start]=='0' && (line[pos+1]=='x' || line[pos+1]=='X')){
                pos+=2;
                while (pos<line.length()){
                    if (delimiters.contains(line[pos])) break;
                    if ((line[pos]>='0' && line[pos]<='9')||(line[pos]>='a' && line[pos]<='f')||
                        (line[pos]>='A' && line[pos]<='F')){
                        pos+=1;
                    }else{
                        std::cout<<"Invalid literal: "<<line.substr(start, pos-start+1)<<" at line: "<<lineNo<<std::endl;
                        return false;
                    }
                }
            }else{
                while (pos<line.length()){
                    if (delimiters.contains(line[pos])) break;
                    if ((line[pos]>='0' && line[pos]<='9')){
                        pos+=1;
                    }else{
                        std::cout<<"Invalid literal: "<<line.substr(start, pos-start+1)<<" at line: "<<lineNo<<std::endl;
                        return false;
                    }
                }
            }
            token t;
            t.token=line.substr(start, pos-start);
            t.type=lit;
            list.push_back(t);
            continue;
        }else if (line[pos]=='['){
            token t;
            t.token="[";
            t.type=lparan;
            list.push_back(t);
            pos+=1;
            continue;
        }else if (line[pos]==']'){
            token t;
            t.token="]";
            t.type=rparan;
            list.push_back(t);
            pos+=1;
            continue;
        }else if (line[pos]==':'){
            token t;
            t.token=":";
            t.type=colon;
            list.push_back(t);
            pos+=1;
            continue;
        }else if (line[pos]==','){
            token t;
            t.token=",";
            t.type=comma;
            list.push_back(t);
            pos+=1;
            continue;
        }else if (line[pos]=='+'){
            token t;
            t.token="+";
            t.type=plus;
            list.push_back(t);
            pos+=1;
            continue;
        }else if (line[pos]=='-'){
            token t;
            t.token="-";
            t.type=minus;
            list.push_back(t);
            pos+=1;
            continue;
        }else if (line[pos]==';'){
            break;
        }
        std::cout<<"Invalid character: "<<line[pos]<<" at line: "<<lineNo<<std::endl;
        return false;
    }
    return true;
}
