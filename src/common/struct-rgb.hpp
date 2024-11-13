//
// Created by hector on 9/10/24.
//

#ifndef STRUCT_RGB_HPP
#define STRUCT_RGB_HPP

#include <vector>
using namespace std;

struct rgb_small {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
};

struct rgb_big {
    unsigned short r = 0;
    unsigned short g = 0;
    unsigned short b = 0;
};

struct soa_rgb_small {
    vector<unsigned char> r;
    vector<unsigned char> g;
    vector<unsigned char> b;
};

struct soa_rgb_big {
    vector<unsigned short> r;
    vector<unsigned short> g;
    vector<unsigned short> b;
};

#endif  // STRUCT_RGB_HPP
