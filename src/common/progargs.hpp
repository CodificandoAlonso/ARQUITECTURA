//
// Created by beto on 4/10/24.
//

#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include <string>
#include <vector>
using namespace std;

class Checker {
public:
    string input_file;
    string output_file;
    string optype;
    vector<int> args;

    Checker(int argc, char *argv[]);
    int check_args();
private:
    int argc;
    char **argv;
};

#endif //PROGARGS_HPP
