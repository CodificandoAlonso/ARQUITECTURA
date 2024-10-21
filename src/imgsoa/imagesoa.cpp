//
// Created by beto on 4/10/24.
//

#include "imagesoa.hpp"

#include "common/AVLTree.hpp"
#include "common/binario.hpp"
#include "common/progargs.hpp"
#include "common/struct-rgb.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static constexpr int MAX_LEVEL = 65535;
static constexpr int MIN_LEVEL = 255;
static constexpr int BYTE      = 8;

using namespace std;

ImageSOA::ImageSOA(int const argc, vector<string> const & argv) : Image(argc, argv) { }

int ImageSOA::process_operation() {
  // Primera operación: leer los metadatos de la imagen de entrada. Como
  // esta función es común a AOS y SOA, será implementada en la biblioteque "common"
  if (this->get_optype() == "info") {
    if (Image::info() < 0) { return -1; }
  } else if (this->get_optype() == "maxlevel") {
    // Implementación de la operación de nivel máximo usando AOS (Array of Structures)
    if (Image::maxlevel() < 0) { return -1; }
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

int ImageSOA::resize() const {
  ifstream input_file(this->get_input_file(), ios::binary);
  ofstream output_file(this->get_output_file(), ios::binary);

  if (!input_file || !output_file) {
    cerr << "Error al abrir los archivos de entrada/salida"
         << "\n";
    return -1;
  }

  string format;
  unsigned int width = 0, height = 0, maxval = 0;
  input_file >> format >> width >> height >> maxval;
  input_file.ignore(1);

  int const new_width  = this->get_args()[0];
  int const new_height = this->get_args()[1];

  output_file << format << " " << new_width << " " << new_height << " " << maxval << "\n";

  if (maxval <= MIN_LEVEL) {
    // leemos la imagen y la almacenamos en memoria
    soa_rgb_small image;
    for (unsigned int i = 0; i < width * height; i++) {
      char r = 0, g = 0, b = 0;
      input_file.read(&r, sizeof(r));
      image.r.push_back(r);
      input_file.read(&g, sizeof(g));
      image.g.push_back(g);
      input_file.read(&b, sizeof(b));
      image.b.push_back(b);
    }

    for (int y_prime = 0; y_prime < new_height; y_prime++) {
      for (int x_prime = 0; x_prime < new_width; x_prime++) {
        // Por como funcionan las operaciones de coma flotante, hay que redondear el valor
        // según una cierta precisión. Si no, la imagen final generará píxeles corruptos.
        double const x = round(x_prime * (static_cast<double>(width) / new_width));
        double const y = round(y_prime * (static_cast<double>(height) / new_height));

        auto xl = static_cast<unsigned int>(floor(x)), xh = static_cast<unsigned int>(ceil(x));
        auto yl = static_cast<unsigned int>(floor(y)), yh = static_cast<unsigned int>(ceil(y));

        xh = min(xh, width - 1);
        xl = min(xl, width - 1);
        yh = min(yh, height - 1);
        yl = min(yl, height - 1);

        // Obtenemos los 4 pixeles más cercanos
        rgb_small const p1 = {.r = image.r[static_cast<unsigned long>(yl) * width + xl],
                              .g = image.g[static_cast<unsigned long>(yl) * width + xl],
                              .b = image.b[static_cast<unsigned long>(yl) * width + xl]};

        rgb_small const p2 = {.r = image.r[static_cast<unsigned long>(yl) * width + xh],
                              .g = image.g[static_cast<unsigned long>(yl) * width + xh],
                              .b = image.b[static_cast<unsigned long>(yl) * width + xh]};

        rgb_small const p3 = {.r = image.r[static_cast<unsigned long>(yh) * width + xl],
                              .g = image.g[static_cast<unsigned long>(yh) * width + xl],
                              .b = image.b[static_cast<unsigned long>(yh) * width + xl]};

        rgb_small const p4 = {.r = image.r[static_cast<unsigned long>(yh) * width + xh],
                              .g = image.g[static_cast<unsigned long>(yh) * width + xh],
                              .b = image.b[static_cast<unsigned long>(yh) * width + xh]};

        // Interpolación en el eje x
        double const t     = x - xl;
        rgb_small const c1 = {.r = static_cast<char>((1 - t) * p1.r + t * p2.r),
                              .g = static_cast<char>((1 - t) * p1.g + t * p2.g),
                              .b = static_cast<char>((1 - t) * p1.b + t * p2.b)};

        rgb_small const c2 = {.r = static_cast<char>((1 - t) * p3.r + t * p4.r),
                              .g = static_cast<char>((1 - t) * p3.g + t * p4.g),
                              .b = static_cast<char>((1 - t) * p3.b + t * p4.b)};

        // Interpolación en el eje y
        double const u    = y - yl;
        rgb_small const c = {.r = static_cast<char>((1 - u) * c1.r + u * c2.r),
                             .g = static_cast<char>((1 - u) * c1.g + u * c2.g),
                             .b = static_cast<char>((1 - u) * c1.b + u * c2.b)};

        // Escribir el pixel interpolado
        output_file.write(&c.r, sizeof(c.r));
        output_file.write(&c.g, sizeof(c.g));
        output_file.write(&c.b, sizeof(c.b));
      }
    }
  } else if (maxval <= MAX_LEVEL) {
    soa_rgb_big image;
    for (unsigned int i = 0; i < width * height; i++) {
      uint16_t r = 0, g = 0, b = 0;
      r = read_binary_16(input_file);
      g = read_binary_16(input_file);
      b = read_binary_16(input_file);

      image.r.push_back(r);
      image.g.push_back(g);
      image.b.push_back(b);
    }

    for (int y_prime = 0; y_prime < new_height; y_prime++) {
      for (int x_prime = 0; x_prime < new_width; x_prime++) {
        double const x = round(x_prime * (static_cast<double>(width) / new_width));
        double const y = round(y_prime * (static_cast<double>(height) / new_height));

        auto xl = static_cast<unsigned int>(floor(x)), xh = static_cast<unsigned int>(ceil(x));
        auto yl = static_cast<unsigned int>(floor(y)), yh = static_cast<unsigned int>(ceil(y));

        xh = min(xh, width - 1);
        xl = min(xl, width - 1);
        yh = min(yh, height - 1);
        yl = min(yl, height - 1);

        // Obtenemos los 4 pixeles más cercanos
        rgb_big const p1 = {.r = image.r[static_cast<unsigned long>(yl) * width + xl],
                            .g = image.g[static_cast<unsigned long>(yl) * width + xl],
                            .b = image.b[static_cast<unsigned long>(yl) * width + xl]};

        rgb_big const p2 = {.r = image.r[static_cast<unsigned long>(yl) * width + xh],
                            .g = image.g[static_cast<unsigned long>(yl) * width + xh],
                            .b = image.b[static_cast<unsigned long>(yl) * width + xh]};

        rgb_big const p3 = {.r = image.r[static_cast<unsigned long>(yh) * width + xl],
                            .g = image.g[static_cast<unsigned long>(yh) * width + xl],
                            .b = image.b[static_cast<unsigned long>(yh) * width + xl]};

        rgb_big const p4 = {.r = image.r[static_cast<unsigned long>(yh) * width + xh],
                            .g = image.g[static_cast<unsigned long>(yh) * width + xh],
                            .b = image.b[static_cast<unsigned long>(yh) * width + xh]};

        // Interpolación en el eje x
        double const t   = x - xl;
        rgb_big const c1 = {.r = static_cast<uint16_t>((1 - t) * p1.r + t * p2.r),
                            .g = static_cast<uint16_t>((1 - t) * p1.g + t * p2.g),
                            .b = static_cast<uint16_t>((1 - t) * p1.b + t * p2.b)};

        rgb_big const c2 = {.r = static_cast<uint16_t>((1 - t) * p3.r + t * p4.r),
                            .g = static_cast<uint16_t>((1 - t) * p3.g + t * p4.g),
                            .b = static_cast<uint16_t>((1 - t) * p3.b + t * p4.b)};

        double const u  = y - yl;
        rgb_big const c = {.r = static_cast<uint16_t>((1 - u) * c1.r + u * c2.r),
                           .g = static_cast<uint16_t>((1 - u) * c1.g + u * c2.g),
                           .b = static_cast<uint16_t>((1 - u) * c1.b + u * c2.b)};

        write_binary_16(output_file, c.r);
        write_binary_16(output_file, c.g);
        write_binary_16(output_file, c.b);
      }
    }
  } else {
    cerr << "Error: maxval no soportado"
         << "\n";
    return -1;
  }

  input_file.close();
  output_file.close();
  return 0;
}

int ImageSOA::compress() const {
  ifstream input_file(this->get_input_file(), ios::binary);
  ofstream output_file(this->get_output_file(), ios::binary);

  if (!input_file || !output_file) {
    cerr << "Error al abrir los archivos de entrada/salida"
         << "\n";
    return -1;
  }

  string format;
  unsigned int width = 0, height = 0, maxval = 0;
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
      char r = 0, g = 0, b = 0;
      input_file.read(&r, sizeof(r));
      input_file.read(&g, sizeof(g));
      input_file.read(&b, sizeof(b));

      if (i == 0) {  // Si es el primer elemento
        element const elem = {.color = static_cast<unsigned long>(r) << 2 * BYTE |
                                       static_cast<unsigned long>(g) << BYTE |
                                       static_cast<unsigned long>(b),
                              .index = 0};
        tree.insert(elem);
        image.r.push_back(r);
        image.g.push_back(g);
        image.b.push_back(b);
        image.print(0);
      } else {  // Si no es el primer elemento
        // Comprobamos si el color ya está en el árbol
        element const elem = {.color = static_cast<unsigned long>(r) << 2 * BYTE |
                                       static_cast<unsigned long>(g) << BYTE |
                                       static_cast<unsigned long>(b),
                              .index = i};
        if (tree.insert(elem) == 0) {  // Se ha podido insertar, por lo que no existía previamente
          image.r.push_back(r);
          image.g.push_back(g);
          image.b.push_back(b);
          image.print(i);
        }
      }
    }
    // DEGUG imprimiremos
    cout << image.r.size();
  } else if (maxval <= MAX_LEVEL) {
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

