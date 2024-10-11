//
// Created by beto on 4/10/24.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <vector>
#include "common/progargs.hpp"

using namespace std;

class ImageSOA : public Image {
public:
    ImageSOA(int argc, const vector<string>& argv);
    int process_operation();
    int maxlevel();
};

#endif //IMAGESOA_HPP
