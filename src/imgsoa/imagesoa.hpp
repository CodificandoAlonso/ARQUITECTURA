//
// Created by beto on 4/10/24.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <iostream>
#include <vector>
#include "common/progargs.hpp"
using namespace std;

class ImageSOA : public Checker {
public:
    ImageSOA(int argc, char *argv[]);  // Constructor
~ImageSOA();
int process_operation();
int maxlevel();
};

#endif //IMAGESOA_HPP
