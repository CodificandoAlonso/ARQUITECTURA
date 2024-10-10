//
// Created by beto on 4/10/24.
//

#include "binario.hpp"
#include "struct-rgb.hpp"


using namespace std;


Filereader::Filereader(int origin_maxlevel, int output_maxlevel, ifstream *file) : origin_maxlevel(origin_maxlevel), output_maxlevel(output_maxlevel), file(file) {

}

void Filereader::r_s_file() {

}




/**
 * Esta función convierte dos bytes en un número de 16 bits (2 bytes).
 * De esta forma, si tenemos dos bytes 0x12 y 0x34, al aplicar esta función
 * obtendremos 0x3412.
 */
unsigned short merge16(unsigned char op1, unsigned char op2) {
    return (op2 << 8) | op1; ;
}
/**
 * Esta función intercambia los bytes de un número de 16 bits (2 bytes).
 * De esta forma, si tenemos un número 0x1234, al aplicar esta función
 * obtendremos 0x3412.
 */
unsigned short swap16(unsigned short op) {
    return (op >> 8) | (op << 8);;
}


