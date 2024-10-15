//
// Created by beto on 4/10/24.
//

#include "imagesoa.hpp"

#include "common/binario.hpp"
#include "common/mtdata.hpp"
#include "common/progargs.hpp"
#include "common/struct-rgb.hpp"

#include <cmath>
#include <fstream>
#include <iostream>

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
  } else if (this->get_optype() == "resize") {
    // Implementación de la operación de redimensionamiento usando AOS (Array of Structures)
    if (resize() < 0) { return -1; }
  } else {
    cerr << "Operación no soportada de momento: " << this->get_optype() << "\n";
    return -1;
  }
  return 0;
}

int ImageSOA::maxlevel() {
  ifstream input_file(this->get_input_file(), ios::binary);
  ofstream output_file(this->get_output_file(), ios::binary);

  if (!input_file || !output_file) {
    cerr << "Error al abrir los archivos de entrada/salida"
         << "\n";
    return -1;
  }

  string format;
  int width = 0, height = 0, maxval = 0;
  input_file >> format >> width >> height >> maxval;
  input_file.ignore(1);

  if (this->get_args()[0] < MIN_LEVEL + 1) {
    output_file << format << " " << width << " " << height << " " << MIN_LEVEL << "\n";
    if (maxval <= MIN_LEVEL) {
      // Este caso es origen maxvalue <256 y destino <256
      char r = 0, g = 0, b = 0;
      soa_rgb_small mysoa;

      // Leer y escribir en memoria
      for (int i = 0; i < width * height; i++) {
        input_file.read(&r, sizeof(r));
        input_file.read(&g, sizeof(g));
        input_file.read(&b, sizeof(b));

        int new_r = static_cast<unsigned char>(r);
        int new_g = static_cast<unsigned char>(g);
        int new_b = static_cast<unsigned char>(b);

        new_r = (new_r * this->get_args()[0]) / maxval;
        new_g = (new_g * this->get_args()[0]) / maxval;
        new_b = (new_b * this->get_args()[0]) / maxval;

        r = static_cast<char>(new_r);
        g = static_cast<char>(new_g);
        b = static_cast<char>(new_b);

        mysoa.r.push_back(r);
        mysoa.g.push_back(g);
        mysoa.b.push_back(b);
      }

      // Leer de memoria y escribir imagen
      for (long unsigned int i = 0; i < mysoa.r.size(); i++) {
        // Escribimos en el output file los datos almecenados
        output_file.write(&mysoa.r[i], sizeof(r));
        output_file.write(&mysoa.g[i], sizeof(g));
        output_file.write(&mysoa.b[i], sizeof(b));
      }
    } else if (maxval <= MAX_LEVEL) {
      // Este caso es origen maxvalue >256 y destino <256
      char r1 = 0, r2 = 0, g1 = 0, g2 = 0, b1 = 0, b2 = 0;
      soa_rgb_small mysoa;

      // Leer y escribir en memoria
      for (int i = 0; i < width * height; i++) {
        input_file.read(&r1, sizeof(r1));
        input_file.read(&r2, sizeof(r2));
        input_file.read(&g1, sizeof(g1));
        input_file.read(&g2, sizeof(g2));
        input_file.read(&b1, sizeof(b1));
        input_file.read(&b2, sizeof(b2));

        unsigned short r =
            (static_cast<unsigned char>(r2)) | (static_cast<unsigned char>(r1) << BYTE);
        unsigned short g =
            (static_cast<unsigned char>(g2)) | (static_cast<unsigned char>(g1) << BYTE);
        unsigned short b =
            (static_cast<unsigned char>(b2)) | (static_cast<unsigned char>(b1) << BYTE);

        int const new_r = r;
        int const new_g = g;
        int const new_b = b;

        r = static_cast<unsigned short>(new_r * this->get_args()[0] / maxval);
        g = static_cast<unsigned short>(new_g * this->get_args()[0] / maxval);
        b = static_cast<unsigned short>(new_b * this->get_args()[0] / maxval);

        char const r_char = static_cast<char>(r);
        char const g_char = static_cast<char>(g);
        char const b_char = static_cast<char>(b);

        mysoa.r.push_back(r_char);
        mysoa.g.push_back(g_char);
        mysoa.b.push_back(b_char);
      }

      // Leer de memoria y escribir imagen
      for (long unsigned int i = 0; i < mysoa.r.size(); i++) {
        // Escribimos en el output file los datos almecenados
        char const r = mysoa.r[i];
        char const g = mysoa.g[i];
        char const b = mysoa.b[i];

        output_file.write(&r, sizeof(char));
        output_file.write(&g, sizeof(char));
        output_file.write(&b, sizeof(char));
      }
    } else {
      cerr << "Error: maxval no soportado"
           << "\n";
      return -1;
    }
  } else if (this->get_args()[0] <= MAX_LEVEL) {
    output_file << format << " " << width << " " << height << " " << MAX_LEVEL << "\n";
    if (maxval <= MIN_LEVEL) {
      // Este caso es origen maxvalue <256 y destino >256
      char r = 0, g = 0, b = 0;
      soa_rgb_small mysoa;

      // Leer y escribir en memoria
      for (int i = 0; i < width * height; i++) {
        input_file.read(&r, sizeof(r));
        input_file.read(&g, sizeof(g));
        input_file.read(&b, sizeof(b));

        mysoa.r.push_back(r);
        mysoa.g.push_back(g);
        mysoa.b.push_back(b);
      }

      // Leer de memoria y escribir imagen
      for (long unsigned int i = 0; i < mysoa.r.size(); i++) {
        int new_r = static_cast<unsigned char>(mysoa.r[i]);
        int new_g = static_cast<unsigned char>(mysoa.g[i]);
        int new_b = static_cast<unsigned char>(mysoa.b[i]);

        new_r = new_r * this->get_args()[0] / maxval;
        new_g = new_g * this->get_args()[0] / maxval;
        new_b = new_b * this->get_args()[0] / maxval;

        auto r_short = static_cast<unsigned short>(new_r);
        auto g_short = static_cast<unsigned short>(new_g);
        auto b_short = static_cast<unsigned short>(new_b);

        char const r1 = static_cast<char>(r_short >> 8);
        char const r2 = static_cast<char>(r_short & 0xFF);
        char const g1 = static_cast<char>(g_short >> 8);
        char const g2 = static_cast<char>(g_short & 0xFF);
        char const b1 = static_cast<char>(b_short >> 8);
        char const b2 = static_cast<char>(b_short & 0xFF);

        // Escribimos en el output file los datos almecenados
        output_file.write(&r1, sizeof(r1));
        output_file.write(&r2, sizeof(r2));
        output_file.write(&g1, sizeof(g1));
        output_file.write(&g2, sizeof(g2));
        output_file.write(&b1, sizeof(b1));
        output_file.write(&b2, sizeof(b2));
      }
    } else if (maxval <= MAX_LEVEL) {
      // Este caso es origen maxvalue >256 y destino >256
      char r1 = 0, r2 = 0, g1 = 0, g2 = 0, b1 = 0, b2 = 0;
      soa_rgb_big mysoa;

      for (int i = 0; i < width * height; i++) {
        input_file.get(r1);
        input_file.get(r2);
        input_file.get(g1);
        input_file.get(g2);
        input_file.get(b1);
        input_file.get(b2);

        unsigned short r =
            (static_cast<unsigned char>(r2)) | (static_cast<unsigned char>(r1) << BYTE);
        unsigned short g =
            (static_cast<unsigned char>(g2)) | (static_cast<unsigned char>(g1) << BYTE);
        unsigned short b =
            (static_cast<unsigned char>(b2)) | (static_cast<unsigned char>(b1) << BYTE);

        int const new_r = r;
        int const new_g = g;
        int const new_b = b;

        r = static_cast<unsigned short>(new_r * this->get_args()[0] / maxval);
        g = static_cast<unsigned short>(new_g * this->get_args()[0] / maxval);
        b = static_cast<unsigned short>(new_b * this->get_args()[0] / maxval);

        mysoa.r.push_back(r);
        mysoa.g.push_back(g);
        mysoa.b.push_back(b);
      }

      for (long unsigned int i = 0; i < mysoa.r.size(); i++) {
        unsigned short const r = mysoa.r[i];
        unsigned short const g = mysoa.g[i];
        unsigned short const b = mysoa.b[i];

        char const r1 = static_cast<char>(r & 0xFF);
        char const r2 = static_cast<char>(r >> 8 & 0xFF);
        char const g1 = static_cast<char>(g & 0xFF);
        char const g2 = static_cast<char>(g >> 8 & 0xFF);
        char const b1 = static_cast<char>(b & 0xFF);
        char const b2 = static_cast<char>(b >> 8 & 0xFF);

        output_file.write(&r2, sizeof(r1));
        output_file.write(&r1, sizeof(r2));
        output_file.write(&g2, sizeof(g1));
        output_file.write(&g1, sizeof(g2));
        output_file.write(&b2, sizeof(b1));
        output_file.write(&b1, sizeof(b2));
      }
    }
  }
  input_file.close();
  output_file.close();
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
        yh = min(yh, height - 1);

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
        yh = min(yh, height - 1);

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

