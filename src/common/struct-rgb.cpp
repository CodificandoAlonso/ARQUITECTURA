//
// Created by hector on 9/10/24.
//



#include<iostream>
#include<vector>
using namespace std;


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

