//
// Created by beto on 4/10/24.
//

#include "imagesoa.hpp"


#include "common/mtdata.hpp"
#include "common/progargs.hpp"
#include "common/struct-rgb.hpp"
#include "common/binario.hpp"

#include <map>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>

static int const MAX_LEVEL = 65535;
static int const MIN_LEVEL = 255;
static int const BYTE      = 8;

using namespace std;

ImageSOA::ImageSOA(int argc, vector<string> const & argv) : Image(argc, argv) { }

int ImageSOA::process_operation() {
  // Primera operación: leer los metadatos de la imagen de entrada. Como
  // esta función es común a AOS y SOA, será implementada en la biblioteque "common"
  if (this->get_optype() == "info") {
    if (get_metadata(this->get_input_file()) < 0) { return -1; }
  } else if (this->get_optype() == "maxlevel") {
    // Implementación de la operación de nivel máximo usando AOS (Array of Structures)
    if (maxlevel() < 0) { return -1; }
  } else if (this->get_optype() == "cutfreq") {
    if (cutfreq() < 0) { return -1; }
  } else if (this->get_optype() == "resize") {
    // Implementación de la operación de redimensionamiento usando AOS (Array of Structures)
    if (resize() < 0) { return -1; }
  } else {
    cerr << "Operación no soportada de momento: " << this->get_optype() << "\n";
    return -1;
  }
  return 0;
}

int ImageSOA::cutfreq() {
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

  soa_rgb_small mysoa;

  char r = 0, g = 0, b = 0;
  map<string, int> myMap;

  for (unsigned int i = 0; i < width * height; i++) {
    input_file.read(&r, sizeof(r));
    input_file.read(&g, sizeof(g));
    input_file.read(&b, sizeof(b));

    /* Se actualizan las repeticines del misimo codigo rgb en el diccionario
     * y si no está se añade
     * */
    if (string const rgb = mix3char(r, g, b); myMap.contains(rgb)) {
      myMap[{rgb}]++;
    } else {
      myMap[{rgb}] = 1;
    }
    mysoa.r.push_back(r);
    mysoa.g.push_back(g);
    mysoa.b.push_back(b);
  }
  vector<pair<string, int>> myVector(myMap.begin(), myMap.end());
  // const int size = static_cast<int>(myVector.size());
  /*size_t const size = myVector.size();
  quick::quicksort(myVector, 0, size - 1);
  */
  ranges::sort(myVector, [](auto const & a, auto const & b) {
    return a.second < b.second;
  });

  // imprimo el vector por pantalla, independientemente de nada

  vector<pair<string, int>> VectorDelete;
  auto n = static_cast<size_t>(this->get_args()[0]);

  for (size_t i = 0; i < n; i++) { VectorDelete.push_back(myVector[i]); }
  size_t tamDelete = n;

  while (myVector[tamDelete].second == VectorDelete[n - 1].second) {
    VectorDelete.push_back(myVector[tamDelete]);
    tamDelete++;
  }
  // imprimo VectorDelete
  for (auto & i : VectorDelete) { cout << i.first << " " << i.second << "\n"; }
  int const pivot  = VectorDelete[n - 1].second;
  int elem_deleted = 0;
  vector<pair<string, string>> Deleteitems;
  for (size_t i = 0; i < VectorDelete.size(); i++) {
    if (VectorDelete[i].second < pivot) {
      cout << "Te elimino jueputa " << VectorDelete[i].second << "\n";
      Deleteitems.emplace_back(VectorDelete[i].first, "");
      elem_deleted++;
    }
  }
  int const new_n = static_cast<int>(n);
  int num_left    = new_n - elem_deleted;
  auto new_e_d    = static_cast<long int>(elem_deleted);

  vector left_elems(VectorDelete.begin() + new_e_d, VectorDelete.end());

  // imprime left_elems
  for (auto & i : left_elems) {
    cout << "tengo que ver si te elimino jueputa "
         << "\n";
    cout << i.first << " " << i.second << "\n";
  }

  vector<pair<string, __uint8_t>> bluevalues;
  __uint8_t blue = 0;
  for (auto & iterator : left_elems) {
    blue = extractblue(iterator.first);
    bluevalues.emplace_back(iterator.first, blue);
  }
  ranges::sort(bluevalues, [](auto const & a, auto const & b) {
    return get<1>(a) > get<1>(b);
  });

  while (num_left > 0) {
    if (bluevalues[0].second == bluevalues[1].second) {
      size_t meanwhile = 1;
      while (bluevalues[meanwhile].second == bluevalues[meanwhile + 1].second) { meanwhile++; }
      if (meanwhile == 1) {
        // Hay que comparar el gree n del elemento 0 y del 1
        __uint8_t const green0 = extractgreen(left_elems[0].first);
        __uint8_t const green1 = extractgreen(left_elems[1].first);
        if (green0 - green1 > 0) {
          Deleteitems.emplace_back(bluevalues[0].first, "");
          bluevalues.erase(bluevalues.begin());
          num_left--;
        } else {
          Deleteitems.emplace_back(bluevalues[1].first, "");
          bluevalues.erase(bluevalues.begin() + 1);
          num_left--;
        }
      } else {
        __uint8_t green = 0;
        vector<tuple<string, __uint8_t, __uint8_t>> greenvalues;
        for (size_t i = 0; i <= meanwhile; i++) {
          green = extractgreen(bluevalues[i].first);
          greenvalues.emplace_back(bluevalues[i].first, bluevalues[i].second, green);
        }

        ranges::sort(greenvalues, [](auto const & a, auto const & b) {
          return get<2>(a) > get<2>(b);
        });
        if (get<2>(greenvalues[0]) == get<2>(greenvalues[1])) {
          size_t meanwhile = 1;
          while (get<2>(greenvalues[meanwhile]) == get<2>(greenvalues[meanwhile + 1])) {
            meanwhile++;
          }
          if (meanwhile == 1) {
            __uint8_t const red1 = extractred(get<0>(greenvalues[1]));
            __uint8_t const red0 = extractred(get<0>(greenvalues[0]));
            if(red1 - red0 >0) {
              Deleteitems.emplace_back(get<0>(greenvalues[1]), "");
              greenvalues.erase(greenvalues.begin() +1);

            }
            else {
              Deleteitems.emplace_back(get<0>(greenvalues[0]), "");
              greenvalues.erase(greenvalues.begin());
            }
            num_left--;
          } else {
            cout << "Tengo que ver a cual mato rojo cabron"
                 << "\n";
            num_left--;
          }

        } else {
          Deleteitems.emplace_back(get<0>(greenvalues[0]), "");
          greenvalues.erase(greenvalues.begin());
          num_left--;
        }
      }
    } else {
      Deleteitems.emplace_back(bluevalues[0].first, "");
      bluevalues.erase(bluevalues.begin());
      num_left--;
    }
  }

  /*
   * Si tenemos los colores c1=(r1,g1,b1) y c2=(r2,g2,b2), la distancia euclídea entre ambos colores
   * no depende de su posición en la imagen sino de sus valores RGB.
   * d(c1,c2) = sqrt((r1-r2)² + (g1-g2)² + (b1-b2)²)
   */

  return 0;
}

int ImageSOA::resize() {
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
        double const t     = x - xl;
        rgb_big const c1 = {.r = static_cast<uint16_t>((1 - t) * p1.r + t * p2.r),
                            .g = static_cast<uint16_t>((1 - t) * p1.g + t * p2.g),
                            .b = static_cast<uint16_t>((1 - t) * p1.b + t * p2.b)};

        rgb_big const c2 = {.r = static_cast<uint16_t>((1 - t) * p3.r + t * p4.r),
                            .g = static_cast<uint16_t>((1 - t) * p3.g + t * p4.g),
                            .b = static_cast<uint16_t>((1 - t) * p3.b + t * p4.b)};

        double const u = y - yl;
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




