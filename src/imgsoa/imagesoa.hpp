//
// Created by beto on 4/10/24.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include "common/progargs.hpp"

#include <common/struct-rgb.hpp>
#include <map>
#include <vector>
using namespace std;

class ImageSOA : public Image {
  public:
    ImageSOA(int argc, vector<string> const & argv);
    int process_operation();
  private:
    bool obtain_args();
    map<string, int> load_and_map_8();
    [[nodiscard]] int resize();
    [[nodiscard]] int compress();
    [[nodiscard]] int cutfreq();
    string input_file;
    string output_file;
    ifstream if_input_file;
    ofstream of_output_file;
    string format ;
    int width =0;
    int height =0 ;
    int maxval=0;
};

#endif  // IMAGESOA_HPP
