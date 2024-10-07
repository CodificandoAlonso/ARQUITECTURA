//
// Created by beto on 4/10/24.
//

#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include <string>
#include <vector>

class Checker {
public:
    std::string input_file;
    std::string output_file;
    std::string optype;
    std::vector<int> args;

    Checker(int argc, char *argv[]);
    int check_args();
private:
    int argc;
    char **argv;
};

#endif //PROGARGS_HPP
