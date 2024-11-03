//
// Created by beto on 4/10/24.
//

#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include "common/progargs.hpp"

#include <common/AVLTree.hpp>
#include <common/struct-rgb.hpp>
#include <unordered_map>
#include <vector>

using namespace std;
static int constexpr FIVE = 5;

class ImageAOS : public Image {
  public:
    ImageAOS(int argc, std::vector<string> const & argv);
    int process_operation();

  private:
    [[nodiscard]] int resize();
    int resize_min(ofstream & output_file);
    int resize_max(ofstream & output_file);
    vector<rgb_small> read_image_rgb_small(ifstream & input_file) const;
    vector<rgb_big> read_image_rgb_big(ifstream & input_file) const;

    template<typename T>
    array<T, 4> rsz_obtain_square(vector<T> const & image, array<unsigned int, FIVE> args) const;

    template<typename T>
    static T rsz_interpolate(double u_param, array<T, 4> square, double t_param);
};

#endif  // IMAGEAOS_HPP
