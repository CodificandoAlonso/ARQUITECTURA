//
// Created by beto on 4/10/24.
//

#include "binario.hpp"
#include <fstream>
#include <cstdint>


using namespace std;
static constexpr int BYTE = 8;

/**
 *
 */
uint16_t read_binary_16(ifstream & input) {
  uint16_t value = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char *>(&value), sizeof(value));
  return value;
}

void write_binary_16(ofstream & output, uint16_t value) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char *>(&value), sizeof(value));
}
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
