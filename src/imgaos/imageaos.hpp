//
// Created by beto on 4/10/24.
//

#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <vector>
#include <common/progargs.hpp>

class ImageAOS : public Image {
public:
    ImageAOS(int argc, const std::vector<string>& argv);
    int process_operation();
    int maxlevel();
};

#endif //IMAGEAOS_HPP
