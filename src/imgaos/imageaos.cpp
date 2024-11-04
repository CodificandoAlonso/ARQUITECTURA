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
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
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
  } else if (this->get_optype() == "compress") {
    // Implementación de la operación de compresión usando AOS (Array of Structures)
    if (compress() < 0) { return -1; }
  } else {
    cerr << "Operación no soportada de momento: " << this->get_optype() << "\n";
    return -1;
  }
  return 0;
}

array<rgb_small, 4> ImageAOS::rsz_obtain_square_min(vector<rgb_small> const & image,
                                                    array<unsigned int, FIVE> args) {
  auto xlength = static_cast<unsigned long>(args[0]);
  auto xheight = static_cast<unsigned long>(args[1]);
  auto ylength = static_cast<unsigned long>(args[2]);
  auto yheigth = static_cast<unsigned long>(args[3]);
  auto width   = static_cast<unsigned long>(args[4]);

  xlength = min(xlength, width - 1);
  xheight = min(xheight, width - 1);
  ylength = min(ylength, (image.size() / width) - 1);
  yheigth = min(yheigth, (image.size() / width) - 1);

  rgb_small const p_1 = image[(ylength * width) + xlength];
  rgb_small const p_2 = image[(ylength * width) + xheight];
  rgb_small const p_3 = image[(yheigth * width) + xlength];
  rgb_small const p_4 = image[(yheigth * width) + xheight];

  array<rgb_small, 4> const square = {p_1, p_2, p_3, p_4};
  return square;
}

array<rgb_big, 4> ImageAOS::rsz_obtain_square_max(vector<rgb_big> const & image,
                                                  array<unsigned int, FIVE> args) {
  auto xlength = static_cast<unsigned long>(args[0]);
  auto xheight = static_cast<unsigned long>(args[1]);
  auto ylength = static_cast<unsigned long>(args[2]);
  auto yheigth = static_cast<unsigned long>(args[3]);
  auto width   = static_cast<unsigned long>(args[4]);

  xlength = min(xlength, width - 1);
  xheight = min(xheight, width - 1);
  ylength = min(ylength, (image.size() / width) - 1);
  yheigth = min(yheigth, (image.size() / width) - 1);

  rgb_big const p_1 = image[(ylength * width) + xlength];
  rgb_big const p_2 = image[(ylength * width) + xheight];
  rgb_big const p_3 = image[(yheigth * width) + xlength];
  rgb_big const p_4 = image[(yheigth * width) + xheight];

  array<rgb_big, 4> const square = {p_1, p_2, p_3, p_4};
  return square;
}

rgb_small ImageAOS::rsz_interpolate_min(double u_param, array<rgb_small, 4> square,
                                        double t_param) {
  rgb_small const c_1 = {
    .r = static_cast<unsigned char>(((1 - t_param) * square[0].r) + (t_param * square[1].r)),
    .g = static_cast<unsigned char>(((1 - t_param) * square[0].g) + (t_param * square[1].g)),
    .b = static_cast<unsigned char>(((1 - t_param) * square[0].b) + (t_param * square[1].b))};

  rgb_small const c_2 = {
    .r = static_cast<unsigned char>(((1 - t_param) * square[2].r) + (t_param * square[3].r)),
    .g = static_cast<unsigned char>(((1 - t_param) * square[2].g) + (t_param * square[3].g)),
    .b = static_cast<unsigned char>(((1 - t_param) * square[2].b) + (t_param * square[3].b))};

  rgb_small const c_param = {
    .r = static_cast<unsigned char>(((1 - u_param) * c_1.r) + (u_param * c_2.r)),
    .g = static_cast<unsigned char>(((1 - u_param) * c_1.g) + (u_param * c_2.g)),
    .b = static_cast<unsigned char>(((1 - u_param) * c_1.b) + (u_param * c_2.b))};

  return c_param;
}

rgb_big ImageAOS::rsz_interpolate_max(double u_param, array<rgb_big, 4> square, double t_param) {
  rgb_big const c_1 = {
    .r = static_cast<unsigned short>(((1 - t_param) * square[0].r) + (t_param * square[1].r)),
    .g = static_cast<unsigned short>(((1 - t_param) * square[0].g) + (t_param * square[1].g)),
    .b = static_cast<unsigned short>(((1 - t_param) * square[0].b) + (t_param * square[1].b))};

  rgb_big const c_2 = {
    .r = static_cast<unsigned short>(((1 - t_param) * square[2].r) + (t_param * square[3].r)),
    .g = static_cast<unsigned short>(((1 - t_param) * square[2].g) + (t_param * square[3].g)),
    .b = static_cast<unsigned short>(((1 - t_param) * square[2].b) + (t_param * square[3].b))};

  rgb_big const c_param = {
    .r = static_cast<unsigned short>(((1 - u_param) * c_1.r) + (u_param * c_2.r)),
    .g = static_cast<unsigned short>(((1 - u_param) * c_1.g) + (u_param * c_2.g)),
    .b = static_cast<unsigned short>(((1 - u_param) * c_1.b) + (u_param * c_2.b))};

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
      array<rgb_small, 4> const square = rsz_obtain_square_min(image, args);

      double const t_param    = equis - xlength;
      double const u_param    = ygreek - ylength;
      rgb_small const c_param = rsz_interpolate_min(u_param, square, t_param);
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
      array<rgb_big, 4> const square = rsz_obtain_square_max(image, args);

      double const t_param  = equis - xlength;
      double const u_param  = ygreek - ylength;
      rgb_big const c_param = rsz_interpolate_max(u_param, square, t_param);
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

void ImageAOS::cp_export_min(ofstream & output_file, AVLTree tree,
                             vector<rgb_small> const & image) {
  unsigned long int const num_colors = image.size();
  ifstream input_file_rep(this->get_input_file(), ios::binary);
  string format;
  int maxval          = 0;
  unsigned int width  = 0;
  unsigned int height = 0;
  input_file_rep >> format >> width >> height >> maxval;
  input_file_rep.ignore(1);
  for (unsigned int i = 0; i < width * height; i++) {
    unsigned char const red = read_binary_8(input_file_rep);
    unsigned char const grn = read_binary_8(input_file_rep);
    unsigned char const blu = read_binary_8(input_file_rep);

    unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
    element const elem              = tree.search(concatenated);
    if (num_colors < static_cast<unsigned long int>(pow(2, BYTE))) {
      write_binary_8(output_file, static_cast<unsigned char>(elem.index));
    } else if (num_colors < static_cast<unsigned long int>(pow(2, 2 * BYTE))) {
      write_binary_16(output_file, static_cast<uint16_t>(elem.index));
    } else if (num_colors < static_cast<unsigned long int>(pow(2, 4 * BYTE))) {
      write_binary_32(output_file, static_cast<uint32_t>(elem.index));
    }
  }
  input_file_rep.close();
}

void ImageAOS::cp_export_max(ofstream & output_file, AVLTree tree, vector<rgb_big> const & image) {
  unsigned long int const num_colors = image.size();
  ifstream input_file_rep(this->get_input_file(), ios::binary);
  string format;
  int maxval          = 0;
  unsigned int width  = 0;
  unsigned int height = 0;
  input_file_rep >> format >> width >> height >> maxval;
  input_file_rep.ignore(1);
  for (unsigned int i = 0; i < width * height; i++) {
    unsigned short const red = read_binary_16(input_file_rep);
    unsigned short const grn = read_binary_16(input_file_rep);
    unsigned short const blu = read_binary_16(input_file_rep);

    unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
    element const elem              = tree.search(concatenated);
    if (num_colors < static_cast<unsigned long int>(pow(2, BYTE))) {
      write_binary_8(output_file, static_cast<unsigned char>(elem.index));
    } else if (num_colors < static_cast<unsigned long int>(pow(2, 2 * BYTE))) {
      write_binary_16(output_file, static_cast<uint16_t>(elem.index));
    } else if (num_colors < static_cast<unsigned long int>(pow(2, 4 * BYTE))) {
      write_binary_32(output_file, static_cast<uint32_t>(elem.index));
    }
  }
  input_file_rep.close();
}

int ImageAOS::compress_min() {
  ifstream input_file = this->get_if_input_file();
  ofstream output_file(this->get_output_file(), ios::binary);
  auto width  = static_cast<unsigned int>(this->get_width());
  auto height = static_cast<unsigned int>(this->get_height());
  AVLTree tree;
  vector<rgb_small> image;
  for (unsigned int i = 0; i < width * height; i++) {
    unsigned char const red         = read_binary_8(input_file);
    unsigned char const grn         = read_binary_8(input_file);
    unsigned char const blu         = read_binary_8(input_file);
    long unsigned int const index   = image.size();
    unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
    element const elem = {.color = concatenated, .index = static_cast<unsigned int>(index)};
    if (tree.insert(elem) == 0) { image.push_back({.r = red, .g = grn, .b = blu}); }
  }
  if (image.size() > static_cast<unsigned long int>(pow(2, 3 * BYTE))) {
    cerr << "Error: demasiados colores distintos."
         << "\n";
    return -1;
  }
  output_file << "C6"
              << " " << width << " " << height << " " << this->get_maxval() << " " << image.size()
              << "\n";
  for (auto & pixel : image) {
    write_binary_8(output_file, pixel.r);
    write_binary_8(output_file, pixel.g);
    write_binary_8(output_file, pixel.b);
  }
  cp_export_min(output_file, tree, image);
  input_file.close();
  output_file.close();
  return 0;
}

int ImageAOS::compress_max() {
  ifstream input_file = this->get_if_input_file();
  ofstream output_file(this->get_output_file(), ios::binary);
  auto width  = static_cast<unsigned int>(this->get_width());
  auto height = static_cast<unsigned int>(this->get_height());
  AVLTree tree;
  vector<rgb_big> image;
  for (unsigned int i = 0; i < width * height; i++) {
    unsigned short const red        = read_binary_16(input_file);
    unsigned short const grn        = read_binary_16(input_file);
    unsigned short const blu        = read_binary_16(input_file);
    long unsigned int const index   = image.size();
    unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
    element const elem = {.color = concatenated, .index = static_cast<unsigned int>(index)};
    if (tree.insert(elem) == 0) { image.push_back({.r = red, .g = grn, .b = blu}); }
  }
  if (image.size() > static_cast<unsigned long int>(pow(2, 4 * BYTE))) {
    cerr << "Error: demasiados colores distintos."
         << "\n";
    return -1;
  }
  output_file << "C6"
              << " " << width << " " << height << " " << this->get_maxval() << " " << image.size()
              << "\n";
  for (auto & pixel : image) {
    write_binary_16(output_file, pixel.r);
    write_binary_16(output_file, pixel.g);
    write_binary_16(output_file, pixel.b);
  }
  cp_export_max(output_file, tree, image);
  input_file.close();
  output_file.close();
  return 0;
}

int ImageAOS::compress() {
  get_imgdata();

  auto maxval = static_cast<unsigned int>(this->get_maxval());

  if (maxval <= MIN_LEVEL) {
    if (compress_min() < 0) { return -1; }
  } else if (maxval <= MAX_LEVEL) {
    if (compress_max() < 0) { return -1; }
  } else {
    cerr << "Error: maxval no soportado"
         << "\n";
    return -1;
  }

  return 0;
}

