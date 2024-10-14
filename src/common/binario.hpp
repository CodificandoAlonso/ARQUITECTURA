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
        int partition(vector<pair<string, int>>& vec, int low, int high);

        void quicksort(vector<pair<string, int>>& vec, int low, int high);
}

#endif //BINARIO_HPP
