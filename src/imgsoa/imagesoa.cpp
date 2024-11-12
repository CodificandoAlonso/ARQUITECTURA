//
// Created by beto on 4/10/24.
//

#include "imagesoa.hpp"

#include "common/binario.hpp"
#include "common/progargs.hpp"
#include "common/struct-rgb.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <ranges>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <utility>
#include <vector>

static constexpr int MAX_LEVEL       = 65535;
static constexpr int MIN_LEVEL       = 255;
static constexpr int BYTE            = 8;
static constexpr int POCO            = 75;
static constexpr int MEDIO           = 150;
static constexpr int ALTO            = 240;
static constexpr size_t NUEVE        = 9;
static constexpr size_t SEIS         = 6;
static constexpr size_t SIETE        = 7;
static constexpr size_t OCHO         = 8;
static constexpr int MAX_DIST        = 100000;
static constexpr size_t CINCO        = 5;
static constexpr size_t DIEZ         = 10;
static constexpr size_t ONCE         = 11;
static constexpr size_t DOCE         = 12;
static constexpr size_t TRECE        = 13;
static constexpr size_t CATORCE      = 14;
static constexpr size_t QUINCE       = 15;
static constexpr size_t DIECISEIS    = 16;
static constexpr size_t DIECISIETE   = 17;
static constexpr size_t DIECIOCHO    = 18;
static constexpr size_t DIECINUEVE   = 19;
static constexpr size_t VEINTE       = 20;
static constexpr size_t VEINTIUNO    = 21;
static constexpr size_t VEINTIDOS    = 22;
static constexpr size_t VEINTITRES   = 23;
static constexpr size_t VEINTICUATRO = 24;
static constexpr size_t VEINTICINCO  = 25;
static constexpr size_t VEINTISEIS   = 26;

using namespace std;

ImageSOA::ImageSOA(int const argc, vector<string> const & argv) : Image(argc, argv) { }

int ImageSOA::process_operation() {
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
  } else if (this->get_optype() == "cutfreq") {
    if (cutfreq() < 0) { return -1; }
  } else if (this->get_optype() == "compress") {
    // Implementación de la operación de compresión usando AOS (Array of Structures)
    if (compress() < 0) { return -1; }
  } else {
    cerr << "Operación no soportada de momento: " << this->get_optype() << "\n";
    return -1;
  }
  return 0;
}

/**
 * Función para hallar los píxeles más cercanos
 */
array<rgb_small, 4> ImageSOA::rsz_obtain_square_min(soa_rgb_small const & image,
                                                    array<unsigned int, FIVE> args) {
  auto xlength = static_cast<unsigned long>(args[0]);
  auto xheight = static_cast<unsigned long>(args[1]);
  auto ylength = static_cast<unsigned long>(args[2]);
  auto yheigth = static_cast<unsigned long>(args[3]);
  auto width   = static_cast<unsigned long>(args[4]);

  xlength = min(xlength, width - 1);
  xheight = min(xheight, width - 1);
  ylength = min(ylength, (image.r.size() / width) - 1);
  yheigth = min(yheigth, (image.r.size() / width) - 1);

  // Obtenemos los 4 pixeles más cercanos
  rgb_small const p_1 = {.r = image.r[(ylength * width) + xlength],
                         .g = image.g[(ylength * width) + xlength],
                         .b = image.b[(ylength * width) + xlength]};

  rgb_small const p_2 = {.r = image.r[(ylength * width) + xheight],
                         .g = image.g[(ylength * width) + xheight],
                         .b = image.b[(ylength * width) + xheight]};

  rgb_small const p_3 = {.r = image.r[(yheigth * width) + xlength],
                         .g = image.g[(yheigth * width) + xlength],
                         .b = image.b[(yheigth * width) + xlength]};

  rgb_small const p_4 = {.r = image.r[(yheigth * width) + xheight],
                         .g = image.g[(yheigth * width) + xheight],
                         .b = image.b[(yheigth * width) + xheight]};

  array<rgb_small, 4> const square = {p_1, p_2, p_3, p_4};
  return square;
}

array<rgb_big, 4> ImageSOA::rsz_obtain_square_max(soa_rgb_big const & image,
                                                  array<unsigned int, FIVE> args) {
  auto xlength = static_cast<unsigned long>(args[0]);
  auto xheight = static_cast<unsigned long>(args[1]);
  auto ylength = static_cast<unsigned long>(args[2]);
  auto yheigth = static_cast<unsigned long>(args[3]);
  auto width   = static_cast<unsigned long>(args[4]);

  xlength = min(xlength, width - 1);
  xheight = min(xheight, width - 1);
  ylength = min(ylength, (image.r.size() / width) - 1);
  yheigth = min(yheigth, (image.r.size() / width) - 1);

  rgb_big const p_1 = {.r = image.r[(ylength * width) + xlength],
                       .g = image.g[(ylength * width) + xlength],
                       .b = image.b[(ylength * width) + xlength]};

  rgb_big const p_2 = {.r = image.r[(ylength * width) + xheight],
                       .g = image.g[(ylength * width) + xheight],
                       .b = image.b[(ylength * width) + xheight]};

  rgb_big const p_3 = {.r = image.r[(yheigth * width) + xlength],
                       .g = image.g[(yheigth * width) + xlength],
                       .b = image.b[(yheigth * width) + xlength]};

  rgb_big const p_4 = {.r = image.r[(yheigth * width) + xheight],
                       .g = image.g[(yheigth * width) + xheight],
                       .b = image.b[(yheigth * width) + xheight]};

  array<rgb_big, 4> const square = {p_1, p_2, p_3, p_4};
  return square;
}

rgb_small ImageSOA::rsz_interpolate_min(double u_param, array<rgb_small, 4> square,
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

rgb_big ImageSOA::rsz_interpolate_max(double u_param, array<rgb_big, 4> square, double t_param) {
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

soa_rgb_small ImageSOA::read_image_rgb_small(ifstream & input_file) const {
  soa_rgb_small image;
  for (int i = 0; i < this->get_width() * this->get_height(); i++) {
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;
    red               = read_binary_8(input_file);
    grn               = read_binary_8(input_file);
    blu               = read_binary_8(input_file);
    image.r.push_back(red);
    image.g.push_back(grn);
    image.b.push_back(blu);
  }
  return image;
}

soa_rgb_big ImageSOA::read_image_rgb_big(ifstream & input_file) const {
  soa_rgb_big image;
  for (int i = 0; i < this->get_width() * this->get_height(); i++) {
    unsigned short red = 0;
    unsigned short grn = 0;
    unsigned short blu = 0;
    red                = read_binary_16(input_file);
    grn                = read_binary_16(input_file);
    blu                = read_binary_16(input_file);
    image.r.push_back(swap16(red));
    image.g.push_back(swap16(grn));
    image.b.push_back(swap16(blu));
  }
  return image;
}

int ImageSOA::resize_min(ofstream & output_file) {
  ifstream input_file  = this->get_if_input_file();
  int const width      = this->get_width();
  int const height     = this->get_height();
  int const new_width  = this->get_args()[0];
  int const new_height = this->get_args()[1];

  soa_rgb_small const image = read_image_rgb_small(input_file);
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

int ImageSOA::resize_max(ofstream & output_file) {
  ifstream input_file  = this->get_if_input_file();
  int const width      = this->get_width();
  int const height     = this->get_height();
  int const new_width  = this->get_args()[0];
  int const new_height = this->get_args()[1];

  soa_rgb_big const image = read_image_rgb_big(input_file);
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

int ImageSOA::resize() {
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

unordered_map<__uint32_t, __uint16_t> ImageSOA::cf_load_and_map_8(int width, ifstream input_file,
                                                                  int height) {
  unordered_map<__uint32_t, __uint16_t> myMap;
  unsigned char red = 0;
  unsigned char grn = 0;
  unsigned char blu = 0;
  for (int i = 0; i < width * height; i++) {
    red = read_binary_8(input_file);
    grn = read_binary_8(input_file);
    blu = read_binary_8(input_file);
    if (__uint32_t const rgb = packRGB(red, grn, blu); myMap.contains(rgb)) {
      myMap[rgb]++;
    } else {
      myMap[rgb] = 1;
    }
    this->soa_small.r.push_back(red);
    this->soa_small.g.push_back(grn);
    this->soa_small.b.push_back(blu);
  }
  return myMap;
}

unordered_map<__uint64_t, __uint16_t> ImageSOA::cf_load_and_map_8BIG(int width, ifstream input_file,
                                                                     int height) {
  unsigned short red = 0;
  unsigned short grn = 0;
  unsigned short blu = 0;
  unordered_map<__uint64_t, __uint16_t> myMap;
  for (int i = 0; i < width * height; i++) {
    red = read_binary_16(input_file);
    grn = read_binary_16(input_file);
    blu = read_binary_16(input_file);
    red = swap16(red);
    grn = swap16(grn);
    blu = swap16(blu);
    if (__uint64_t const rgb = packRGBIG(red, grn, blu); myMap.contains(rgb)) {
      myMap[{rgb}]++;
    } else {
      myMap[{rgb}] = 1;
    }
    this->soa_big.r.push_back(red);
    this->soa_big.g.push_back(grn);
    this->soa_big.b.push_back(blu);
  }
  return myMap;
}

void ImageSOA::cf_add_nodes() {
  this->nod.push_back(packRGB(POCO, POCO, POCO));
  this->nod.push_back(packRGB(POCO, POCO, MEDIO));
  this->nod.push_back(packRGB(POCO, POCO, ALTO));
  this->nod.push_back(packRGB(POCO, MEDIO, POCO));
  this->nod.push_back(packRGB(POCO, MEDIO, MEDIO));
  this->nod.push_back(packRGB(POCO, MEDIO, ALTO));
  this->nod.push_back(packRGB(POCO, ALTO, POCO));
  this->nod.push_back(packRGB(POCO, ALTO, MEDIO));
  this->nod.push_back(packRGB(POCO, ALTO, ALTO));

  this->nod.push_back(packRGB(MEDIO, POCO, POCO));
  this->nod.push_back(packRGB(MEDIO, POCO, MEDIO));
  this->nod.push_back(packRGB(MEDIO, POCO, ALTO));
  this->nod.push_back(packRGB(MEDIO, MEDIO, POCO));
  this->nod.push_back(packRGB(MEDIO, MEDIO, MEDIO));
  this->nod.push_back(packRGB(MEDIO, MEDIO, ALTO));
  this->nod.push_back(packRGB(MEDIO, ALTO, POCO));
  this->nod.push_back(packRGB(MEDIO, ALTO, MEDIO));
  this->nod.push_back(packRGB(MEDIO, ALTO, ALTO));

  this->nod.push_back(packRGB(ALTO, POCO, POCO));
  this->nod.push_back(packRGB(ALTO, POCO, MEDIO));
  this->nod.push_back(packRGB(ALTO, POCO, ALTO));
  this->nod.push_back(packRGB(ALTO, MEDIO, POCO));
  this->nod.push_back(packRGB(ALTO, MEDIO, MEDIO));
  this->nod.push_back(packRGB(ALTO, MEDIO, ALTO));
  this->nod.push_back(packRGB(ALTO, ALTO, POCO));
  this->nod.push_back(packRGB(ALTO, ALTO, MEDIO));
  this->nod.push_back(packRGB(ALTO, ALTO, ALTO));
}

void ImageSOA::cf_add_nodes_BIG(__uint16_t const POCOBIG, __uint16_t const MEDIOBIG,
                                __uint16_t const ALTOBIG) {
  this->nodBIG.push_back(packRGBIG(POCOBIG, POCOBIG, POCOBIG));
  this->nodBIG.push_back(packRGBIG(POCOBIG, POCOBIG, MEDIOBIG));
  this->nodBIG.push_back(packRGBIG(POCOBIG, POCOBIG, ALTOBIG));
  this->nodBIG.push_back(packRGBIG(POCOBIG, MEDIOBIG, POCOBIG));
  this->nodBIG.push_back(packRGBIG(POCOBIG, MEDIOBIG, MEDIOBIG));
  this->nodBIG.push_back(packRGBIG(POCOBIG, MEDIOBIG, ALTOBIG));
  this->nodBIG.push_back(packRGBIG(POCOBIG, ALTOBIG, POCOBIG));
  this->nodBIG.push_back(packRGBIG(POCOBIG, ALTOBIG, MEDIOBIG));
  this->nodBIG.push_back(packRGBIG(POCOBIG, ALTOBIG, ALTOBIG));

  this->nodBIG.push_back(packRGBIG(MEDIOBIG, POCOBIG, POCOBIG));
  this->nodBIG.push_back(packRGBIG(MEDIOBIG, POCOBIG, MEDIOBIG));
  this->nodBIG.push_back(packRGBIG(MEDIOBIG, POCOBIG, ALTOBIG));
  this->nodBIG.push_back(packRGBIG(MEDIOBIG, MEDIOBIG, POCOBIG));
  this->nodBIG.push_back(packRGBIG(MEDIOBIG, MEDIOBIG, MEDIOBIG));
  this->nodBIG.push_back(packRGBIG(MEDIOBIG, MEDIOBIG, ALTOBIG));
  this->nodBIG.push_back(packRGBIG(MEDIOBIG, ALTOBIG, POCOBIG));
  this->nodBIG.push_back(packRGBIG(MEDIOBIG, ALTOBIG, MEDIOBIG));
  this->nodBIG.push_back(packRGBIG(MEDIOBIG, ALTOBIG, ALTOBIG));

  this->nodBIG.push_back(packRGBIG(ALTOBIG, POCOBIG, POCOBIG));
  this->nodBIG.push_back(packRGBIG(ALTOBIG, POCOBIG, MEDIOBIG));
  this->nodBIG.push_back(packRGBIG(ALTOBIG, POCOBIG, ALTOBIG));
  this->nodBIG.push_back(packRGBIG(ALTOBIG, MEDIOBIG, POCOBIG));
  this->nodBIG.push_back(packRGBIG(ALTOBIG, MEDIOBIG, MEDIOBIG));
  this->nodBIG.push_back(packRGBIG(ALTOBIG, MEDIOBIG, ALTOBIG));
  this->nodBIG.push_back(packRGBIG(ALTOBIG, ALTOBIG, POCOBIG));
  this->nodBIG.push_back(packRGBIG(ALTOBIG, ALTOBIG, MEDIOBIG));
  this->nodBIG.push_back(packRGBIG(ALTOBIG, ALTOBIG, ALTOBIG));
}

unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>>
    ImageSOA::cf_generate_graph() {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph;
  cf_add_nodes();
  graph[this->nod[0]] = {
    {this->nod[1], this->nod[3], this->nod[NUEVE]},
    {}
  };  // PPP
  graph[this->nod[1]] = {
    {this->nod[0], this->nod[2], this->nod[4], this->nod[DIEZ]},
    {}
  };  // PPM
  graph[this->nod[2]] = {
    {this->nod[1], this->nod[CINCO], this->nod[ONCE]},
    {}
  };  // PPA
  graph[this->nod[3]] = {
    {this->nod[0], this->nod[4], this->nod[SEIS], this->nod[DOCE]},
    {}
  };  // PMP
  graph[this->nod[4]] = {
    {this->nod[1], this->nod[3], this->nod[CINCO], this->nod[SIETE], this->nod[TRECE]},
    {}
  };  // PMM
  graph[this->nod[CINCO]] = {
    {this->nod[2], this->nod[4], this->nod[OCHO], this->nod[CATORCE]},
    {}
  };  // PMA
  graph[this->nod[SEIS]] = {
    {this->nod[3], this->nod[SIETE], this->nod[QUINCE]},
    {}
  };  // PAP
  return graph;
}

unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>>
    ImageSOA::cf_generate_graph_2(
        unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> & graph) {
  graph[this->nod[SIETE]] = {
    {this->nod[4], this->nod[SEIS], this->nod[OCHO], this->nod[DIECISEIS]},
    {}
  };  // PAM
  graph[this->nod[OCHO]] = {
    {this->nod[CINCO], this->nod[SIETE], this->nod[DIECISIETE]},
    {}
  };  // PAA

  graph[this->nod[NUEVE]] = {
    {this->nod[0], this->nod[DIEZ], this->nod[DOCE], this->nod[DIECIOCHO]},
    {}
  };  // MPP
  graph[this->nod[DIEZ]] = {
    {this->nod[1], this->nod[NUEVE], this->nod[ONCE], this->nod[TRECE], this->nod[DIECINUEVE]},
    {}
  };  // MPM
  graph[this->nod[ONCE]] = {
    {this->nod[2], this->nod[DIEZ], this->nod[CATORCE], this->nod[VEINTE]},
    {}
  };  // MPA
  graph[this->nod[DOCE]] = {
    {this->nod[3], this->nod[NUEVE], this->nod[TRECE], this->nod[QUINCE], this->nod[VEINTIUNO]},
    {}
  };  // MMP
  graph[this->nod[TRECE]] = {
    {this->nod[4], this->nod[DIEZ], this->nod[DOCE], this->nod[CATORCE], this->nod[DIECISEIS],
     this->nod[VEINTIDOS]},
    {}
  };  // MMM
  return graph;
}

unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>>
    ImageSOA::cf_generate_graph_3(
        unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> & graph) {
  graph[this->nod[CATORCE]] = {
    {this->nod[CINCO], this->nod[ONCE], this->nod[TRECE], this->nod[DIECISIETE],
     this->nod[VEINTITRES]},
    {}
  };  // MMA
  graph[this->nod[QUINCE]] = {
    {this->nod[SEIS], this->nod[DOCE], this->nod[DIECISEIS], this->nod[DIECIOCHO],
     this->nod[VEINTICUATRO]},
    {}
  };  // MAP
  graph[this->nod[DIECISEIS]] = {
    {this->nod[SIETE], this->nod[TRECE], this->nod[QUINCE], this->nod[DIECISIETE],
     this->nod[VEINTICINCO]},
    {}
  };  // MAM
  graph[this->nod[DIECISIETE]] = {
    {this->nod[OCHO], this->nod[CATORCE], this->nod[DIECISEIS], this->nod[VEINTISEIS]},
    {}
  };  // MAA

  graph[this->nod[DIECIOCHO]] = {
    {this->nod[NUEVE], this->nod[QUINCE], this->nod[DIECINUEVE]},
    {}
  };  // APP
  graph[this->nod[DIECINUEVE]] = {
    {this->nod[DIEZ], this->nod[DIECIOCHO], this->nod[VEINTE], this->nod[VEINTIDOS]},
    {}
  };  // APM
  graph[this->nod[VEINTE]] = {
    {this->nod[ONCE], this->nod[DIECINUEVE], this->nod[VEINTITRES]},
    {}
  };  // APA
  graph[this->nod[VEINTIUNO]] = {
    {this->nod[DOCE], this->nod[VEINTIDOS], this->nod[VEINTICUATRO]},
    {}
  };  // AMP
  return graph;
}

unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>>
    ImageSOA::cf_generate_graph_4(
        unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> & graph) {
  graph[this->nod[VEINTIDOS]] = {
    {this->nod[TRECE], this->nod[DIECINUEVE], this->nod[VEINTIUNO], this->nod[VEINTITRES],
     this->nod[VEINTICINCO]},
    {}
  };  // AMM
  graph[this->nod[VEINTITRES]] = {
    {this->nod[CATORCE], this->nod[VEINTE], this->nod[VEINTIDOS], this->nod[VEINTISEIS]},
    {}
  };  // AMA
  graph[this->nod[VEINTICUATRO]] = {
    {this->nod[QUINCE], this->nod[VEINTIUNO], this->nod[VEINTICINCO]},
    {}
  };  // AAP
  graph[this->nod[VEINTICINCO]] = {
    {this->nod[DIECISEIS], this->nod[VEINTIDOS], this->nod[VEINTICUATRO], this->nod[VEINTISEIS]},
    {}
  };  // AAM
  graph[this->nod[VEINTISEIS]] = {
    {this->nod[DIECISIETE], this->nod[VEINTITRES], this->nod[VEINTICINCO]},
    {}
  };  // AAA
  return graph;
}

unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>>
    ImageSOA::cf_generate_graph_BIG() {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph;
  int const maxval    = this->get_maxval();
  auto const newpoco  = static_cast<unsigned short>((POCO * maxval) / MAX_LEVEL);
  auto const newmedio = static_cast<unsigned short>((MEDIO * maxval) / MAX_LEVEL);
  auto const newalto  = static_cast<unsigned short>((ALTO * maxval) / MAX_LEVEL);
  cf_add_nodes_BIG(newpoco, newmedio, newalto);
  graph[this->nodBIG[0]] = {
    {this->nodBIG[1], this->nodBIG[3], this->nodBIG[NUEVE]},
    {}
  };  // PPP
  graph[this->nodBIG[1]] = {
    {this->nodBIG[0], this->nodBIG[2], this->nodBIG[4], this->nodBIG[DIEZ]},
    {}
  };  // PPM
  graph[this->nodBIG[2]] = {
    {this->nodBIG[1], this->nodBIG[CINCO], this->nodBIG[ONCE]},
    {}
  };  // PPA
  graph[this->nodBIG[3]] = {
    {this->nodBIG[0], this->nodBIG[4], this->nodBIG[SEIS], this->nodBIG[DOCE]},
    {}
  };  // PMP
  graph[this->nodBIG[4]] = {
    {this->nodBIG[1], this->nodBIG[3], this->nodBIG[CINCO], this->nodBIG[SIETE],
     this->nodBIG[TRECE]},
    {}
  };  // PMM
  graph[this->nodBIG[CINCO]] = {
    {this->nodBIG[2], this->nodBIG[4], this->nodBIG[OCHO], this->nodBIG[CATORCE]},
    {}
  };  // PMA
  graph[this->nodBIG[SEIS]] = {
    {this->nodBIG[3], this->nodBIG[SIETE], this->nodBIG[QUINCE]},
    {}
  };  // PAP
  return graph;
}

unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>>
    ImageSOA::cf_generate_graph_BIG_2(
        unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> & graph) {
  graph[this->nodBIG[SIETE]] = {
    {this->nodBIG[4], this->nodBIG[SEIS], this->nodBIG[OCHO], this->nodBIG[DIECISEIS]},
    {}
  };  // PAM
  graph[this->nodBIG[OCHO]] = {
    {this->nodBIG[CINCO], this->nodBIG[SIETE], this->nodBIG[DIECISIETE]},
    {}
  };  // PAA

  graph[this->nodBIG[NUEVE]] = {
    {this->nodBIG[0], this->nodBIG[DIEZ], this->nodBIG[DOCE], this->nodBIG[DIECIOCHO]},
    {}
  };  // MPP
  graph[this->nodBIG[DIEZ]] = {
    {this->nodBIG[1], this->nodBIG[NUEVE], this->nodBIG[ONCE], this->nodBIG[TRECE],
     this->nodBIG[DIECINUEVE]},
    {}
  };  // MPM
  graph[this->nodBIG[ONCE]] = {
    {this->nodBIG[2], this->nodBIG[DIEZ], this->nodBIG[CATORCE], this->nodBIG[VEINTE]},
    {}
  };  // MPA
  graph[this->nodBIG[DOCE]] = {
    {this->nodBIG[3], this->nodBIG[NUEVE], this->nodBIG[TRECE], this->nodBIG[QUINCE],
     this->nodBIG[VEINTIUNO]},
    {}
  };  // MMP
  graph[this->nodBIG[TRECE]] = {
    {this->nodBIG[4], this->nodBIG[DIEZ], this->nodBIG[DOCE], this->nodBIG[CATORCE],
     this->nodBIG[DIECISEIS], this->nodBIG[VEINTIDOS]},
    {}
  };  // MMM
  return graph;
}

unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>>
    ImageSOA::cf_generate_graph_BIG_3(
        unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> & graph) {
  graph[this->nodBIG[CATORCE]] = {
    {this->nodBIG[CINCO], this->nodBIG[ONCE], this->nodBIG[TRECE], this->nodBIG[DIECISIETE],
     this->nodBIG[VEINTITRES]},
    {}
  };  // MMA
  graph[this->nodBIG[QUINCE]] = {
    {this->nodBIG[SEIS], this->nodBIG[DOCE], this->nodBIG[DIECISEIS], this->nodBIG[DIECIOCHO],
     this->nodBIG[VEINTICUATRO]},
    {}
  };  // MAP
  graph[this->nodBIG[DIECISEIS]] = {
    {this->nodBIG[SIETE], this->nodBIG[TRECE], this->nodBIG[QUINCE], this->nodBIG[DIECISIETE],
     this->nodBIG[VEINTICINCO]},
    {}
  };
  // MAM
  graph[this->nodBIG[DIECISIETE]] = {
    {this->nodBIG[OCHO], this->nodBIG[CATORCE], this->nodBIG[DIECISEIS], this->nodBIG[VEINTISEIS]},
    {}
  };  // MAA

  graph[this->nodBIG[DIECIOCHO]] = {
    {this->nodBIG[NUEVE], this->nodBIG[QUINCE], this->nodBIG[DIECINUEVE]},
    {}
  };  // APP
  graph[this->nodBIG[DIECINUEVE]] = {
    {this->nodBIG[DIEZ], this->nodBIG[DIECIOCHO], this->nodBIG[VEINTE], this->nodBIG[VEINTIDOS]},
    {}
  };  // APM
  graph[this->nodBIG[VEINTE]] = {
    {this->nodBIG[ONCE], this->nodBIG[DIECINUEVE], this->nodBIG[VEINTITRES]},
    {}
  };  // APA
  graph[this->nodBIG[VEINTIUNO]] = {
    {this->nodBIG[DOCE], this->nodBIG[VEINTIDOS], this->nodBIG[VEINTICUATRO]},
    {}
  };  // AMP
  return graph;
}

unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>>
    ImageSOA::cf_generate_graph_BIG_4(
        unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> & graph) {
  graph[this->nodBIG[VEINTIDOS]] = {
    {this->nodBIG[TRECE], this->nodBIG[DIECINUEVE], this->nodBIG[VEINTIUNO],
     this->nodBIG[VEINTITRES], this->nodBIG[VEINTICINCO]},
    {}
  };  // AMM
  graph[this->nodBIG[VEINTITRES]] = {
    {this->nodBIG[CATORCE], this->nodBIG[VEINTE], this->nodBIG[VEINTIDOS],
     this->nodBIG[VEINTISEIS]},
    {}
  };  // AMA
  graph[this->nodBIG[VEINTICUATRO]] = {
    {this->nodBIG[QUINCE], this->nodBIG[VEINTIUNO], this->nodBIG[VEINTICINCO]},
    {}
  };  // AAP
  graph[this->nodBIG[VEINTICINCO]] = {
    {this->nodBIG[DIECISEIS], this->nodBIG[VEINTIDOS], this->nodBIG[VEINTICUATRO],
     this->nodBIG[VEINTISEIS]},
    {}
  };  // AAM
  graph[this->nodBIG[VEINTISEIS]] = {
    {this->nodBIG[DIECISIETE], this->nodBIG[VEINTITRES], this->nodBIG[VEINTICINCO]},
    {}
  };  // AAA
  return graph;
}

deque<pair<__uint32_t, __uint16_t>>
    ImageSOA::cf_check_first_part_small(unordered_map<__uint32_t, __uint16_t> myMap,
                                        unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                                        int & num_left) const {
  vector<pair<__uint32_t, __uint16_t>> myVector(myMap.begin(), myMap.end());
  ranges::sort(myVector, [](auto const & op1, auto const & op2) {
    return op1.second < op2.second;
  });
  // Me paso a size_t el numero de elementos a eliminar y me creo un vector delete
  vector<pair<__uint32_t, __uint16_t>> VectorDelete;
  auto const elems_to_delete = static_cast<size_t>(this->get_args()[0]);

  // Añado al vector delete el numero de elementos que pide
  for (size_t i = 0; i < elems_to_delete; i++) { VectorDelete.emplace_back(myVector[i]); }
  size_t tamDelete = elems_to_delete;

  // Mientras el siguiente al ultimo guardado tenga el mismo value, se añadira tmb
  while (myVector[tamDelete].second == VectorDelete[elems_to_delete - 1].second) {
    VectorDelete.emplace_back(myVector[tamDelete]);
    tamDelete++;
  }

  int const pivot  = VectorDelete[elems_to_delete - 1].second;
  int elem_deleted = 0;
  for (auto & [fst, snd] : VectorDelete) {
    if (snd < pivot) {
      Deleteitems[{fst}] = 0;
      elem_deleted++;
    }
  }

  int const new_n    = static_cast<int>(elems_to_delete);
  num_left           = new_n - elem_deleted;
  auto const new_e_d = static_cast<long int>(elem_deleted);  // elem_deleted

  deque const left_elems(VectorDelete.begin() + new_e_d, VectorDelete.end());
  return left_elems;
}

deque<pair<__uint64_t, __uint16_t>>
    ImageSOA::cf_check_first_part_BIG(unordered_map<__uint64_t, __uint16_t> myMapBIG,
                                      unordered_map<__uint64_t, __uint64_t> & Deleteitems,
                                      int & num_left) const {
  vector<pair<__uint64_t, __uint16_t>> myVector(myMapBIG.begin(), myMapBIG.end());
  ranges::sort(myVector, [](auto const & op1, auto const & op2) {
    return op1.second < op2.second;
  });

  // Me paso a size_t el numero de elementos a eliminar y me creo un vector delete
  vector<pair<__uint64_t, __uint16_t>> VectorDelete;
  auto const elems_to_delete = static_cast<size_t>(this->get_args()[0]);

  // Añado al vector delete el numero de elementos que pide
  for (size_t i = 0; i < elems_to_delete; i++) { VectorDelete.emplace_back(myVector[i]); }
  size_t tamDelete = elems_to_delete;

  // Mientras el siguiente al ultimo guardado tenga el mismo value, se añadira tmb
  while (myVector[tamDelete].second == VectorDelete[elems_to_delete - 1].second) {
    VectorDelete.emplace_back(myVector[tamDelete]);
    tamDelete++;
  }

  int const pivot  = VectorDelete[elems_to_delete - 1].second;
  int elem_deleted = 0;
  for (auto & [fst, snd] : VectorDelete) {
    if (snd < pivot) {
      Deleteitems[{fst}] = 0;
      elem_deleted++;
    }
  }

  int const new_n    = static_cast<int>(elems_to_delete);
  num_left           = new_n - elem_deleted;
  auto const new_e_d = static_cast<long int>(elem_deleted);  // elem_deleted

  deque const left_elems(VectorDelete.begin() + new_e_d, VectorDelete.end());
  return left_elems;
}

void ImageSOA::cf_write_in_exit(unordered_map<__uint32_t, __uint32_t> Deleteitems) {
  write_out(this->get_maxval());
  ofstream output_file = this->get_of_output_file();
  auto const iter      = this->soa_small.r.size();

  for (size_t counter = 0; counter < iter; counter++) {
    __uint8_t red = this->soa_small.r[counter];
    __uint8_t grn = this->soa_small.g[counter];
    __uint8_t blu = this->soa_small.b[counter];
    if (__uint32_t const rgb = packRGB(red, grn, blu); Deleteitems.contains(rgb)) {
      red = extractred(Deleteitems[rgb]);
      grn = extractgreen(Deleteitems[rgb]);
      blu = extractblue(Deleteitems[rgb]);
    }
    write_binary_8(output_file, red);
    write_binary_8(output_file, grn);
    write_binary_8(output_file, blu);
  }

  output_file.close();
}

void ImageSOA::cf_write_in_exit_BIG(unordered_map<__uint64_t, __uint64_t> Deleteitems) {
  write_out(this->get_maxval());
  ofstream output_file = this->get_of_output_file();
  auto const iter      = this->soa_big.r.size();

  for (size_t counter = 0; counter < iter; counter++) {
    __uint16_t red = this->soa_big.r[counter];
    __uint16_t grn = this->soa_big.g[counter];
    __uint16_t blu = this->soa_big.b[counter];
    if (__uint64_t const rgb = packRGBIG(red, grn, blu); Deleteitems.contains(rgb)) {
      red = extractredBIG(Deleteitems[rgb]);
      grn = extractgreenBIG(Deleteitems[rgb]);
      blu = extractblueBIG(Deleteitems[rgb]);
    }
    write_binary_16(output_file, swap16(red));
    write_binary_16(output_file, swap16(grn));
    write_binary_16(output_file, swap16(blu));
  }
  output_file.close();
}

void ImageSOA::cf_search_in_graph_BIG(
    unordered_map<__uint64_t, __uint64_t> & Deleteitems,
    unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph) {
  for (auto & entry : Deleteitems) {
    __uint64_t const color_to_delete             = entry.first;
    double min_distance                          = MAX_DIST;
    unordered_map<__uint64_t, __uint8_t> visited = {};
    // Obtener el nodo correspondiente al color a eliminar
    auto node_it = graph.find(entry.second);
    if (node_it == graph.end()) { continue; }
    visited[entry.second] = 0;
    // Primero, verificar la distancia en el nodo principal
    // bool found_in_main_node = false;
    for (__uint64_t const candidate : node_it->second.second) {
      double const distance = get_distance_BIG(color_to_delete, candidate);
      if (distance < min_distance) {
        min_distance = distance;
        entry.second = candidate;
      }
    }
    cf_find_neigh_BIG const params_s   = {.color_to_delete = color_to_delete,
                                          .graph           = &graph,
                                          .neighbors       = &node_it->second.first,
                                          .min_distance    = &min_distance,
                                          .visited_node    = &visited};
    __uint64_t const replacement_color = cf_find_closest_in_neighbors_BIG(&params_s);
    // Si encontramos un reemplazo adecuado, guardarlo en el grafo y en Deleteitems
    if (replacement_color != 0) { entry.second = replacement_color; }
  }
}

void ImageSOA::cf_search_in_graph_small(
    unordered_map<__uint32_t, __uint32_t> & Deleteitems,
    unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph) {
  for (auto & entry : Deleteitems) {
    __uint32_t const color_to_delete             = entry.first;
    double min_distance                          = MAX_DIST;
    unordered_map<__uint32_t, __uint8_t> visited = {};
    // Obtener el nodo correspondiente al color a eliminar
    auto node_it = graph.find(entry.second);
    if (node_it == graph.end()) { continue; }
    visited[entry.second] = 0;
    // Primero, verificar la distancia en el nodo principal
    // bool found_in_main_node = false;
    for (__uint32_t const candidate : node_it->second.second) {
      double const distance = get_distance(color_to_delete, candidate);
      if (distance < min_distance) {
        min_distance = distance;
        entry.second = candidate;
      }
    }
    cf_find_neigh_small const params_s = {.color_to_delete = color_to_delete,
                                          .graph           = &graph,
                                          .neighbors       = &node_it->second.first,
                                          .min_distance    = &min_distance,
                                          .visited_node    = &visited};
    __uint32_t const replacement_color = cf_find_closest_in_neighbors(&params_s);
    // Si encontramos un reemplazo adecuado, guardarlo en el grafo y en Deleteitems
    if (replacement_color != 0) { entry.second = replacement_color; }
  }
}

void ImageSOA::cutfreq_min(unordered_map<__uint32_t, __uint16_t> const & myMap) {
  // Convertir myMap a vector de pares y ordenar
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  int num_left                          = 0;
  auto left_elems                       = cf_check_first_part_small(myMap, Deleteitems, num_left);
  params_same_vector_small const params = {
    .father_vector = left_elems, .value = 1, .counter = left_elems.size()};
  auto bluevalues = cf_same_bgr_vector(params);

  // Chequear y actualizar colores para eliminar
  Deleteitems = cf_check_colors_to_delete(Deleteitems, num_left, bluevalues);

  unordered_map<__uint32_t, __uint32_t> toSave;
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph =
      cf_generate_graph();
  cf_generate_graph_2(graph);
  cf_generate_graph_3(graph);
  cf_generate_graph_4(graph);

  // Configurar parámetros para la función de finalización del grafo
  params_finish_graph const params_graph = {
    .myMap = &myMap, .Deleteitems = &Deleteitems, .toSave = &toSave, .graph = &graph};
  cf_finish_graph(&params_graph);
  cf_search_in_graph_small(Deleteitems, graph);
  cf_write_in_exit(Deleteitems);
}

void ImageSOA::cutfreq_max(unordered_map<__uint64_t, __uint16_t> const & myMapBIG) {
  // Convierto myMap a vector de pares y ordeno
  unordered_map<__uint64_t, __uint64_t> Deleteitems;
  int num_left                          = 0;
  auto left_elems                       = cf_check_first_part_BIG(myMapBIG, Deleteitems, num_left);
  params_same_vector_BIG const params_b = {
    .father_vector = left_elems, .value = 1, .counter = left_elems.size()};
  auto bluevalues = cf_same_bgr_vector_BIG(params_b);
  // Para saber que elemento de bluevalues utilizar
  Deleteitems = cf_check_colors_to_delete_BIG(Deleteitems, num_left, bluevalues);
  unordered_map<__uint64_t, __uint64_t> toSave;
  // Me recorro las keys de myMap
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph =
      cf_generate_graph_BIG();
  cf_generate_graph_BIG_2(graph);
  cf_generate_graph_BIG_3(graph);
  cf_generate_graph_BIG_4(graph);
  params_finish_graph_BIG const params_graph = {
    .myMap = &myMapBIG, .Deleteitems = &Deleteitems, .toSave = &toSave, .graph = &graph};
  cf_finish_graph_BIG(&params_graph);
  cf_search_in_graph_BIG(Deleteitems, graph);
  cf_write_in_exit_BIG(Deleteitems);
}

int ImageSOA::cutfreq() {
  get_imgdata();
  ifstream input_file = this->get_if_input_file();

  if (!input_file) {
    cerr << "Error al abrir los archivos de entrada/salida"
         << "\n";
    return -1;
  }

  int const width  = this->get_width();
  int const height = this->get_height();
  int const maxval = this->get_maxval();
  // ofstream output_file(this->get_output_file(), ios::binary);
  if (maxval == MIN_LEVEL) {
    unordered_map<__uint32_t, __uint16_t> myMap;
    myMap                      = cf_load_and_map_8(width, std::move(input_file), height);
    auto const elems_to_delete = static_cast<size_t>(this->get_args()[0]);
    if (elems_to_delete >= myMap.size()) {
      cerr << "El numero de pixeles menos frecuentes a eliminar es mayor que el numero de "
              "pixeles unicos"
           << "\n";
      return -1;
    }
    cutfreq_min(myMap);
  } else {
    unordered_map<__uint64_t, __uint16_t> myMapBIG;
    myMapBIG                   = cf_load_and_map_8BIG(width, std::move(input_file), height);
    auto const elems_to_delete = static_cast<size_t>(this->get_args()[0]);
    if (elems_to_delete >= myMapBIG.size()) {
      cerr << "El numero de pixeles menos frecuentes a eliminar es mayor que el numero de "
              "pixeles unicos"
           << "\n";
      return -1;
    }
    cutfreq_max(myMapBIG);
  }
  return 0;
}

void ImageSOA::cp_export(ofstream & output_file,
                         unordered_map<unsigned int, unsigned int> const & color_map,
                         list<unsigned int> const & indexes) {
  unsigned long int const num_colors = color_map.size();
  for (unsigned int const index : indexes) {
    if (num_colors < static_cast<unsigned long int>(pow(2, BYTE))) {
      write_binary_8(output_file, static_cast<unsigned char>(index));
    } else if (num_colors < static_cast<unsigned long int>(pow(2, 2 * BYTE))) {
      write_binary_16(output_file, static_cast<uint16_t>(index));
    } else if (num_colors < static_cast<unsigned long int>(pow(2, 4 * BYTE))) {
      write_binary_32(output_file, static_cast<uint32_t>(index));
    } else {
      cerr << "Error: demasiados colores distintos.\n";
      return;
    }
  }
}

void ImageSOA::cp_export_BIG(ofstream & output_file,
                             unordered_map<unsigned long int, unsigned int> const & color_map,
                             list<unsigned int> const & indexes) {
  unsigned long int const num_colors = color_map.size();
  for (unsigned int const index : indexes) {
    if (num_colors < static_cast<unsigned long int>(pow(2, BYTE))) {
      write_binary_8(output_file, static_cast<unsigned char>(index));
    } else if (num_colors < static_cast<unsigned long int>(pow(2, 2 * BYTE))) {
      write_binary_16(output_file, static_cast<uint16_t>(index));
    } else if (num_colors < static_cast<unsigned long int>(pow(2, 4 * BYTE))) {
      write_binary_32(output_file, static_cast<uint32_t>(index));
    } else {
      cerr << "Error: demasiados colores distintos.\n";
      return;
    }
  }
}

int ImageSOA::compress_min() {
  ifstream input_file = this->get_if_input_file();
  ofstream output_file(this->get_output_file(), ios::binary);
  auto width  = static_cast<unsigned int>(this->get_width());
  auto height = static_cast<unsigned int>(this->get_height());
  unordered_map<unsigned int, unsigned int> color_map;
  list<unsigned int> indexes;
  soa_rgb_small unique_colors;
  for (unsigned int i = 0; i < width * height; i++) {
    unsigned char const red = read_binary_8(input_file);
    unsigned char const grn = read_binary_8(input_file);
    unsigned char const blu = read_binary_8(input_file);
    // unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
    unsigned int const concatenated = packRGB(red, grn, blu);
    if (!color_map.contains(concatenated)) {
      auto index              = static_cast<unsigned int>(unique_colors.r.size());
      color_map[concatenated] = index;
      unique_colors.r.push_back(red);
      unique_colors.g.push_back(grn);
      unique_colors.b.push_back(blu);
    }
    indexes.push_back(color_map[concatenated]);
  }
  output_file << "C6"
              << " " << width << " " << height << " " << this->get_maxval() << " "
              << unique_colors.r.size() << "\n";
  // Escribir los colores únicos
  for (unsigned int i = 0; i < unique_colors.r.size(); i++) {
    write_binary_8(output_file, unique_colors.r[i]);
    write_binary_8(output_file, unique_colors.g[i]);
    write_binary_8(output_file, unique_colors.b[i]);
  }
  cp_export(output_file, color_map, indexes);
  input_file.close();
  output_file.close();
  return 0;
}

int ImageSOA::compress_max() {
  ifstream input_file = this->get_if_input_file();
  ofstream output_file(this->get_output_file(), ios::binary);
  auto width  = static_cast<unsigned int>(this->get_width());
  auto height = static_cast<unsigned int>(this->get_height());
  unordered_map<unsigned long int, unsigned int> color_map;
  list<unsigned int> indexes;
  soa_rgb_big unique_colors;
  for (unsigned int i = 0; i < width * height; i++) {
    unsigned short const red = read_binary_16(input_file);
    unsigned short const grn = read_binary_16(input_file);
    unsigned short const blu = read_binary_16(input_file);
    // unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
    unsigned long int const concatenated = packRGBIG(red, grn, blu);
    if (!color_map.contains(concatenated)) {
      auto index              = static_cast<unsigned int>(unique_colors.r.size());
      color_map[concatenated] = index;
      unique_colors.r.push_back(red);
      unique_colors.g.push_back(grn);
      unique_colors.b.push_back(blu);
    }
    indexes.push_back(color_map[concatenated]);
  }
  output_file << "C6"
              << " " << width << " " << height << " " << this->get_maxval() << " "
              << unique_colors.r.size() << "\n";
  for (unsigned int i = 0; i < unique_colors.r.size(); i++) {
    write_binary_16(output_file, unique_colors.r[i]);
    write_binary_16(output_file, unique_colors.g[i]);
    write_binary_16(output_file, unique_colors.b[i]);
  }
  cp_export_BIG(output_file, color_map, indexes);
  input_file.close();
  output_file.close();
  return 0;
}

int ImageSOA::compress() {
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

