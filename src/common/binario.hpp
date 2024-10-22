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
uint8_t read_binary_8(ifstream & input);
uint16_t read_binary_16(ifstream & input);

void write_binary_8(ofstream & output, uint8_t value);

void write_binary_16(ofstream & output, uint16_t value);

void write_binary_32(ofstream & output, uint32_t value);

unsigned short merge16(unsigned char op1,unsigned char op2);

unsigned short swap16(unsigned short opr);

string mix3char(char ch1, char ch2, char ch3);
__uint8_t extractblue(const string& rgb);
__uint8_t extractgreen(const string& rgb);
__uint8_t extractred(const string& rgb);
namespace quick {
        size_t partition(vector<pair<string, int>>& vec, size_t low, size_t high);

        void quicksort(vector<pair<string, int>>& vec, size_t low, size_t high);
}

#endif //BINARIO_HPP
