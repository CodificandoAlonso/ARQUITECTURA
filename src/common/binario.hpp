//
// Created by beto on 4/10/24.
//

#ifndef BINARIO_HPP
#define BINARIO_HPP

#include <fstream>
#include <cstdint>

using namespace std;

uint16_t read_binary_16(ifstream & input);

void write_binary_8(ofstream & output, uint8_t value);

void write_binary_16(ofstream & output, uint16_t value);

void write_binary_32(ofstream & output, uint32_t value);

unsigned short merge16(unsigned char op1,unsigned char op2);

unsigned short swap16(unsigned short op);

#endif //BINARIO_HPP
