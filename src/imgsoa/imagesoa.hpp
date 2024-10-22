//
// Created by beto on 4/10/24.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include "common/progargs.hpp"

#include <vector>

class ImageSOA : public Image {
  public:
    ImageSOA(int argc, std::vector<string> const & argv);
    int process_operation();
  private:
    [[nodiscard]] int resize() const;
    [[nodiscard]] int compress() const;
    [[nodiscard]] int cutfreq() const;
};

#endif  // IMAGESOA_HPP
