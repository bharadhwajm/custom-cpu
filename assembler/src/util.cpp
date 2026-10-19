#include "../include/util.h"

bool inRange(int val, int a, int b){
    if (!(val>=a && val<=b)){
        return false;
    }
    return true;
}
