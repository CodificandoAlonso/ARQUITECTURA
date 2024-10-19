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
    vector<char> r;
    vector<char> g;
    vector<char> b;
    [[nodiscard]] size_t find_color(rgb_small color) const;
};

struct soa_rgb_big {
    vector<unsigned short> r;
    vector<unsigned short> g;
    vector<unsigned short> b;
    [[nodiscard]] size_t find_color(rgb_big color) const;
};

#endif //STRUCT_RGB_HPP
