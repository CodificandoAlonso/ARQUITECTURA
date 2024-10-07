//
// Created by beto on 4/10/24.
//

#ifndef PROGARGS_HPP
#define PROGARGS_HPP
#include <iostream>
#include <vector>
class Checker   {
    public:
        string input_file;
        string output_file;
        string optype;
        vector<int> args;

        int check_args(int argc, char *argv[]);
};

#endif //PROGARGS_HPP
