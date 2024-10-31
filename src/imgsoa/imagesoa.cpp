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
#include <map>
#include <string>
#include <sys/stat.h>
#include <utility>
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
array<rgb_small, 4> ImageSOA::obtain_square(soa_rgb_small const & image,
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
  string format;
  int width  = 0;
  int height = 0;
  int maxval = 0;
  input_file >> format >> width >> height >> maxval;
  input_file.ignore(1);
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

int ImageSOA::compress() {
  get_imgdata();
  ifstream input_file(this->get_input_file(), ios::binary);
  ofstream output_file(this->get_output_file(), ios::binary);

  if (!input_file || !output_file) {
    cerr << "Error al abrir los archivos de entrada/salida"
         << "\n";
    return -1;
  }

  string format;
  unsigned int width  = 0;
  unsigned int height = 0;
  unsigned int maxval = 0;
  input_file >> format >> width >> height >> maxval;
  input_file.ignore(1);

  if (maxval <= MIN_LEVEL) {
    /*
     * Usaremos un árbol AVL como si fuera un catálogo de colores para almacenar los colores
     * DISTINTOS de la imagen. Éstos también se almacenan un struct_soa, para recorrelo
     * posteriormente. Esta implementación hará que la complejidad de esta operación sea
     * O(n log(n)), donde n es el número de píxeles de la imagen.
     */
    AVLTree tree;
    soa_rgb_small image;
    for (unsigned int i = 0; i < width * height; i++) {
      unsigned char red = 0;
      unsigned char grn = 0;
      unsigned char blu = 0;
      red               = read_binary_8(input_file);
      grn               = read_binary_8(input_file);
      blu               = read_binary_8(input_file);

      if (i == 0) {  // Si es el primer elemento
        unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
        element const elem              = {.color = concatenated, .index = 0};
        tree.insert(elem);
        image.r.push_back(red);
        image.g.push_back(grn);
        image.b.push_back(blu);
      } else {  // Si no es el primer elemento
        // Comprobamos si el color ya está en el árbol
        unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
        element const elem              = {.color = concatenated, .index = i};
        if (tree.insert(elem) == 0) {  // Se ha podido insertar, por lo que no existía previamente
          image.r.push_back(red);
          image.g.push_back(grn);
          image.b.push_back(blu);
        }
      }
    }
    if (image.r.size() > static_cast<unsigned long int>(pow(2, 4 * BYTE))) {
      cerr << "Error: demasiados colores distintos."
           << "\n";
      return -1;
    }
    // Ahora ya sabemos cuántos colores distintos hay en la imagen. Los escribimos
    output_file << "C6"
                << " " << width << " " << height << " " << maxval << " " << image.r.size() << "\n";
    for (unsigned int i = 0; i < image.r.size(); i++) {
      write_binary_8(output_file, image.r[i]);
      write_binary_8(output_file, image.g[i]);
      write_binary_8(output_file, image.b[i]);
    }
    /*
     * Ahora ya podemos escribir los píxeles de la imagen pero antes de hacerlo, debemos determinar
     * cuántos bits necesitamos para representar los índices de los colores. Tenemos 3 casos:
     * 1. Si hay < 2^8 colores distintos, necesitamos 8 bits.
     * 2. Si hay < 2^16 colores distintos, necesitamos 16 bits.
     * 3. Si hay < 2^32 colores distintos, necesitamos 32 bits.
     * 4. Si hay más, no lo soportamos.
     */
    unsigned long int const num_colors = image.r.size();
    // Hay que volver a abrir el archivo para volver a leerlo
    input_file.close();
    ifstream input_file(this->get_input_file(), ios::binary);
    input_file >> format >> width >> height >> maxval;
    input_file.ignore(1);
    if (num_colors < static_cast<unsigned long int>(pow(2, BYTE))) {
      for (unsigned int i = 0; i < width * height; i++) {
        unsigned char red = 0;
        unsigned char grn = 0;
        unsigned char blu = 0;
        red               = read_binary_8(input_file);
        grn               = read_binary_8(input_file);
        blu               = read_binary_8(input_file);

        unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
        element const elem = tree.search(concatenated);
        write_binary_8(output_file, static_cast<unsigned char>(elem.index));
      }
    } else if (num_colors < static_cast<unsigned long int>(pow(2, 2 * BYTE))) {
      for (unsigned int i = 0; i < width * height; i++) {
        unsigned char red = 0;
        unsigned char grn = 0;
        unsigned char blu = 0;
        red               = read_binary_8(input_file);
        grn               = read_binary_8(input_file);
        blu               = read_binary_8(input_file);

        unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
        element const elem = tree.search(concatenated);
        write_binary_16(output_file, static_cast<uint16_t>(elem.index));
      }
    } else if (num_colors < static_cast<unsigned long int>(pow(2, 4 * BYTE))) {
      for (unsigned int i = 0; i < width * height; i++) {
        unsigned char red = 0;
        unsigned char grn = 0;
        unsigned char blu = 0;
        red               = read_binary_8(input_file);
        grn               = read_binary_8(input_file);
        blu               = read_binary_8(input_file);

        unsigned int const concatenated = red << 2 * BYTE | grn << BYTE | blu;
        element const elem = tree.search(concatenated);
        write_binary_32(output_file, static_cast<uint32_t>(elem.index));
      }
    }
  }

  else if (maxval <= MAX_LEVEL) {
    ;
  } else {
    cerr << "Error: maxval no soportado"
         << "\n";
    return -1;
  }

  input_file.close();
  output_file.close();
  return 0;
}


