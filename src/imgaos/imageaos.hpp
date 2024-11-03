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
    static array<rgb_small, 4> rsz_obtain_square_min(vector<rgb_small> const & image,
                                                     array<unsigned int, FIVE> args);
    static rgb_small rsz_interpolate_min(double u_param, array<rgb_small, 4> square,
                                         double t_param);
    int resize_max(ofstream & output_file);
    static array<rgb_big, 4> rsz_obtain_square_max(vector<rgb_big> const & image,
                                                   array<unsigned int, FIVE> args);
    static rgb_big rsz_interpolate_max(double u_param, array<rgb_big, 4> square, double t_param);
    vector<rgb_small> read_image_rgb_small(ifstream & input_file) const;
    vector<rgb_big> read_image_rgb_big(ifstream & input_file) const;
};

#endif  // IMAGEAOS_HPP
