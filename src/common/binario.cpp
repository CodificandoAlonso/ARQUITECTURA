//
// Created by beto on 4/10/24.
//

#include "binario.hpp"
#include <iostream>
#include <bitset>

using namespace std;
static constexpr int BYTE = 8;

/**
 * Esta función convierte dos bytes en un número de 16 bits (2 bytes).
 * De esta forma, si tenemos dos bytes 0x12 y 0x34, al aplicar esta función
 * obtendremos 0x3412.
 */
unsigned short merge16(unsigned char op1,unsigned char op2) {
    return (op2 << BYTE) | op1; ;
}
/**
 * Esta función intercambia los bytes de un número de 16 bits (2 bytes).
 * De esta forma, si tenemos un número 0x1234, al aplicar esta función
 * obtendremos 0x3412.
 */
unsigned short swap16(unsigned short op) {
    return (op >> BYTE) | (op << BYTE);;
}


string mix3char(char ch1, char ch2, char ch3) {
    bitset<BYTE> const byte1(ch1);
    bitset <BYTE> const byte2(ch2);
    bitset <BYTE> const byte3(ch3);
    return byte1.to_string() + byte2.to_string() + byte3.to_string();

}
