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
#include <unistd.h>
#include <vector>

// Constantes
static constexpr int MAX_LEVEL    = 65535;
static constexpr int MIN_LEVEL    = 255;
static constexpr int MAX_ARGS     = 6;
static constexpr int DECIMAL_BASE = 10;

using namespace std;

/**
 * Constructor de la superclase Imagen.
 */
Image::Image(int const argc, vector<string> const & argv) : argc(argc), argv(argv) { }

bool Image::info_constraints(int const argc) {
  // Si la opción es info, deben ser exactamente tres argumentos
  if (argc != 4) {
    cerr << "Error: Invalid number of arguments for option info: " << argc << '\n';
    return true;
  }
  return false;
}

const std::vector<std::string>& Image::getArgv() const {
  return argv;
}

bool Image::maxval_constraints(int const argc, vector<string> const & argv) {
  // Si la opción es maxlevel, el número de argumentos debe ser exactamente cuatro. El cuarto
  // argumento debe ser un número entero entre los valores 0 y 65535.
  if (argc != MAX_ARGS - 1) {
    cerr << "Error: Invalid number of arguments for option maxlevel: " << argc << '\n';
    return true;
  }
  // Comprobamos que el cuarto argumento sea un número entero entre 0 y 65535, teniendo en cuenta
  // que tampoco puede ser una palabra
  char * end          = nullptr;
  long const argument = strtol(argv[4].c_str(), &end, DECIMAL_BASE);
  if (argument < 0 || argument > MAX_LEVEL || (argument == 0 && argv[4][0] != '0')) {
    cerr << "Error: Invalid argument for option maxlevel: " << argv[4] << '\n';
    return true;
  }
  this->args.push_back(static_cast<int>(argument));
  return false;
}

bool Image::resize_constraints(int const argc, vector<string> const & argv) {
  if (argc != MAX_ARGS) {
    cerr << "Error: Invalid number of arguments for option resize: " << argc << '\n';
    return true;
  }
  // Comprobamos que el cuarto y quinto argumento sean números enteros positivos
  char * end           = nullptr;
  long const argument1 = strtol(argv[4].c_str(), &end, DECIMAL_BASE);
  long const argument2 = strtol(argv[MAX_ARGS - 1].c_str(), &end, DECIMAL_BASE);
  if (argument1 <= 0) {
    cerr << "Error: Invalid resize width: " << argv[4] << '\n';
    return true;
  }
  if (argument2 <= 0) {
    cerr << "Error: Invalid resize height: " << argv[MAX_ARGS - 1] << '\n';
    return true;
  }
  this->args.push_back(static_cast<int>(argument1));
  this->args.push_back(static_cast<int>(argument2));
  return false;
}

bool Image::cutfreq_constraints(int const argc, vector<string> const & argv) {
  // Si la opción es cutfreq, el número de argumentos debe ser exactamente cuatro. El cuarto
  // argumento debe ser un número entero positivo.
  if (argc != MAX_ARGS - 1) {
    cerr << "Error: Invalid number of arguments for cutfreq: " << argc << '\n';
    return true;
  }
  // Comprobamos que el cuarto argumento sea un número entero positivo
  char * end    = nullptr;
  long argument = strtol(argv[4].c_str(), &end, DECIMAL_BASE);
  if (argument <= 0) {
    cerr << "Error: Invalid cutfreq: " << argv[4] << '\n';
    return true;
  }
  this->args.push_back(static_cast<int>(argument));
  return false;
}

bool Image::compress_constraints(int const argc) {
  // Si la opción es compress, el número de argumentos debe ser exactamente tres.
  if (argc != 4) {
    cerr << "Error: Invalid extra arguments for compress: " << argc << '\n';
    return true;
  }
  return false;
}

/**
 * Función que verifica los parámetros de entrada.
 */
int Image::check_args() {
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

  if (option == "info") {
    if (info_constraints(argc)) { return -1; }
  } else if (option == "maxlevel") {
    if (maxval_constraints(argc, argv)) { return -1; }
  } else if (option == "resize") {
    if (resize_constraints(argc, argv)) { return -1; }
  } else if (option == "cutfreq") {
    if (cutfreq_constraints(argc, argv)) { return -1; }
  } else if (option == "compress") {
    if (compress_constraints(argc)) { return -1; }
  }

  return 0;
}

/**
 * Función que esscribe por la salida estándar los metadatos de la imagen.
 */
int Image::info() const {
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

/**
 * Función para leer el archivo de entrada y escribir los atributos de la superclase.
 */
void Image::get_imgdata() {
  ifstream input_file(this->input_file, ios::binary);

  if (!input_file) {
    cerr << "Error al abrir el archivo de entrada"
         << "\n";
  }

  string format;
  int width  = 0;
  int height = 0;
  int maxval = 0;

  input_file >> format >> width >> height >> maxval;
  input_file.ignore(1);
  this->if_input_file = move(input_file);
  this->format        = format;
  this->width         = width;
  this->height        = height;
  this->maxval        = maxval;
}

/**
 * Función para escribir la cabecera del archivo de salida.
 */
void Image::write_out(int const level) {
  ofstream output_file(this->get_output_file(), ios::binary);

  if (!output_file) {
    cerr << "Error al abrir el archivo de salida"
         << "\n";
  }

  string const format = this->format;
  int const min_level = level;
  int const width     = this->width;
  int const height    = this->height;
  output_file << format << " " << width << " " << height << " " << min_level << '\n';
  this->of_output_file = move(output_file);
}

/*
 * Caso 1 de la función maxlevel:
 * imagen de entrada con maxlevel = 255
 * imagen de salida con maxlevel = 255
 */
void Image::min_min() {
  /*
   * Si se desea escalar una imagen cuyo máximo nivel de intensidad es 255 a
   * otra con un nivel de intensidad menor a 256, leemos la imagen de entrada
   * de 8 bits en 8 bits y escribimos en la imagen de salida de 8 bits en 8 bits.
   */
  unsigned char red = 0;
  unsigned char grn = 0;
  unsigned char blu = 0;
  for (int i = 0; i < width * height; i++) {
    red = read_binary_8(this->if_input_file);
    grn = read_binary_8(this->if_input_file);
    blu = read_binary_8(this->if_input_file);

    red = static_cast<unsigned char>((red * this->get_args()[0]) / maxval);
    grn = static_cast<unsigned char>((grn * this->get_args()[0]) / maxval);
    blu = static_cast<unsigned char>((blu * this->get_args()[0]) / maxval);

    write_binary_8(this->of_output_file, red);
    write_binary_8(this->of_output_file, grn);
    write_binary_8(this->of_output_file, blu);
  }
  this->if_input_file.close();
}

/*
 * Caso 2 de la función maxlevel:
 * imagen de entrada con maxlevel = 65535
 * imagen de salida con maxlevel = 255
 */
void Image::max_min() {
  unsigned short red = 0;
  unsigned short grn = 0;
  unsigned short blu = 0;
  for (int i = 0; i < width * height; i++) {
    red = read_binary_16(this->if_input_file);
    grn = read_binary_16(this->if_input_file);
    blu = read_binary_16(this->if_input_file);

    red = swap16(red);
    grn = swap16(grn);
    blu = swap16(blu);

    int const new_r = red;
    int const new_g = grn;
    int const new_b = blu;

    auto r_8 = static_cast<unsigned char>(new_r * this->get_args()[0] / maxval);
    auto g_8 = static_cast<unsigned char>(new_g * this->get_args()[0] / maxval);
    auto b_8 = static_cast<unsigned char>(new_b * this->get_args()[0] / maxval);

    write_binary_8(this->of_output_file, r_8);
    write_binary_8(this->of_output_file, g_8);
    write_binary_8(this->of_output_file, b_8);
  }
}

/**
 * Caso 3 de la función maxlevel:
 * imagen de entrada con maxlevel = 255
 * imagen de salida con maxlevel = 65535
 */
void Image::min_max() {
  unsigned char red = 0;
  unsigned char grn = 0;
  unsigned char blu = 0;
  for (int i = 0; i < width * height; i++) {
    red = read_binary_8(this->if_input_file);
    grn = read_binary_8(this->if_input_file);
    blu = read_binary_8(this->if_input_file);

    auto r_16 = static_cast<unsigned short>(red * this->get_args()[0] / maxval);
    auto g_16 = static_cast<unsigned short>(grn * this->get_args()[0] / maxval);
    auto b_16 = static_cast<unsigned short>(blu * this->get_args()[0] / maxval);

    r_16 = swap16(r_16);
    g_16 = swap16(g_16);
    b_16 = swap16(b_16);

    write_binary_16(this->of_output_file, r_16);
    write_binary_16(this->of_output_file, g_16);
    write_binary_16(this->of_output_file, b_16);
  }
}

/**
 * Caso 4 de la función maxlevel:
 * imagen de entrada con maxlevel = 65535
 * imagen de salida con maxlevel = 65535
 */
void Image::max_max() {
  /*
   * Si se desea escalar una imagen cuyo máximo nivel de intensidad es mayor a 255 a
   * otra con un nivel de intensidad entre 0 y 65535, leemos la imagen de entrada
   * de 8 bits en 8 bits y escribimos en la imagen de salida de 16 bits en 16 bits.
   */
  unsigned short red = 0;
  unsigned short grn = 0;
  unsigned short blu = 0;
  for (int i = 0; i < width * height; i++) {
    red = read_binary_16(this->if_input_file);
    grn = read_binary_16(this->if_input_file);
    blu = read_binary_16(this->if_input_file);

    red = swap16(red);
    grn = swap16(grn);
    blu = swap16(blu);

    auto r_16 = static_cast<uint16_t>((red * this->args[0]) / maxval);
    auto g_16 = static_cast<uint16_t>((grn * this->args[0]) / maxval);
    auto b_16 = static_cast<uint16_t>((blu * this->args[0]) / maxval);

    r_16 = swap16(r_16);
    g_16 = swap16(g_16);
    b_16 = swap16(b_16);

    write_binary_16(this->of_output_file, r_16);
    write_binary_16(this->of_output_file, g_16);
    write_binary_16(this->of_output_file, b_16);
  }
}

int Image::maxlevel() {
  get_imgdata();
  if (this->args[0] <= MIN_LEVEL) {  // Imagen de salida 255
    write_out(MIN_LEVEL);
    if (maxval <= MIN_LEVEL) {
      min_min();
    } else if (maxval <= MAX_LEVEL) {  // Imagen de entrada 65535
      max_min();
    } else {
      cerr << "Incorrect format" << '\n';
      return -1;
    }
  } else if (this->args[0] <= MAX_LEVEL) {  // Imagen de salida 65535.
    write_out(MAX_LEVEL);
    if (maxval <= MIN_LEVEL) {  // Imagen de entrada 255
      min_max();
    } else if (maxval <= MAX_LEVEL) {
      max_max();
    }
  } else {
    cerr << "Incorrect Format" << '\n';
    return -1;
    }

  return 0;
}
