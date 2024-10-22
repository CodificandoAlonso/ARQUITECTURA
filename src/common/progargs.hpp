//
// Created by beto on 4/10/24.
//

#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include <string>
#include <vector>
using namespace std;

class Image {
public:
    Image(int argc, const vector<string> &argv);
    int check_args();
    int info();
    int maxlevel();
    [[nodiscard]] string get_input_file() const { return input_file; }
    [[nodiscard]] string get_output_file() const { return output_file; }
    [[nodiscard]] string get_optype() const { return optype; }
    [[nodiscard]] vector<int> get_args() const { return args; }
private:
    string input_file;
    string output_file;
    string optype;
    vector<int> args;
    int argc;
    vector<string> argv;
};

#endif //PROGARGS_HPP
