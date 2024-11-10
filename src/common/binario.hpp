//
// Created by beto on 4/10/24.
//

#ifndef BINARIO_HPP
#define BINARIO_HPP
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

using namespace std;
uint8_t read_binary_8(ifstream  & input);
uint16_t read_binary_16(ifstream & input);

void write_binary_8(ofstream  &output, uint8_t value);

void write_binary_16(ofstream & output, uint16_t value);

void write_binary_32(ofstream & output, uint32_t value);

unsigned short merge16(unsigned char op1,unsigned char op2);

unsigned short swap16(unsigned short opr);

string mix3char(unsigned char ch1, unsigned char ch2, unsigned char ch3);
__uint8_t extractblue(__uint32_t color);
__uint8_t extractgreen(__uint32_t color);
__uint8_t extractred(__uint32_t color);
uint16_t extractredBIG(uint64_t color);
uint16_t extractgreenBIG(uint64_t color);
uint16_t extractblueBIG(uint64_t color);

uint64_t packRGBIG(uint16_t red, uint16_t grn, uint16_t blu);
uint32_t packRGB(uint8_t red, uint8_t grn, uint8_t blu);
double distance_to_black(__uint32_t color);
double get_distance(__uint32_t item_1, __uint32_t item_2);
#endif //BINARIO_HPP
