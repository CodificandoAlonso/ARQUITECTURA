//
// Created by beto on 4/10/24.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <iostream>
#include <vector>
using namespace std;

class ImageSOA {
public:
    ImageSOA(string input_file, string output_file, string optype, vector<int> args);
    ~ImageSOA();
    int process_operation();
private:
    string input_file;
    string output_file;
    string optype;
    vector<int> args;
    int maxlevel();
};



#endif //IMAGESOA_HPP
