//
// Created by beto on 4/10/24.
//

#include "binario.hpp"

#include <bitset>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>

static constexpr unsigned char BYTE = 8;
static constexpr unsigned char FFF  = 0xFF;
static constexpr __uint16_t FFFF    = 0xFFFF;

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

uint64_t packRGBIG(uint16_t const red, uint16_t const grn, uint16_t const blu) {
  return (static_cast<uint64_t>(red) << (BYTE * 4)) | (static_cast<uint64_t>(grn) << (BYTE * 2)) | blu;
}

uint32_t packRGB(uint8_t const red, uint8_t const grn, uint8_t const blu) {
  return static_cast<uint32_t>(red) << (BYTE * 2) | static_cast<uint32_t>(grn) << BYTE | blu;
}

uint8_t extractred(uint32_t const color) {
  return color >> (BYTE * 2) & FFF;
}

uint8_t extractgreen(uint32_t const color) {
  return color >> BYTE & FFF;
}

uint8_t extractblue(uint32_t const color) {
  return color & FFF;
}

uint16_t extractredBIG(uint64_t const color) {
  return (color >> (BYTE * 4)) & FFFF;
}

uint16_t extractgreenBIG(uint64_t const color) {
  return (color >> (BYTE * 2)) & FFFF;
}

uint16_t extractblueBIG(uint64_t const color) {
  return color & FFFF;
}

double get_distance(__uint32_t item_1, __uint32_t item_2) {
  __uint8_t const red1 = extractred(item_1);
  __uint8_t const grn1 = extractgreen(item_1);
  __uint8_t const blu1 = extractblue(item_1);
  __uint8_t const red2 = extractred(item_2);
  __uint8_t const grn2 = extractgreen(item_2);
  __uint8_t const blu2 = extractblue(item_2);
  return sqrt(pow(red1 - red2, 2) + pow(grn1 - grn2, 2) + pow(blu1 - blu2, 2));
}

double get_distance_BIG(__uint64_t item_1, __uint64_t item_2) {
  __uint16_t const red1 = extractredBIG(item_1);
  __uint16_t const grn1 = extractgreenBIG(item_1);
  __uint16_t const blu1 = extractblueBIG(item_1);
  __uint16_t const red2 = extractredBIG(item_2);
  __uint16_t const grn2 = extractgreenBIG(item_2);
  __uint16_t const blu2 = extractblueBIG(item_2);
  return sqrt(pow(red1 - red2, 2) + pow(grn1 - grn2, 2) + pow(blu1 - blu2, 2));
}

