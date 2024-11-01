//
// Created by beto on 4/10/24.
//

#include "imagesoa.hpp"

#include "common/binario.hpp"
#include "common/progargs.hpp"
#include "common/struct-rgb.hpp"
#include "common/AVLTree.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include<unordered_map>
#include <string>
#include <math.h>
#include <sys/stat.h>
#include <utility>
#include <limits>
#include <vector>

static constexpr int MAX_LEVEL = 65535;
static constexpr int MIN_LEVEL = 255;
static constexpr int BYTE      = 8;
static constexpr int FIVE      = 5;

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
    if (cutfreq() < 0) {return -1;}
  }

  else {
    cerr << "Operación no soportada de momento: " << this->get_optype() << "\n";
    return -1;
  }
  return 0;
}

/**
 * Función para hallar los píxeles más cercanos
 */
array<rgb_small, 4> ImageSOA::obtain_square(soa_rgb_small const & image,
                                            array<unsigned int, FIVE> args) {
  auto xlength = static_cast<unsigned long>(args[0]);
  auto xheight = static_cast<unsigned long>(args[1]);
  auto ylength = static_cast<unsigned long>(args[2]);
  auto yheigth = static_cast<unsigned long>(args[3]);

  auto width = static_cast<unsigned long>(args[4]);

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

rgb_small ImageSOA::interpolate(double u_param, array<rgb_small, 4> square, double t_param) {
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
    image.r.push_back(red);
    image.g.push_back(grn);
    image.b.push_back(blu);
  }
  return image;
}

int ImageSOA::resize_min(ofstream & output_file) {
  ifstream input_file = ifstream(this->get_input_file(), ios::binary);
  std::string dummy;
  std::getline(input_file, dummy);

  int const width      = this->get_width();
  int const height     = this->get_height();
  int const new_width  = this->get_args()[0];
  int const new_height = this->get_args()[1];

  soa_rgb_small const image = read_image_rgb_small(input_file);
  for (int y_prime = 0; y_prime < new_height; y_prime++) {
    for (int x_prime = 0; x_prime < new_width; x_prime++) {
      double const equis  = floor(x_prime * (static_cast<double>(width) / new_width));
      double const ygreek = floor(y_prime * (static_cast<double>(height) / new_height));

      auto xlength = static_cast<unsigned int>(floor(equis));
      auto xheight = static_cast<unsigned int>(ceil(equis));
      auto ylength = static_cast<unsigned int>(floor(ygreek));
      auto yheight = static_cast<unsigned int>(ceil(ygreek));

      xheight = min(xheight, static_cast<unsigned int>(width - 1));
      xlength = min(xlength, static_cast<unsigned int>(width - 1));
      yheight = min(yheight, static_cast<unsigned int>(height - 1));
      ylength = min(ylength, static_cast<unsigned int>(height - 1));

      array<unsigned int, FIVE> const args = {xlength, xheight, ylength, yheight,
                                              static_cast<unsigned int>(width)};
      array<rgb_small, 4> const square     = obtain_square(image, args);

      double const t_param    = equis - xlength;
      double const u_param    = ygreek - ylength;
      rgb_small const c_param = interpolate(u_param, square, t_param);
      write_binary_8(output_file, c_param.r);
      write_binary_8(output_file, c_param.g);
      write_binary_8(output_file, c_param.b);
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

  if (maxval <= MIN_LEVEL) { resize_min(output_file); }

  output_file.close();
  return 0;
}



unordered_map<__uint32_t, __uint16_t> ImageSOA::load_and_map_8(int width, ifstream input_file, int height) {
  unordered_map<__uint32_t, __uint16_t> myMap;
  unsigned char red = 0;
  unsigned char grn = 0;
  unsigned char blu = 0;
  for (int i = 0; i < width * height; i++) {
    red = read_binary_8(input_file);
    grn = read_binary_8(input_file);
    blu = read_binary_8(input_file);
    if (__uint32_t const rgb = packRGB(red, grn, blu);myMap.contains(rgb)  ) {
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

unordered_map<__uint64_t, __uint16_t> ImageSOA::load_and_map_8BIG(int width, ifstream input_file,
        int height){
    unsigned short red = 0;
    unsigned short grn = 0;
    unsigned short blu = 0;
    unordered_map<__uint64_t, __uint16_t> myMap;
    for(int i=0; i< width * height; i++) {
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

deque<pair<__uint32_t, __uint16_t>> ImageSOA::same_bgr_vector(deque<pair<__uint32_t, __uint16_t>>
                                          father_vector, const int value, const size_t counter) {
    //Value será 1 para blue, 2 para green y 3 para red
    deque<pair<__uint32_t, __uint16_t>> color_vector;
    __uint8_t color = 0;
    for (size_t i = 0; i <= counter; i++) {
        if(value ==1){color = extractblue(father_vector[i].first);}
        if(value ==2){color = extractgreen(father_vector[i].first);}
        if(value ==3){color = extractred(father_vector[i].first);}
      color_vector.emplace_back(father_vector[i].first, color);
    }
    ranges::sort(color_vector, [](auto const & op1, auto const & op2) {
      return op1.second > op2.second;
    });
  return color_vector;
}


int ImageSOA::check_and_delete(deque<pair<__uint32_t, __uint16_t>> & color_vector, int const color,
                               unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                               deque<pair<__uint32_t, __uint16_t>> &bluevalues) {
  size_t my_index = 0;
  //1 para azul, 0 para verde
  size_t meanwhile = 1;
  while (color_vector[meanwhile].second == color_vector[meanwhile + 1].second) { meanwhile++; }
  if (meanwhile == 1) {
    __uint8_t value0 = 0;
    __uint8_t value1 = 0;
    if(color ==1) {
      value0 = extractgreen(color_vector[0].first);
      value1 = extractgreen(color_vector[1].first);
    }
    else {
      value0 = extractred(color_vector[0].first);
      value1 = extractred(color_vector[1].first);
    }
    if (value0 - value1> 0) {
      Deleteitems[{color_vector[0].first}] = 0;
      //color_vector.pop_front();
      my_index = search_in_blue(bluevalues, color_vector[0].first);
      delete_from_deque(bluevalues, my_index);
    } else {
      Deleteitems[{color_vector[1].first}] = 0;
      my_index = search_in_blue(bluevalues, color_vector[1].first);
      delete_from_deque(bluevalues, my_index);
    }
    return 0;
  }
  return static_cast<int>(meanwhile);
}

void ImageSOA::delete_from_deque(deque<pair<__uint32_t, __uint16_t>> &deque_general, size_t index) {
  swap(deque_general[0], deque_general[index]);
  deque_general.pop_front();
}

size_t ImageSOA::search_in_blue(deque<pair<__uint32_t, unsigned short>> & pairs, __uint32_t & first) {
  for (size_t i = 0; i < pairs.size(); i++) {
    if (pairs[i].first == first) { return i; }
  }
  return 0;
}


unordered_map<__uint32_t, __uint32_t> ImageSOA::check_colors_to_delete(
                                    unordered_map<__uint32_t, __uint32_t> Deleteitems, int num_left,
                                     deque<pair<__uint32_t, __uint16_t>> bluevalues) {
  size_t my_index = 0;
  while (num_left > 0) {
    if (bluevalues[0].second == bluevalues[1].second) {
      if (int my_meanwhile = check_and_delete(bluevalues, 1, Deleteitems, bluevalues);
          my_meanwhile > 0) {
        if (my_meanwhile < num_left) {
          auto iterator = static_cast<size_t>(my_meanwhile);
          for(size_t iii = 0; iii< iterator;iii++) {
            Deleteitems[bluevalues[0].first] = 0;
            bluevalues.pop_front();
            num_left --;}}
        auto greenvalues = same_bgr_vector(bluevalues, 2, static_cast<size_t>(my_meanwhile));
        if (greenvalues[0].second == greenvalues[1].second) {
          my_meanwhile = check_and_delete(greenvalues, 0, Deleteitems, bluevalues);
          if (my_meanwhile > 0) {
            auto redvalues = same_bgr_vector(greenvalues, 3, static_cast<size_t>(my_meanwhile));
            Deleteitems[{redvalues[0].first}] = 0;
            my_index = search_in_blue(bluevalues, redvalues[0].first);
            delete_from_deque(bluevalues, my_index);
            num_left--;
          } else {
            num_left--;
          }
        } else {
          Deleteitems[{greenvalues[0].first}] = 0;
          my_index = search_in_blue(bluevalues, greenvalues[0].first);
          delete_from_deque(bluevalues, my_index);
          num_left--;
        }
      } else {
        num_left--;
      }
    } else {
      Deleteitems[{bluevalues[0].first}] = 0;
      bluevalues.pop_front();
      num_left--;
    }
  }

  return Deleteitems;
}



void ImageSOA::cutfreq_min(unordered_map<__uint32_t, __uint16_t> myMap) {
  // Convierto myMap a vector de pares y ordeno

  vector<pair< __uint32_t, __uint16_t>> myVector(myMap.begin(), myMap.end());
  ranges::sort(myVector, [](auto const & op1, auto const & op2) {
    return op1.second < op2.second;
  });

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
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  for (auto & [fst, snd] : VectorDelete) {
    if (snd < pivot) {
      Deleteitems[{fst}] = 0;
      elem_deleted++;
    }
  }

  int const new_n    = static_cast<int>(elems_to_delete);
  int const num_left = new_n - elem_deleted;
  auto const new_e_d = static_cast<long int>(elem_deleted);  // elem_deleted

  deque const left_elems(VectorDelete.begin() + new_e_d, VectorDelete.end());

  auto bluevalues = same_bgr_vector(left_elems, 1, left_elems.size());
  // Para saber que elemento de bluevalues utilizar
  Deleteitems = check_colors_to_delete(Deleteitems, num_left, bluevalues);

  for(auto& Delitem : Deleteitems) {
    double distance = sqrt(3*pow(MIN_LEVEL,2));
    double new_distance = 0;
    for(const auto& storage : myMap) {
      __uint8_t const check_red = extractred(storage.first);
      __uint8_t const check_grn = extractgreen(storage.first);
      __uint8_t const check_blu = extractblue(storage.first);
      if (__uint32_t const rgb =
                      packRGB(check_red, check_grn, check_blu);not Deleteitems.contains(rgb)) {
        __uint8_t const actual_red = extractred(Delitem.first);
        __uint8_t const actual_grn = extractgreen(Delitem.first);
        __uint8_t const actual_blu = extractblue(Delitem.first);

        new_distance = sqrt(pow(actual_red -check_red, 2) +
                                pow(actual_grn -check_grn, 2) +pow(actual_blu - check_blu,2) );
        if (new_distance < distance) {
          distance = new_distance;
          Delitem.second = rgb;
        }
      }
    }
  }
  int const width  = this->get_width();
  int const height = this->get_height();
  ofstream output_file(this->get_output_file(), ios::binary);
  write_out(this->get_maxval());
  auto const iter = static_cast<size_t>(width * height);

  for(size_t pene = 0; pene < iter; pene ++){
    __uint8_t red = this->soa_small.r[pene];
    __uint8_t grn = this->soa_small.g[pene];
    __uint8_t blu = this->soa_small.b[pene];
    if (const __uint32_t rgb = packRGB(red, grn, blu); Deleteitems.contains(rgb)) {
      red = extractred(Deleteitems[rgb]);
      grn = extractgreen(Deleteitems[rgb]);
      blu = extractblue(Deleteitems[rgb]);
    }
    write_binary_8(output_file, red);
    write_binary_8(output_file, grn);
    write_binary_8(output_file, blu);
  }
  output_file.close();

    /*
     * Si tenemos los colores c1=(r1,g1,b1) y c2=(r2,g2,b2), la distancia euclídea entre ambos colores
     * no depende de su posición en la imagen sino de sus valores RGB.
     * d(c1,c2) = sqrt((r1-r2)² + (g1-g2)² + (b1-b2)²)
     */
}

void ImageSOA::cutfreq_max(unordered_map<__uint64_t, __uint16_t>  myMapBIG) {
  constexpr __uint32_t TUSMUERTOS = 33;
  myMapBIG[{TUSMUERTOS}] = TUSMUERTOS;
  cout << "Pinga" << myMapBIG[0] << "\n";
}

int ImageSOA::cutfreq()  {

  get_imgdata();
  ifstream input_file(this->get_input_file(), ios::binary);

  if (!input_file) {
    cerr << "Error al abrir los archivos de entrada/salida"
         << "\n";
    return -1;
  }

  write_out(get_maxval());


  int const width  = this->get_width();
  int const height = this->get_height();
  int const maxval = this->get_maxval();
  //ofstream output_file(this->get_output_file(), ios::binary);
  unordered_map<__uint32_t, __uint16_t> myMap;
  unordered_map<__uint64_t, __uint16_t> myMapBIG;
  if (maxval == MIN_LEVEL) {
    myMap = load_and_map_8(width, move(input_file), height);
    cutfreq_min(myMap);
    cout << "Pinga";
  }
  else {
    myMapBIG = load_and_map_8BIG(width, move(input_file), height);
    cutfreq_max(myMapBIG);
  }
  input_file.close();

  cutfreq_min(myMap);

    return 0;
  }






int ImageSOA::compress() {
  return 0;
}


