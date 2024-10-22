//
// Created by beto on 4/10/24.
//

#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Image {
public:
    Image(int argc, const vector<string> &argv);
    int check_args();
    [[nodiscard]] int info() const;
    int maxlevel();
    [[nodiscard]] ifstream get_input_file() const { return input_file; }
    [[nodiscard]] ofstream get_output_file() const { return output_file; }
    [[nodiscard]] string get_optype() const { return optype; }
    [[nodiscard]] vector<int> get_args() const { return args; }
private:
    void get_imgdata();
    void write_out(int level)
    void min_min() const;
    ifstream  input_file;
    ofstream  output_file;
    string optype;
    vector<int> args;
    int argc;
    vector<string> argv;
    string format ;
    int width ;
    int height ;
    int maxval;
};

#endif //PROGARGS_HPP
