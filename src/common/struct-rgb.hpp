//
// Created by hector on 9/10/24.
//

#ifndef STRUCT_RGB_HPP
#define STRUCT_RGB_HPP

#include <vector>
using namespace std;

struct rgb_small {
    char r;
    char g;
    char b;
};

struct rgb_big {
    unsigned short r;
    unsigned short g;
    unsigned short b;
};

struct soa_rgb_small {
    vector<unsigned char> r;
    vector<unsigned char> g;
    vector<unsigned char> b;
    void print(unsigned int index);
};

struct soa_rgb_big {
    vector<unsigned short> r;
    vector<unsigned short> g;
    vector<unsigned short> b;
};

#endif //STRUCT_RGB_HPP
