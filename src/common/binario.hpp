//
// Created by beto on 4/10/24.
//

#ifndef BINARIO_HPP
#define BINARIO_HPP
#include <vector>
#include <istream>
using namespace std;

unsigned short merge16(unsigned char op1, unsigned char op2);

unsigned short swap16(unsigned short op);

class Filereader {

    Filereader(int origin_maxlevel, int output_maxlevel, ifstream * file);
    void r_s_file();
private:
    int origin_maxlevel;
    int output_maxlevel;
    ifstream *file;

};
#endif //BINARIO_HPP
