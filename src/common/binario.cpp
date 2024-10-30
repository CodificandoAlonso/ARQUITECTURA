//
// Created by beto on 4/10/24.
//

#include "binario.hpp"
#include <iostream>
#include <bitset>
#include <cstdint>
#include <fstream>

static constexpr int BYTE_2 = 16;
static constexpr unsigned char BYTE = 8;
using namespace std;

uint8_t read_binary_8(ifstream & input) {
  uint8_t value = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char *>(&value), sizeof(value));
  return value;
}

uint16_t read_binary_16(ifstream & input) {
  uint16_t value = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char *>(&value), sizeof(value));
  return value;
}

void write_binary_8(ofstream & output, uint8_t value) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char *>(&value), sizeof(value));
}

void write_binary_16(ofstream & output, uint16_t value) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char *>(&value), sizeof(value));
}

void write_binary_32(ofstream & output, uint32_t value) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char *>(&value), sizeof(value));
}

/**
 * Esta función convierte dos bytes en un número de 16 bits (2 bytes).
 * De esta forma, si tenemos dos bytes 0x12 y 0x34, al aplicar esta función
 * obtendremos 0x3412.
 */

unsigned short merge16(unsigned char op1, unsigned char op2) {
  return static_cast<unsigned short>((op2 << BYTE) | op1);
}

/**
 * Esta función intercambia los bytes de un número de 16 bits (2 bytes).
 * De esta forma, si tenemos un número 0x1234, al aplicar esta función
 * obtendremos 0x3412.
 */


string mix3char(const unsigned char ch1, const unsigned char ch2, const unsigned char ch3) {
      bitset<BYTE> const byte1(ch1);
      bitset<BYTE> const byte2(ch2);
      bitset<BYTE> const byte3(ch3);
    return byte1.to_string() + byte2.to_string() + byte3.to_string();

}

__uint8_t extractblue(const string& rgb) {
  const string blue = rgb.substr(rgb.size() - BYTE, BYTE);
  bitset<BYTE> const bluebinary(blue);
  auto const blueint = static_cast<__uint8_t>(bluebinary.to_ulong());
  return blueint;
}

__uint8_t extractgreen(const string& rgb) {
  const string green = rgb.substr(rgb.size() - BYTE_2, BYTE);
  bitset<BYTE> const greenbinary(green);
  auto const greenint = static_cast<__uint8_t>(greenbinary.to_ulong());
  return greenint;
}

__uint8_t extractred(const string& rgb) {
  const string red = rgb.substr(0, BYTE);
  bitset<BYTE> const redbinary(red);
  auto const redint = static_cast<__uint8_t>(redbinary.to_ulong());
  return redint;
}

unsigned short swap16(unsigned short opr) {
  return static_cast<unsigned short>((opr >> BYTE) | (opr << BYTE));
}










