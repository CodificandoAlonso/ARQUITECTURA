//
// Created by beto on 4/10/24.
//

#ifndef BINARIO_HPP
#define BINARIO_HPP
#include <string>
#include <vector>
using namespace std;

unsigned short merge16(unsigned char op1,unsigned char op2);

unsigned short swap16(unsigned short op);

string mix3char(char ch1, char ch2, char ch3);
namespace quick {
        size_t partition(vector<pair<string, int>>& vec, size_t low, size_t high);

        void quicksort(vector<pair<string, int>>& vec, size_t low, size_t high);
}

#endif //BINARIO_HPP
