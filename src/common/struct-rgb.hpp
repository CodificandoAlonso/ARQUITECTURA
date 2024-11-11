//
// Created by hector on 9/10/24.
//

#ifndef STRUCT_RGB_HPP
#define STRUCT_RGB_HPP

#include <vector>
using namespace std;

struct rgb_small {
    unsigned char r= 0;
    unsigned char g= 0;
    unsigned char b= 0;
};

struct rgb_big {
    unsigned short r = 0;
    unsigned short g= 0;
    unsigned short b= 0;
};

struct soa_rgb_small {
    vector<unsigned char> r = {}; // NOLINT(*-redundant-member-init)
    vector<unsigned char> g = {};// NOLINT(*-redundant-member-init)
    vector<unsigned char> b = {};// NOLINT(*-redundant-member-init)
};

struct soa_rgb_big {
    vector<unsigned short> r = {};// NOLINT(*-redundant-member-init)
    vector<unsigned short> g= {};// NOLINT(*-redundant-member-init)
    vector<unsigned short> b= {};// NOLINT(*-redundant-member-init)
};

#endif //STRUCT_RGB_HPP
