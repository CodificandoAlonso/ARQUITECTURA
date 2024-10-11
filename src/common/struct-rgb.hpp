//
// Created by hector on 9/10/24.
//

#ifndef STRUCT_RGB_HPP
#define STRUCT_RGB_HPP

struct rgb_small {
    char r;
    char g;
    char b;
};

struct rgb_big {
    short r;
    short g;
    short b;
};

struct soa_rgb_small {
    vector<char> r;
    vector<char> g;
    vector<char> b;
};

struct soa_rgb_big {
    vector<short> r;
    vector<short> g;
    vector<short> b;
};

#endif //STRUCT_RGB_HPP
