//
// Created by beto on 4/10/24.
//

#include "imagesoa.hpp"

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

static int const MAX_LEVEL = 65535;
static int const MIN_LEVEL = 255;
static int const BYTE      = 8;

using namespace std;

ImageSOA::ImageSOA(int argc, vector<string> const & argv) : Image(argc, argv) { }

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
    soa_rgb_small colors;
    vector<char *> pixels;
    for (unsigned int i = 0; i < width * height; i++) {
      char r = 0, g = 0, b = 0;
      input_file.read(&r, sizeof(r));
      input_file.read(&g, sizeof(g));
      input_file.read(&b, sizeof(b));

      // Usaremos un puntero únicamente al canal rojo ya que comparte índice con los canales verde y
      // azul
      char * ptr_r = nullptr;

      // Si es el primer elemento, lo guardamos directamente
      if (i == 0) {
        colors.r.push_back(r);
        colors.g.push_back(g);
        colors.b.push_back(b);

        ptr_r = &colors.r.back();
      }

      // Si NO es el primer elemento de la lista, verificamos si
      // está haciendo una busqueda binaria (ya que los elementos se introducirán
      // en la lista de manera ordenada)
      else {
        /*
         * Esta función realiza una búsqueda binaria para encontrar el primer elemento en el rango
         * que no sea menor que el valor buscado (r, g, b). El resultado es un iterador que apunta a
         * la posición donde el valor debería ser insertado si no se encuentra en el vector.
         */
        auto it_r = ranges::lower_bound(colors.r, r);
        auto it_g = ranges::lower_bound(colors.g, g);
        auto it_b = ranges::lower_bound(colors.b, b);

        // Si el valor ya existe, almacenamos un puntero a la posición
        if ((it_r != colors.r.end() && *it_r == r) && (it_g != colors.g.end() && *it_g == g) &&
            (it_b != colors.b.end() && *it_b == b)) {
          ptr_r = &(*it_r);
        } else {
          // Si el elemento no se encuentra en la lista, lo insertamos
          colors.r.insert(it_r, r);
          colors.g.insert(it_g, g);
          colors.b.insert(it_b, b);
          ptr_r = &colors.r.back();
        }
      }
      // Ahora, almacenamos el puntero a los elementos en el vector de píxeles
      pixels.push_back(ptr_r);
    }
    /*
     * Una vez almacenada la tabla de colores y los punteros (correspondientes a los píxeles que
     * apuntan a un color de la tabla de colores) escribimos en la imagen de salida los datos con el
     * formato correspondiente.
     */
    output_file << "C6"
                << " " << width << " " << height << " " << maxval << " " << colors.r.size() << "\n";
    for (unsigned long int i = 0; i < colors.r.size(); i++) {
      output_file.write(&colors.r[i], sizeof(char));
      output_file.write(&colors.g[i], sizeof(char));
      output_file.write(&colors.b[i], sizeof(char));
    }
    /*
     * Una vez escrita la tabla de colores, falta escribir los píxeles de la imagen de salida. Para
     * ello, sabiendo que tenemos un puntero del pixel al ELEMENTO del vector de colores, debemos
     * conseguir el ÍNDICE del elemento en dicho vector, para así poder escribirlo en binario en el
     * archivo de salida. Hay que recordar, además, que dependiendo de la cantidad de colores, la
     * cantidad de bits necesarios para representar un índice será distinta:
     * - Si la cantidad de colores es <=2^8, necesitamos 8 bit
     * - Si la cantidad de colores es <=2^16, necesitamos 16 bit
     * - Si la cantidad de colores es <= 2^32, necesitamos 32 bit
     * - Si la cantidad de colores es > 2^32, no se soporta
     */

    for (auto & pixel : pixels) {
      long const index_r = pixel - &colors.r[0];  // Calcula el índice del elemento
      cout << "El índice de r es: " << index_r << "\n";
    }
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

