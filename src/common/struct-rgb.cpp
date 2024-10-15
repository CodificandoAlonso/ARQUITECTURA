//
// Created by hector on 9/10/24.
//



#include<vector>
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
};

struct soa_rgb_big {
    vector<unsigned short> r;
    vector<unsigned short> g;
    vector<unsigned short> b;
};

