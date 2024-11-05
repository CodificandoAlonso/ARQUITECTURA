//
// Created by beto on 4/10/24.
//

#include "binario.hpp"
#include <iostream>
#include <bitset>
#include <cstdint>
#include <fstream>
#include <cmath>

static constexpr unsigned char BYTE_2 = 16;
static constexpr unsigned char BYTE = 8;
static constexpr unsigned char BYTE_4 = 32;
static constexpr unsigned char FFF = 0xFF;
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

unsigned short swap16(unsigned short opr) {
  return static_cast<unsigned short>((opr >> BYTE) | (opr << BYTE));
}


string mix3char(const unsigned char ch1, const unsigned char ch2, const unsigned char ch3) {
      bitset<BYTE> const byte1(ch1);
      bitset<BYTE> const byte2(ch2);
      bitset<BYTE> const byte3(ch3);
    return byte1.to_string() + byte2.to_string() + byte3.to_string();

}


uint64_t packRGBIG(uint16_t const red, uint16_t const grn, uint16_t const blu) {
  return (static_cast<uint64_t>(red) << BYTE_4) | (static_cast<uint64_t>(grn) << BYTE_2) | blu;
}



uint32_t packRGB(uint8_t const red, uint8_t const grn, uint8_t const blu) {
  return static_cast<uint32_t>(red) << BYTE_2 | static_cast<uint32_t>(grn) << BYTE | blu;
}



uint8_t extractred(uint32_t const color) {
  return color >> BYTE_2 & FFF;
}

uint8_t extractgreen(uint32_t const color) {
  return color >> BYTE & FFF;
}

uint8_t extractblue(uint32_t const color) {
  return color & FFF;
}

double distance_to_black(__uint32_t const color) {
  __uint8_t const red = color >> BYTE_2 & FFF;
  __uint8_t const grn = color >> BYTE & FFF;
  __uint8_t const blu = color & FFF;
  return sqrt(pow(red,2) + pow(grn, 2) + pow(blu, 2));
}


double get_distance(__uint32_t item_1, __uint32_t item_2) {
  __uint8_t const red1 = extractred(item_1);
  __uint8_t const grn1 = extractgreen(item_1);
  __uint8_t const blu1 = extractblue(item_1);
  __uint8_t const red2 = extractred(item_2);
  __uint8_t const grn2 = extractgreen(item_2);
  __uint8_t const blu2 = extractblue(item_2);
  return sqrt(pow(red1-red2, 2)+ pow(grn1-grn2, 2)+ pow(blu1-blu2, 2));

}

