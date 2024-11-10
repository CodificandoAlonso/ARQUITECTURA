//
// Created by beto on 4/10/24.
//

#include "imagesoa.hpp"

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
#include <list>
#include <map>
#include <ranges>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <utility>
#include <vector>

static constexpr double calc_index = 10;
static constexpr int MAX_LEVEL     = 65535;
static constexpr int MIN_LEVEL     = 255;
static constexpr int BYTE          = 8;
static constexpr size_t CIEN       = 20000;
static constexpr int POCO = 75;
static constexpr int MEDIO = 150;
static constexpr int ALTO = 240;
static constexpr size_t NUEVE = 9;
static constexpr size_t SEIS = 6;
static constexpr size_t SIETE = 7;
static constexpr size_t OCHO = 8;
static constexpr int MAX_DIST = 100000;
static constexpr size_t CINCO = 5;
static constexpr size_t DIEZ = 10;
static constexpr size_t ONCE = 11;
static constexpr size_t DOCE = 12;
static constexpr size_t TRECE = 13;
static constexpr size_t CATORCE = 14;
static constexpr size_t QUINCE = 15;
static constexpr size_t DIECISEIS = 16;
static constexpr size_t DIECISIETE = 17;
static constexpr size_t DIECIOCHO = 18;
static constexpr size_t DIECINUEVE = 19;
static constexpr size_t VEINTE = 20;
static constexpr size_t VEINTIUNO = 21;
static constexpr size_t VEINTIDOS = 22;
static constexpr size_t VEINTITRES = 23;
static constexpr size_t VEINTICUATRO = 24;
static constexpr size_t VEINTICINCO = 25;
static constexpr size_t VEINTISEIS = 26;

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

vector<__uint32_t> ImageSOA::sort_and_map_keys(unordered_map<__uint32_t, __uint16_t> const & myMap,
                                               unordered_map<__uint32_t, size_t> & color_to_index) {
  // Crear un vector de claves (colores) de myMap
  vector<__uint32_t> sorted_colors;
  sorted_colors.reserve(myMap.size());
  for (auto const & entry : myMap) { sorted_colors.push_back(entry.first); }

  // Ordenar el vector de colores por distancia al negro
  ranges::sort(sorted_colors.begin(), sorted_colors.end(),
               [](__uint32_t const & fst, __uint32_t const & scnd) {
                 return distance_to_black(fst) < distance_to_black(scnd);
               });

  // Mapear cada color a su índice en el vector ordenado
  for (size_t item = 0; item < sorted_colors.size(); ++item) {
    color_to_index[sorted_colors[item]] = item;
  }

  return sorted_colors;
}

unordered_map<__uint64_t, __uint16_t> ImageSOA::load_and_map_8BIG(int width, ifstream input_file,
                                                                  int height) {
  unsigned short red = 0;
  unsigned short grn = 0;
  unsigned short blu = 0;
  unordered_map<__uint64_t, __uint16_t> myMap;
  for (int i = 0; i < width * height; i++) {
    red = read_binary_16(input_file);
    grn = read_binary_16(input_file);
    blu = read_binary_16(input_file);
    if (__uint64_t const rgb = packRGBIG(red, grn, blu); myMap.contains(rgb)) {
      myMap[{rgb}]++;
    } else {
      myMap[{rgb}] = 1;
    }
    cout << "PORRO";
    /*
    this->soa_big.r.push_back(red);
    this->soa_big.g.push_back(grn);
    this->soa_big.b.push_back(blu);
    */
  }
  return myMap;
}

deque<pair<__uint32_t, __uint16_t>>
    ImageSOA::same_bgr_vector(deque<pair<__uint32_t, __uint16_t>> father_vector, int const value,
                              size_t const counter) {
  // Value será 1 para blue, 2 para green y 3 para red
  deque<pair<__uint32_t, __uint16_t>> color_vector;
  __uint8_t color = 0;
  for (size_t i = 0; i < counter; i++) {
    if (value == 1) { color = extractblue(father_vector[i].first); }
    if (value == 2) { color = extractgreen(father_vector[i].first); }
    if (value == 3) { color = extractred(father_vector[i].first); }
    color_vector.emplace_back(father_vector[i].first, color);
  }
  ranges::sort(color_vector, [](auto const & op1, auto const & op2) {
    return op1.second > op2.second;
  });
  return color_vector;
}

int ImageSOA::check_and_delete(deque<pair<__uint32_t, __uint16_t>> & color_vector, int const color,
                               unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                               deque<pair<__uint32_t, __uint16_t>> & bluevalues) {
  size_t my_index = 0;
  // 1 para azul, 0 para verde
  size_t meanwhile = 0;
  while (color_vector[meanwhile].second == color_vector[meanwhile + 1].second) { meanwhile++; }
  if (meanwhile == 1) {
    __uint8_t value0 = 0;
    __uint8_t value1 = 0;
    if (color == 1) {
      value0 = extractgreen(color_vector[0].first);
      value1 = extractgreen(color_vector[1].first);
    } else {
      value0 = extractred(color_vector[0].first);
      value1 = extractred(color_vector[1].first);
    }
    if (value0 - value1 > 0) {
      Deleteitems[{color_vector[0].first}] = 0;
      // color_vector.pop_front();
      my_index = search_in_blue(bluevalues, color_vector[0].first);
      delete_from_deque(bluevalues, my_index);
    } else {
      Deleteitems[{color_vector[1].first}] = 0;
      my_index                             = search_in_blue(bluevalues, color_vector[1].first);
      delete_from_deque(bluevalues, my_index);
    }
    return 0;
  }
  return static_cast<int>(meanwhile + 1);
}

void ImageSOA::delete_from_deque(deque<pair<__uint32_t, __uint16_t>> & deque_general,
                                 size_t index) {
  swap(deque_general[0], deque_general[index]);
  deque_general.pop_front();
}

size_t ImageSOA::search_in_blue(deque<pair<__uint32_t, unsigned short>> & pairs,
                                __uint32_t & first) {
  for (size_t i = 0; i < pairs.size(); i++) {
    if (pairs[i].first == first) { return i; }
  }
  return 0;
}

unordered_map<__uint32_t, __uint32_t>
    ImageSOA::check_colors_to_delete(unordered_map<__uint32_t, __uint32_t> Deleteitems,
                                     int num_left, deque<pair<__uint32_t, __uint16_t>> bluevalues) {
  size_t my_index = 0;
  while (num_left > 0) {
    if(bluevalues.size() >1){
      if (bluevalues[0].second == bluevalues[1].second) {
        if (int my_meanwhile = check_and_delete(bluevalues, 1, Deleteitems, bluevalues);
            my_meanwhile > 0) {
          if (my_meanwhile < num_left) {
            auto iterator = static_cast<size_t>(my_meanwhile);
            for (size_t iii = 0; iii < iterator; iii++) {
              Deleteitems[bluevalues[0].first] = 0;
              bluevalues.pop_front();
              num_left--;}
          } else {
            auto greenvalues = same_bgr_vector(bluevalues, 2, static_cast<size_t>(my_meanwhile));
            if (greenvalues[0].second == greenvalues[1].second) {
              my_meanwhile = check_and_delete(greenvalues, 0, Deleteitems, bluevalues);
              if (my_meanwhile > 0) {
                auto redvalues = same_bgr_vector(greenvalues, 3, static_cast<size_t>(my_meanwhile));
                Deleteitems[{redvalues[0].first}] = 0;
                my_index                          = search_in_blue(bluevalues, redvalues[0].first);
                delete_from_deque(bluevalues, my_index);
                num_left--;
              } else {
                num_left--;}
            } else {
              Deleteitems[{greenvalues[0].first}] = 0;
              my_index                            = search_in_blue(bluevalues, greenvalues[0].first);
              delete_from_deque(bluevalues, my_index);
              num_left--;}}} else {
              num_left--;}} else {
        Deleteitems[{bluevalues[0].first}] = 0;
        bluevalues.pop_front();
        num_left--;}}
    else {
      Deleteitems[{bluevalues[0].first}] = 0;
      bluevalues.pop_front();
      num_left--;}}
  return Deleteitems;
}

void ImageSOA::addEdge(unordered_map<__uint32_t, vector<__uint32_t>> & graph, const __uint32_t key1, const __uint32_t key2) {
  graph[key1].push_back(key2);
  graph[key2].push_back(key1);
}


void ImageSOA::add_nodes() {
    this->nod.push_back( packRGB(POCO, POCO, POCO));
  this->nod.push_back( packRGB(POCO, POCO, MEDIO));
  this->nod.push_back( packRGB(POCO, POCO, ALTO));
  this->nod.push_back( packRGB(POCO, MEDIO, POCO));
 this->nod.push_back( packRGB(POCO, MEDIO, MEDIO));
  this->nod.push_back( packRGB(POCO, MEDIO, ALTO));
  this->nod.push_back( packRGB(POCO, ALTO, POCO));
  this->nod.push_back( packRGB(POCO, ALTO, MEDIO));
  this->nod.push_back( packRGB(POCO, ALTO, ALTO));

  this->nod.push_back( packRGB(MEDIO, POCO, POCO));
  this->nod.push_back( packRGB(MEDIO, POCO, MEDIO));
  this->nod.push_back( packRGB(MEDIO, POCO, ALTO));
  this->nod.push_back( packRGB(MEDIO, MEDIO, POCO));
  this->nod.push_back( packRGB(MEDIO, MEDIO, MEDIO));
  this->nod.push_back(   packRGB(MEDIO, MEDIO, ALTO));
  this->nod.push_back( packRGB(MEDIO, ALTO, POCO));
  this->nod.push_back( packRGB(MEDIO, ALTO, MEDIO));
  this->nod.push_back( packRGB(MEDIO, ALTO, ALTO));

  this->nod.push_back( packRGB(ALTO, POCO, POCO));
  this->nod.push_back( packRGB(ALTO, POCO, MEDIO));
  this->nod.push_back( packRGB(ALTO, POCO, ALTO));
  this->nod.push_back( packRGB(ALTO, MEDIO, POCO));
  this->nod.push_back( packRGB(ALTO, MEDIO, MEDIO));
  this->nod.push_back( packRGB(ALTO, MEDIO, ALTO));
  this->nod.push_back( packRGB(ALTO, ALTO, POCO));
  this->nod.push_back( packRGB(ALTO, ALTO, MEDIO));
  this->nod.push_back( packRGB(ALTO, ALTO, ALTO));
}



unordered_map<__uint32_t,pair<vector<__uint32_t>, vector<__uint32_t>>> ImageSOA::cf_generate_graph() {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>>graph;
    add_nodes();
   graph[this->nod[0]] = {{this->nod[1], this->nod[3], this->nod[NUEVE]}, {}}; // PPP
    graph[this->nod[1]] = {{this->nod[0], this->nod[2], this->nod[4], this->nod[DIEZ]}, {}}; // PPM
    graph[this->nod[2]] = {{this->nod[1], this->nod[CINCO], this->nod[ONCE]}, {}}; // PPA
    graph[this->nod[3]] = {{this->nod[0], this->nod[4], this->nod[SEIS], this->nod[DOCE]}, {}}; // PMP
    graph[this->nod[4]] = {{this->nod[1], this->nod[3], this->nod[CINCO], this->nod[SIETE], this->nod[TRECE]}, {}}; // PMM
    graph[this->nod[CINCO]] = {{this->nod[2], this->nod[4], this->nod[OCHO], this->nod[CATORCE]}, {}}; // PMA
    graph[this->nod[SEIS]] = {{this->nod[3], this->nod[SIETE], this->nod[QUINCE]}, {}}; // PAP
    graph[this->nod[SIETE]] = {{this->nod[4], this->nod[SEIS], this->nod[OCHO], this->nod[DIECISEIS]}, {}}; // PAM
    graph[this->nod[OCHO]] = {{this->nod[CINCO], this->nod[SIETE], this->nod[DIECISIETE]}, {}}; // PAA

    graph[this->nod[NUEVE]] = {{this->nod[0], this->nod[DIEZ], this->nod[DOCE], this->nod[DIECIOCHO]}, {}}; // MPP
    graph[this->nod[DIEZ]] = {{this->nod[1], this->nod[NUEVE], this->nod[ONCE], this->nod[TRECE], this->nod[DIECINUEVE]}, {}}; // MPM
    graph[this->nod[ONCE]] = {{this->nod[2], this->nod[DIEZ], this->nod[CATORCE], this->nod[VEINTE]}, {}}; // MPA
    graph[this->nod[DOCE]] = {{this->nod[3], this->nod[NUEVE], this->nod[TRECE], this->nod[QUINCE], this->nod[VEINTIUNO]}, {}}; // MMP
    graph[this->nod[TRECE]] = {{this->nod[4], this->nod[DIEZ], this->nod[DOCE], this->nod[CATORCE], this->nod[DIECISEIS], this->nod[VEINTIDOS]}, {}}; // MMM
    graph[this->nod[CATORCE]] = {{this->nod[CINCO], this->nod[ONCE], this->nod[TRECE], this->nod[DIECISIETE], this->nod[VEINTITRES]}, {}}; // MMA
    graph[this->nod[QUINCE]] = {{this->nod[SEIS], this->nod[DOCE], this->nod[DIECISEIS], this->nod[DIECIOCHO], this->nod[VEINTICUATRO]}, {}}; // MAP
    graph[this->nod[DIECISEIS]] = {{this->nod[SIETE], this->nod[TRECE], this->nod[QUINCE], this->nod[DIECISIETE], this->nod[VEINTICINCO]}, {}}; // MAM
    graph[this->nod[DIECISIETE]] = {{this->nod[OCHO], this->nod[CATORCE], this->nod[DIECISEIS], this->nod[VEINTISEIS]}, {}}; // MAA

    graph[this->nod[DIECIOCHO]] = {{this->nod[NUEVE], this->nod[QUINCE], this->nod[DIECINUEVE]}, {}}; // APP
    graph[this->nod[DIECINUEVE]] = {{this->nod[DIEZ], this->nod[DIECIOCHO], this->nod[VEINTE], this->nod[VEINTIDOS]}, {}}; // APM
    graph[this->nod[VEINTE]] = {{this->nod[ONCE], this->nod[DIECINUEVE], this->nod[VEINTITRES]}, {}}; // APA
    graph[this->nod[VEINTIUNO]] = {{this->nod[DOCE], this->nod[VEINTIDOS], this->nod[VEINTICUATRO]}, {}}; // AMP
    graph[this->nod[VEINTIDOS]] = {{this->nod[TRECE], this->nod[DIECINUEVE], this->nod[VEINTIUNO], this->nod[VEINTITRES], this->nod[VEINTICINCO]}, {}}; // AMM
    graph[this->nod[VEINTITRES]] = {{this->nod[CATORCE], this->nod[VEINTE], this->nod[VEINTIDOS], this->nod[VEINTISEIS]}, {}}; // AMA
    graph[this->nod[VEINTICUATRO]] = {{this->nod[QUINCE], this->nod[VEINTIUNO], this->nod[VEINTICINCO]}, {}}; // AAP
    graph[this->nod[VEINTICINCO]] = {{this->nod[DIECISEIS], this->nod[VEINTIDOS], this->nod[VEINTICUATRO], this->nod[VEINTISEIS]}, {}}; // AAM
    graph[this->nod[VEINTISEIS]] = {{this->nod[DIECISIETE], this->nod[VEINTITRES], this->nod[VEINTICINCO]}, {}}; // AAA
  return graph;
}
//NOLINTBEGIN(misc-no-recursion)
__uint32_t ImageSOA::cf_find_closest_in_neighbors(
    __uint32_t color_to_delete,const unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> &graph,
    const vector<__uint32_t> &neighbors,
    double &min_distance, unordered_map<__uint32_t, __uint8_t> &visited_node) {
  __uint32_t closest_color = 0;
  bool found_closest = false;
  for (__uint32_t const neighbor : neighbors) {
    auto iter = graph.find(neighbor);
    if(visited_node.contains(neighbor)){continue;}
    for (__uint32_t const candidate : iter->second.second) {
      double const distance = get_distance(color_to_delete, candidate);
      if (distance <= min_distance) {
        min_distance = distance;
        closest_color = candidate;
        found_closest = true;}}
    visited_node[neighbor] = 0;}
  if (found_closest) {return closest_color;}
  if (min_distance ==MAX_DIST) {
    for (__uint32_t const neighbor : neighbors) {
      auto iter = graph.find(neighbor);
      closest_color = cf_find_closest_in_neighbors(color_to_delete, graph, iter->second.first, min_distance,visited_node);
      if (closest_color != 0) { return closest_color;}}}
  return 0;
}
//NOLINTEND(misc-no-recursion)

deque<pair<__uint32_t, __uint16_t>>
    ImageSOA::cf_check_first_part_small(unordered_map<__uint32_t, __uint16_t> myMap,
                                        unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                                        int & num_left) const {
  vector<pair<__uint32_t, __uint16_t>> myVector(myMap.begin(), myMap.end());
  ranges::sort(myVector, [](auto const & op1, auto const & op2) {
    return op1.second < op2.second;
  });

  unordered_map<__uint32_t, size_t> color_to_index;
  auto sorted_colors = sort_and_map_keys(myMap, color_to_index);
  // Me paso a size_t el numero de elementos a eliminar y me creo un vector delete
  vector<pair<__uint32_t, __uint16_t>> VectorDelete;
  size_t const elems_to_delete = static_cast<size_t>(this->get_args()[0]);

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

void ImageSOA::cf_finish_graph(
    unordered_map<__uint32_t, __uint16_t> myMap,
    unordered_map<__uint32_t, __uint32_t> & Deleteitems,
    unordered_map<__uint32_t, __uint32_t> & toSave,
    unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> & graph) {
  for (auto const & key : myMap | views::keys) {
    // me recorro las keys de graph
    // me recorro graph
    double distance = MAX_DIST;
    for (auto const & key1 : graph | views::keys) {
      double const new_distance = get_distance(key, key1);
      if (new_distance <= distance) {
        distance = new_distance;
        if (!Deleteitems.contains(key)) {
          toSave[key] = key1;
        } else {
          Deleteitems[key] = key1;
        }
      }
    }
    if (!Deleteitems.contains(key)) { graph[toSave[key]].second.push_back(key); }
  }
}

void ImageSOA::write_in_exit(unordered_map<__uint32_t, __uint32_t> Deleteitems) {
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

  output_file.close();}void ImageSOA::cutfreq_min(const unordered_map<__uint32_t, __uint16_t>& myMap) {
  // Convierto myMap a vector de pares y ordeno
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  int num_left = 0;
  auto left_elems= cf_check_first_part_small(myMap, Deleteitems, num_left);
  auto bluevalues = same_bgr_vector(left_elems, 1, left_elems.size());

  // Para saber que elemento de bluevalues utilizar
  Deleteitems = check_colors_to_delete(Deleteitems, num_left, bluevalues);
  unordered_map<__uint32_t, __uint32_t> toSave;
  // Me recorro las keys de myMap
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph = cf_generate_graph();
  cf_finish_graph(myMap, Deleteitems,toSave,graph);

  for (auto &entry : Deleteitems) {
    __uint32_t const color_to_delete = entry.first;
    double min_distance = MAX_DIST;
    unordered_map<__uint32_t, __uint8_t> visited = {};
    // Obtener el nodo correspondiente al color a eliminar
    auto node_it = graph.find(entry.second);
    if (node_it == graph.end()) { continue; // Si no se encuentra el nodo, omitir
}
    visited[entry.second] = 0;

    // Primero, verificar la distancia en el nodo principal
    //bool found_in_main_node = false;
    for (__uint32_t const candidate : node_it->second.second) {
      double const distance = get_distance(color_to_delete, candidate);
      if (distance < min_distance) {
        min_distance = distance;
        entry.second = candidate;  // Guardar en Deleteitems el candidato encontrado
        //found_in_main_node = true;
      }
    }
    __uint32_t const replacement_color = cf_find_closest_in_neighbors(color_to_delete, graph, node_it->second.first, min_distance, visited);
    // Si encontramos un reemplazo adecuado, guardarlo en el grafo y en Deleteitems
    if (replacement_color != 0) {
      entry.second = replacement_color;  // Guardar el color reemplazo en Deleteitems
    }}
  write_in_exit(Deleteitems);
  }



void ImageSOA::cutfreq_max(unordered_map<__uint64_t, __uint16_t> myMapBIG) {
  constexpr __uint32_t TUSMUERTOS = 33;
  myMapBIG[{TUSMUERTOS}]          = TUSMUERTOS;
  cout << "Pinga" << myMapBIG[0] << "\n";
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
  unordered_map<__uint32_t, __uint16_t> myMap;
  unordered_map<__uint64_t, __uint16_t> myMapBIG;
  if (maxval == MIN_LEVEL) {
    myMap                        = cf_load_and_map_8(width, move(input_file), height);
    size_t const elems_to_delete = static_cast<size_t>(this->get_args()[0]);
    if (elems_to_delete >= myMap.size()) {
      cerr << "El numero de pixeles menos frecuentes a eliminar es mayor que el numero de "
              "pixeles unicos"
           << "\n";
      return -1;
    }
    cutfreq_min(myMap);
    cout << "Pinga";
  } else {
    myMapBIG = load_and_map_8BIG(width, move(input_file), height);
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

int ImageSOA::compress_min() {
  ifstream input_file = this->get_if_input_file();
  ofstream output_file(this->get_output_file(), ios::binary);
  auto width  = static_cast<unsigned int>(this->get_width());
  auto height = static_cast<unsigned int>(this->get_height());
  unordered_map<unsigned int, unsigned int> color_map;
  list<unsigned int> indexes;
  soa_rgb_small unique_colors;
  for (unsigned int i = 0; i < width * height; i++) {
    unsigned char const red         = read_binary_8(input_file);
    unsigned char const grn         = read_binary_8(input_file);
    unsigned char const blu         = read_binary_8(input_file);
    unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
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
  unordered_map<unsigned int, unsigned int> color_map;
  list<unsigned int> indexes;
  soa_rgb_big unique_colors;
  for (unsigned int i = 0; i < width * height; i++) {
    unsigned short const red        = read_binary_16(input_file);
    unsigned short const grn        = read_binary_16(input_file);
    unsigned short const blu        = read_binary_16(input_file);
    unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
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
  cp_export(output_file, color_map, indexes);
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


