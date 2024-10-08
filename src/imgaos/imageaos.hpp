//
// Created by beto on 4/10/24.
//

#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <iostream>
#include <vector>

class ImageAOS {
public:
    ImageAOS(std::string input_file, std::string output_file, std::string optype, std::vector<int> args);
    ~ImageAOS();
    int process_operation();
private:
    std::string input_file;
    std::string output_file;
    std::string optype;
    std::vector<int> args;
    int escalate_intensity();
};

#endif //IMAGEAOS_HPP
