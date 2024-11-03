//
// Created by beto on 4/10/24.
//

#include "imageaos.hpp"

#include "common/AVLTree.hpp"
#include "common/binario.hpp"
#include "common/progargs.hpp"
#include "common/struct-rgb.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <utility>
#include <vector>

static int const MAX_LEVEL = 65535;
static int const MIN_LEVEL = 255;
static int const BYTE      = 8;

using namespace std;

ImageAOS::ImageAOS(int argc, vector<string> const & argv) : Image(argc, argv) {
  // Constructor de la clase
}

int ImageAOS::process_operation() {
  // Primera operación: leer los metadatos de la imagen de entrada. Como
  // esta función es común a AOS y SOA, será implementada en la biblioteque "common"
  if (this->get_optype() == "info") {
    if (info() < 0) { return -1; }
  } else if (this->get_optype() == "maxlevel") {
    // Implementación de la operación de nivel máximo usando AOS (Array of Structures)
    if (maxlevel() < 0) { return -1; }
  } else if (this->get_optype() == "resize") {
    // Implementación de la operación de redimensionamiento usando AOS (Array of Structures)
    if (resize() < 0) { return -1; }
  } else {
    cerr << "Operación no soportada de momento: " << this->get_optype() << "\n";
    return -1;
  }
  return 0;
}

template <typename T>
array<T, 4> ImageAOS::rsz_obtain_square(vector<T> const & image,
                                        array<unsigned int, FIVE> args) const{
  auto xlength = static_cast<unsigned long>(args[0]);
  auto xheight = static_cast<unsigned long>(args[1]);
  auto ylength = static_cast<unsigned long>(args[2]);
  auto yheigth = static_cast<unsigned long>(args[3]);
  auto width   = static_cast<unsigned long>(args[4]);

  xlength = min(xlength, width - 1);
  xheight = min(xheight, width - 1);
  ylength = min(ylength, (image.size() / width) - 1);
  yheigth = min(yheigth, (image.size() / width) - 1);

  T const p_1 = image[(ylength * width) + xlength];
  T const p_2 = image[(ylength * width) + xheight];
  T const p_3 = image[(yheigth * width) + xlength];
  T const p_4 = image[(yheigth * width) + xheight];

  array<T, 4> const square = {p_1, p_2, p_3, p_4};
  return square;
}

template<typename T>
T ImageAOS::rsz_interpolate(double u_param, array<T, 4> square, double t_param) {
  T const c_1 = {
    .r = static_cast<unsigned char>(((1 - t_param) * square[0].r) + (t_param * square[1].r)),
    .g = static_cast<unsigned char>(((1 - t_param) * square[0].g) + (t_param * square[1].g)),
    .b = static_cast<unsigned char>(((1 - t_param) * square[0].b) + (t_param * square[1].b))};

  T const c_2 = {
    .r = static_cast<unsigned char>(((1 - t_param) * square[2].r) + (t_param * square[3].r)),
    .g = static_cast<unsigned char>(((1 - t_param) * square[2].g) + (t_param * square[3].g)),
    .b = static_cast<unsigned char>(((1 - t_param) * square[2].b) + (t_param * square[3].b))};

  T const c_param = {
    .r = static_cast<unsigned char>(((1 - u_param) * c_1.r) + (u_param * c_2.r)),
    .g = static_cast<unsigned char>(((1 - u_param) * c_1.g) + (u_param * c_2.g)),
    .b = static_cast<unsigned char>(((1 - u_param) * c_1.b) + (u_param * c_2.b))};

  return c_param;
}

vector<rgb_small> ImageAOS::read_image_rgb_small(ifstream & input_file) const {
  vector<rgb_small> image;
  for (int i = 0; i < this->get_width() * this->get_height(); i++) {
    rgb_small pixel{};
    pixel.r = read_binary_8(input_file);
    pixel.g = read_binary_8(input_file);
    pixel.b = read_binary_8(input_file);
    image.push_back(pixel);
  }
  return image;
}

vector<rgb_big> ImageAOS::read_image_rgb_big(ifstream & input_file) const {
  vector<rgb_big> image;
  for (int i = 0; i < this->get_width() * this->get_height(); i++) {
    rgb_big pixel{};
    pixel.r = swap16(read_binary_16(input_file));
    pixel.g = swap16(read_binary_16(input_file));
    pixel.b = swap16(read_binary_16(input_file));
    image.push_back(pixel);
  }
  return image;
}

int ImageAOS::resize_min(ofstream & output_file) {
  ifstream input_file  = this->get_if_input_file();
  int const width      = this->get_width();
  int const height     = this->get_height();
  int const new_width  = this->get_args()[0];
  int const new_height = this->get_args()[1];

  vector<rgb_small> const image = read_image_rgb_small(input_file);
  for (int y_prime = 0; y_prime < new_height; y_prime++) {
    for (int x_prime = 0; x_prime < new_width; x_prime++) {
      double const equis  = x_prime * static_cast<double>(width - 1) / (new_width - 1);
      double const ygreek = y_prime * static_cast<double>(height - 1) / (new_height - 1);

      auto xlength = static_cast<unsigned int>(floor(equis));
      auto xheight = static_cast<unsigned int>(ceil(equis));
      auto ylength = static_cast<unsigned int>(floor(ygreek));
      auto yheight = static_cast<unsigned int>(ceil(ygreek));

      array const args = {xlength, xheight, ylength, yheight, static_cast<unsigned int>(width)};
      array<rgb_small, 4> const square = rsz_obtain_square(image, args);

      double const t_param    = equis - xlength;
      double const u_param    = ygreek - ylength;
      rgb_small const c_param = rsz_interpolate(u_param, square, t_param);
      write_binary_8(output_file, c_param.r);
      write_binary_8(output_file, c_param.g);
      write_binary_8(output_file, c_param.b);
    }
  }
  return 0;
}

int ImageAOS::resize_max(ofstream & output_file) {
  ifstream input_file  = this->get_if_input_file();
  int const width      = this->get_width();
  int const height     = this->get_height();
  int const new_width  = this->get_args()[0];
  int const new_height = this->get_args()[1];

  vector<rgb_big> const image = read_image_rgb_big(input_file);
  for (int y_prime = 0; y_prime < new_height; y_prime++) {
    for (int x_prime = 0; x_prime < new_width; x_prime++) {
      double const equis  = x_prime * static_cast<double>(width - 1) / (new_width - 1);
      double const ygreek = y_prime * static_cast<double>(height - 1) / (new_height - 1);

      auto xlength = static_cast<unsigned int>(floor(equis));
      auto xheight = static_cast<unsigned int>(ceil(equis));
      auto ylength = static_cast<unsigned int>(floor(ygreek));
      auto yheight = static_cast<unsigned int>(ceil(ygreek));

      array const args = {xlength, xheight, ylength, yheight, static_cast<unsigned int>(width)};
      array<rgb_big, 4> const square = rsz_obtain_square(image, args);

      double const t_param  = equis - xlength;
      double const u_param  = ygreek - ylength;
      rgb_big const c_param = rsz_interpolate(u_param, square, t_param);
      write_binary_16(output_file, swap16(c_param.r));
      write_binary_16(output_file, swap16(c_param.g));
      write_binary_16(output_file, swap16(c_param.b));
    }
  }
  return 0;
}

int ImageAOS::resize() {
  get_imgdata();
  ofstream output_file(this->get_output_file(), ios::binary);

  if (!output_file) {
    cerr << "Error al abrir los archivos de entrada/salida"
         << "\n";
    return -1;
  }

  int const new_width  = this->get_args()[0];
  int const new_height = this->get_args()[1];
  int const maxval     = this->get_maxval();

  output_file << "P6 " << new_width << " " << new_height << " " << maxval << "\n";

  if (maxval <= MIN_LEVEL) {
    resize_min(output_file);
  } else if (maxval <= MAX_LEVEL) {
    resize_max(output_file);
  } else {
    cerr << "Error: valor máximo no soportado."
         << "\n";
    return -1;
  }

  output_file.close();
  return 0;
}

