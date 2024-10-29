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
    map<string, int> load_and_map_8();
    [[nodiscard]] int resize();
    [[nodiscard]] int compress();
    [[nodiscard]] int cutfreq();
};

#endif  // IMAGESOA_HPP
