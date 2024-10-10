//
// Created by hector on 9/10/24.
//

#ifndef STRUCT_RGB_HPP
#define STRUCT_RGB_HPP

struct rgb_small {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct rgb_big {
    unsigned short r;
    unsigned short g;
    unsigned short b;
};

struct soa_rgb_small {
    vector<rgb_small> r;
    vector<rgb_small> g;
    vector<rgb_small> b;
};

struct soa_rgb_big {
    vector<rgb_big> r;
    vector<rgb_big> g;
    vector<rgb_big> b;
};

#endif //STRUCT_RGB_HPP
