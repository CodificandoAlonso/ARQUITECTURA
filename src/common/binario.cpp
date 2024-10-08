//
// Created by beto on 4/10/24.
//

#include "binario.hpp"

using namespace std;

unsigned short convert_from_2byte_to_short(unsigned char op1, unsigned char op2) {
    unsigned short result = (op2 << 8) | op1;
    return result ;
}

unsigned short convert_from_short_to_2byte(unsigned short op) {
    unsigned short result = (op >> 8) | (op << 8);
    return result;
}