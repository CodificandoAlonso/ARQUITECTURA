//
// Created by beto on 4/10/24.
//

#include "progargs.hpp"

#include "binario.hpp"

#include <bits/fs_fwd.h>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Constantes
static int const MAX_LEVEL    = 65535;
static int const MIN_LEVEL    = 255;
static int const MAX_ARGS     = 6;
static int const DECIMAL_BASE = 10;

using namespace std;

Image::Image(int argc, vector<string> const & argv) : argc(argc), argv(argv) { }

int Image::check_args() {
  /*
   * La aplicación tomará los siguientes parámetros:
   *      - Ruta del archivo de entrada
   *      - Ruta del archivo de salida
   *      - Opción a ejecutar (info, maxlevel, resize, cutfreq, compress)
   *      - Parámetros adicionales
   */
  // Para mayor facilidad de uso
  int const argc      = this->argc;
  vector<string> argv = this->argv;

  if (argc <= 3) {
    cerr << "Error: Invalid number of arguments: " << argc << '\n';
    return -1;
  }
  this->input_file  = argv[1];
  this->output_file = argv[2];

  // Comprobaremos ahora que el tercer argumento corresponde con la opción a ejecutar
  string const & option = argv[3];
  if (option != "info" && option != "maxlevel" && option != "resize" && option != "cutfreq" &&
      option != "compress") {
    cerr << "Error: Invalid option: " << option << '\n';
    return -1;
  }
  this->optype = option;

  // Si la opción es info, deben ser exactamente tres argumentos
  if (option == "info" && argc != 4) {
    cerr << "Error: Invalid number of arguments for option info: " << argc << '\n';
    return -1;
  }

  // Si la opción es maxlevel, el número de argumentos debe ser exactamente cuatro. El cuarto
  // argumento debe ser un número entero entre los valores 0 y 65535.
  else if (option == "maxlevel") {
    if (argc != MAX_ARGS - 1) {
      cerr << "Error: Invalid number of arguments for option maxlevel: " << argc << '\n';
      return -1;
    }
    // Comprobamos que el cuarto argumento sea un número entero entre 0 y 65535, teniendo en cuenta
    // que tampoco puede ser una palabra
    char * end    = nullptr;
    long argument = strtol(argv[4].c_str(), &end, DECIMAL_BASE);
    if (argument < 0 || argument > MAX_LEVEL || (argument == 0 && argv[4][0] != '0')) {
      cerr << "Error: Invalid argument for option maxlevel: " << argv[4] << '\n';
      return -1;
    }
    this->args.push_back(static_cast<int>(argument));
  }

  // Si la opción es resize, el número de argumentos debe ser exactamente cinco. El cuarto y quinto
  // argumento deben ser números enteros positivos.
  else if (option == "resize") {
    if (argc != MAX_ARGS) {
      cerr << "Error: Invalid number of arguments for option resize: " << argc << '\n';
      return -1;
    }
    // Comprobamos que el cuarto y quinto argumento sean números enteros positivos
    char * end     = nullptr;
    long argument1 = strtol(argv[4].c_str(), &end, DECIMAL_BASE);
    long argument2 = strtol(argv[MAX_ARGS - 1].c_str(), &end, DECIMAL_BASE);
    if (argument1 <= 0) {
      cerr << "Error: Invalid resize width: " << argv[4] << '\n';
      return -1;
    }
    if (argument2 <= 0) {
      cerr << "Error: Invalid resize height: " << argv[MAX_ARGS - 1] << '\n';
      return -1;
    }
    this->args.push_back(static_cast<int>(argument1));
    this->args.push_back(static_cast<int>(argument2));
  }

  // Si la opción es cutfreq, el número de argumentos debe ser exactamente cuatro. El cuarto
  // argumento debe ser un número entero positivo.
  else if (option == "cutfreq") {
    if (argc != MAX_ARGS - 1) {
      cerr << "Error: Invalid number of arguments for cutfreq: " << argc << '\n';
      return -1;
    }
    // Comprobamos que el cuarto argumento sea un número entero positivo
    char * end    = nullptr;
    long argument = strtol(argv[4].c_str(), &end, DECIMAL_BASE);
    if (argument <= 0) {
      cerr << "Error: Invalid cutfreq: " << argv[4] << '\n';
      return -1;
    }
    this->args.push_back(static_cast<int>(argument));
  }

  // Si la opción es compress, el número de argumentos debe ser exactamente tres.
  else if (option == "compress" && argc != 4) {
    cerr << "Error: Invalid extra arguments for compress: " << argc << '\n';
    return -1;
  }

  return 0;
}

int Image::info() {
  /*
   * Función común a ImageAOS e ImageSOA para leer los metadatos de la imagen de entrada .ppm.
   */
  cout << "Reading metadata from file " << input_file << '\n';

  ifstream file(input_file, ios::in | ios::binary);

  if (!file) {
    cerr << "Error opening file: " << input_file << '\n';
    return -1;
  }

  string format;
  file >> format;

  float width  = NAN;
  float height = NAN;
  file >> width >> height;

  int max_color_value = 0;
  file >> max_color_value;

  cout << "Format: " << format << '\n';
  cout << "Width: " << width << '\n';
  cout << "Height: " << height << '\n';
  cout << "Max color value: " << max_color_value << '\n';

  file.close();

  return 0;
}

int Image::maxlevel() {
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

  if (this->args[0] <= MIN_LEVEL) {  // Imagen de salida 255
    output_file << format << " " << width << " " << height << " " << MIN_LEVEL << '\n';

    if (maxval <= MIN_LEVEL) {  // Imagen de entrada 255
      /*
       * Si se desea escalar una imagen cuyo máximo nivel de intensidad es 255 a
       * otra con un nivel de intensidad menor a 256, leemos la imagen de entrada
       * de 8 bits en 8 bits y escribimos en la imagen de salida de 8 bits en 8 bits.
       */
      char r = 0, g = 0, b = 0;
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

        output_file.write(&r, sizeof(unsigned char));
        output_file.write(&g, sizeof(unsigned char));
        output_file.write(&b, sizeof(unsigned char));
      }
    } else if (maxval <= MAX_LEVEL) {  // Imagen de entrada 65535
      /*
       * Si se desea escalar una imagen cuyo máximo nivel de intensidad es mayor a
       * 255 a otra con un nivel de intensidad entre 0 y 255, leemos la imagen de
       * entrada de 8 bits en 8 bits (teniendo en cuenta que cada color ocupa 2 bytes)
       * y escribimos en la imagen de salida de 16 bits
       */
      unsigned short r = 0, g = 0, b = 0;
      for (int i = 0; i < width * height; i++) {
        r = read_binary_16(input_file);
        g = read_binary_16(input_file);
        b = read_binary_16(input_file);

        r = swap16(r);
        g = swap16(g);
        b = swap16(b);

        int const new_r = r;
        int const new_g = g;
        int const new_b = b;

        r = static_cast<unsigned short>(new_r * this->get_args()[0] / maxval);
        g = static_cast<unsigned short>(new_g * this->get_args()[0] / maxval);
        b = static_cast<unsigned short>(new_b * this->get_args()[0] / maxval);

        char const r_char = static_cast<char>(r);
        char const g_char = static_cast<char>(g);
        char const b_char = static_cast<char>(b);

        output_file.write(&r_char, sizeof(unsigned char));
        output_file.write(&g_char, sizeof(unsigned char));
        output_file.write(&b_char, sizeof(unsigned char));
      }
    } else {
      cerr << "Formato incorrecto" << '\n';
      return -1;
    }
  } else if (this->args[0] <= MAX_LEVEL) {  // Imagen de salida 65535.
    output_file << format << " " << width << " " << height << " " << MAX_LEVEL << '\n';
    if (maxval <= MIN_LEVEL) {  // Imagen de entrada 255
      /*
       * Si se desea escalar una imagen cuyo máximo nivel de intensidad es mayor a
       * 255 a otra con un nivel de intensidad entre 255 y 65535, leemos la imagen de
       * entrada de 16 bits en 16 bits y escribimos en la imagen de salida de 16 bits
       * en 16 bits.
       */
      char r = 0, g = 0, b = 0;
      for (int i = 0; i < width * height; i++) {
        input_file.read(&r, sizeof(unsigned char));
        input_file.read(&g, sizeof(unsigned char));
        input_file.read(&b, sizeof(unsigned char));

        auto new_r = static_cast<int>(static_cast<unsigned char>(r));
        auto new_g = static_cast<int>(static_cast<unsigned char>(g));
        auto new_b = static_cast<int>(static_cast<unsigned char>(b));

        auto r_16 = static_cast<unsigned short>(new_r * this->get_args()[0] / maxval);
        auto g_16 = static_cast<unsigned short>(new_g * this->get_args()[0] / maxval);
        auto b_16 = static_cast<unsigned short>(new_b * this->get_args()[0] / maxval);

        r_16 = swap16(r_16);
        g_16 = swap16(g_16);
        b_16 = swap16(b_16);

        write_binary_16(output_file, r_16);
        write_binary_16(output_file, g_16);
        write_binary_16(output_file, b_16);
      }
    } else if (maxval <= MAX_LEVEL) {  // Imagen de entrada 65535
      /*
       * Si se desea escalar una imagen cuyo máximo nivel de intensidad es mayor a 255 a
       * otra con un nivel de intensidad entre 0 y 65535, leemos la imagen de entrada
       * de 8 bits en 8 bits y escribimos en la imagen de salida de 16 bits en 16 bits.
       */
      unsigned short r = 0, g = 0, b = 0;
      for (int i = 0; i < width * height; i++) {
        r = read_binary_16(input_file);
        g = read_binary_16(input_file);
        b = read_binary_16(input_file);

        r = swap16(r);
        g = swap16(g);
        b = swap16(b);

        int const new_r = r;
        int const new_g = g;
        int const new_b = b;

        auto r_16 = static_cast<uint16_t>((new_r * this->args[0]) / maxval);
        auto g_16 = static_cast<uint16_t>((new_g * this->args[0]) / maxval);
        auto b_16 = static_cast<uint16_t>((new_b * this->args[0]) / maxval);

        r_16 = swap16(r_16);
        g_16 = swap16(g_16);
        b_16 = swap16(b_16);

        write_binary_16(output_file, r_16);
        write_binary_16(output_file, g_16);
        write_binary_16(output_file, b_16);
      }
    } else {
      cerr << "Incorret Format" << '\n';
      return -1;
    }
    }
  return 0;
}
