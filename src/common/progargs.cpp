//
// Created by beto on 4/10/24.
//

#include "progargs.hpp"

#include "binario.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

// Constantes
static constexpr int MAX_LEVEL    = 65535;
static constexpr int MIN_LEVEL    = 255;
static constexpr int MAX_ARGS     = 6;
static constexpr int DECIMAL_BASE = 10;
static constexpr int MAX_DIST     = 100000;
using namespace std;

/**
 * Constructor de la superclase Imagen.
 */
Image::Image(int const argc, vector<string> const & argv) : argc(argc), argv(argv) { }

std::vector<std::string> const & Image::getArgv() const {
  return argv;
}

bool Image::info_constraints(int const argc) {
  // Si la opción es info, deben ser exactamente tres argumentos
  if (argc != 4) {
    cerr << "Error: Invalid number of arguments for option info: " << argc << '\n';
    return true;
  }
  return false;
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
    cerr << "Error: Invalid number of arguments for option resize: " + std::to_string(argc) << '\n';
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
    cerr << "Error: Invalid number of arguments for cutfreq: " + std::to_string(argc);
    return true;
  }
  // Comprobamos que el cuarto argumento sea un número entero positivo
  char * end    = nullptr;
  long argument = strtol(argv[4].c_str(), &end, DECIMAL_BASE);
  if (argument <= 0) {
    cerr << "Error: Invalid cutfreq: " + argv[4] << '\n';
    return true;
  }
  this->args.push_back(static_cast<int>(argument));
  return false;
}

bool Image::compress_constraints(int const argc) {
  // Si la opción es compress, el número de argumentos debe ser exactamente 4.
  if (argc != 4) {
    cerr << "Error: Invalid number of arguments for compress: " + to_string(argc) << "\n";
    return true;
  }
  return false;
}

/**
 * Función que verifica los parámetros de entrada del programa.
 */
int Image::check_args() {
  int const argc      = this->argc;
  vector<string> argv = this->argv;

  if (argc <= 3) {
    cerr << "Error: Invalid number of arguments: " + to_string(argc) << "\n";
    return -1;
  }
  this->input_file  = argv[1];
  this->output_file = argv[2];

  // Comprobaremos ahora que el tercer argumento corresponde con la opción a ejecutar
  string const & option = argv[3];
  if (option != "info" && option != "maxlevel" && option != "resize" && option != "cutfreq" &&
      option != "compress") {
    cerr << "Error: Invalid option: " + option << "\n";
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
  cout << "Reading metadata from file " << input_file << '\n';

  ifstream file(input_file, ios::in | ios::binary);

  if (!file) {
    cerr << "Error opening file: " << input_file << '\n';
    return -1;
  }

  string format;
  file >> format;

  float width  = 0.0;
  float height = 0.0;
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
 * Almacena en el atributo if_input_file el archivo de entrada.
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
  this->if_input_file = std::move(input_file);
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
  this->of_output_file = std::move(output_file);
}

/*
 * Caso 1 de la función maxlevel:
 * imagen de entrada con maxlevel = 255
 * imagen de salida con maxlevel = 255
 */
void Image::min_min() {
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

/**
 * Implementación de la función maxlevel. Esta función modifica el valor máximo de intensidad,
 * alterando la exposición de la imagen original.
 */
int Image::maxlevel() {
  get_imgdata();
  if (this->args[0] <= MIN_LEVEL) {  // Imagen de salida 255
    write_out(this->get_args()[0]);
    if (maxval <= MIN_LEVEL) {
      min_min();
    } else if (maxval <= MAX_LEVEL) {  // Imagen de entrada 65535
      max_min();
    } else {
      cerr << "Incorrect format" << '\n';
      return -1;
    }
  } else if (this->args[0] <= MAX_LEVEL) {  // Imagen de salida 65535.
    write_out(this->get_args()[0]);
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

/**
 *Funcion auxiliar de cutfreq que se encarga de recibir un deque, un contador y un value.
 *Esta funcion se encarga de ordenar los contador elementos del deque en otro deque en funcion del
 *color indicado por el value
 */
deque<pair<__uint32_t, __uint16_t>> Image::cf_same_bgr_vector(params_same_vector_small params) {
  // Value será 1 para blue, 2 para green y 3 para red
  deque<pair<__uint32_t, __uint16_t>> color_vector;
  __uint8_t color = 0;

  for (size_t i = 0; i < params.counter; i++) {
    if (params.value == 1) {
      color = extractblue(params.father_vector[i].first);
    } else if (params.value == 2) {
      color = extractgreen(params.father_vector[i].first);
    } else if (params.value == 3) {
      color = extractred(params.father_vector[i].first);
    }
    color_vector.emplace_back(params.father_vector[i].first, color);
  }
  ranges::sort(color_vector, [](auto const & op1, auto const & op2) {
    return op1.second > op2.second;
  });
  return color_vector;
}

/**
 *Igual que la de arriba pero en version BIG, es decir, para imagenes de valor maximo de intensidad
 *de color 65535
 */
deque<pair<__uint64_t, __uint16_t>> Image::cf_same_bgr_vector_BIG(params_same_vector_BIG params) {
  // Value será 1 para blue, 2 para green y 3 para red
  deque<pair<__uint64_t, __uint16_t>> color_vector;
  __uint16_t color = 0;
  // Usamos `params.counter` y `params.value` para acceder a los valores originales
  for (size_t i = 0; i < params.counter; i++) {
    // Accedemos a `father_vector` como copia local en `params`
    if (params.value == 1) {
      color = extractblueBIG(params.father_vector[i].first);
    } else if (params.value == 2) {
      color = extractgreenBIG(params.father_vector[i].first);
    } else if (params.value == 3) {
      color = extractredBIG(params.father_vector[i].first);
    }
    color_vector.emplace_back(params.father_vector[i].first, color);
  }
  // Ordenamos el color_vector por el segundo elemento del par en orden descendente
  ranges::sort(color_vector, [](auto const & op1, auto const & op2) {
    return op1.second > op2.second;
  });
  return color_vector;
}

/**
 *Funcion auxiliar de cutfreq que se invoca en caso de igualdad de valor de componentes en el
 *color_vector entre el elemento 0 y el 1. Esta se encarga de comprobar cuantos valores posteriores
 *tienen tambien esa igualdad de componentes. En caso de ser asi, devueve el numero de elementos
 *con igualdad, y en caso contrario, compara el valor de la siguiente componente y elimina o el
 *0 o el 1.
 */
int Image::cf_check_and_delete(deque<pair<__uint32_t, __uint16_t>> & color_vector, int const color,
                               unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                               deque<pair<__uint32_t, __uint16_t>> & bluevalues) {
  // 1 para azul, 0 para verde
  size_t meanwhile = 0;
  while (color_vector[meanwhile].second == color_vector[meanwhile + 1].second) { meanwhile++; }
  if (meanwhile == 1) {
    size_t my_index  = 0;
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
      my_index = cf_search_in_blue(bluevalues, color_vector[0].first);
      cf_delete_from_deque(bluevalues, my_index);
    } else {
      Deleteitems[{color_vector[1].first}] = 0;
      my_index                             = cf_search_in_blue(bluevalues, color_vector[1].first);
      cf_delete_from_deque(bluevalues, my_index);
    }
    return 0;
  }
  return static_cast<int>(meanwhile + 1);
}

/**
 *Igual que la de arriba pero en version BIG, es decir, para imagenes de valor maximo de intensidad
 *de color 65535
 */
int Image::cf_check_and_delete_BIG(deque<pair<__uint64_t, __uint16_t>> & color_vector,
                                   int const color,
                                   unordered_map<__uint64_t, __uint64_t> & Deleteitems,
                                   deque<pair<__uint64_t, __uint16_t>> & bluevalues) {
  // 1 para azul, 0 para verde
  size_t meanwhile = 0;
  while (color_vector[meanwhile].second == color_vector[meanwhile + 1].second) { meanwhile++; }
  if (meanwhile == 1) {
    size_t my_index   = 0;
    __uint16_t value0 = 0;
    __uint16_t value1 = 0;
    if (color == 1) {
      value0 = extractgreenBIG(color_vector[0].first);
      value1 = extractgreenBIG(color_vector[1].first);
    } else {
      value0 = extractredBIG(color_vector[0].first);
      value1 = extractredBIG(color_vector[1].first);
    }
    if (value0 - value1 > 0) {
      Deleteitems[{color_vector[0].first}] = 0;
      // color_vector.pop_front();
      my_index = cf_search_in_blue_BIG(bluevalues, color_vector[0].first);
      cf_delete_from_deque_BIG(bluevalues, my_index);
    } else {
      Deleteitems[{color_vector[1].first}] = 0;
      my_index = cf_search_in_blue_BIG(bluevalues, color_vector[1].first);
      cf_delete_from_deque_BIG(bluevalues, my_index);
    }
    return 0;
  }
  return static_cast<int>(meanwhile + 1);
}

/**
 *Funcion auxiliar del cutfreq que se encarga de popear el n elemento del deque
 */
void Image::cf_delete_from_deque(deque<pair<__uint32_t, __uint16_t>> & deque_general,
                                 size_t index) {
  swap(deque_general[0], deque_general[index]);
  deque_general.pop_front();
}

/**
 *Igual que la de arriba pero en version BIG, es decir, para imagenes de valor maximo de intensidad
 *de color 65535
 */
void Image::cf_delete_from_deque_BIG(deque<pair<__uint64_t, __uint16_t>> & deque_general,
                                     size_t index) {
  swap(deque_general[0], deque_general[index]);
  deque_general.pop_front();
}

/**
 *Funcion auxiliar del cutfreq que dado un numero te devuelve el indice de donde esta colocado en un deque
 */
size_t Image::cf_search_in_blue(deque<pair<__uint32_t, unsigned short>> & pairs,
                                __uint32_t & first) {
  for (size_t i = 0; i < pairs.size(); i++) {
    if (pairs[i].first == first) { return i; }
  }
  return 0;
}

/**
 *Igual que la de arriba pero en version BIG, es decir, para imagenes de valor maximo de intensidad
 *de color 65535
 */
size_t Image::cf_search_in_blue_BIG(deque<pair<__uint64_t, unsigned short>> & pairs,
                                    __uint64_t & first) {
  for (size_t i = 0; i < pairs.size(); i++) {
    if (pairs[i].first == first) { return i; }
  }
  return 0;
}


/**
 *Funcion auxiliar del cutfreq que se encarga de eliminar el primer elemento de Bluevalues, asi como
 *de guardarlo en Deleteitems
 */
void Image::cf_delete_first_blue_value(unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                                       int & num_left,
                                       deque<pair<__uint32_t, __uint16_t>> & bluevalues) {
  Deleteitems[bluevalues[0].first] = 0;
  bluevalues.pop_front();
  num_left--;
}

/**
 *Igual que la de arriba pero en version BIG, es decir, para imagenes de valor maximo de intensidad
 *de color 65535
 */
void Image::cf_delete_first_blue_value_BIG(unordered_map<__uint64_t, __uint64_t> & Deleteitems,
                                           int & num_left,
                                           deque<pair<__uint64_t, __uint16_t>> & bluevalues) {
  Deleteitems[bluevalues[0].first] = 0;
  bluevalues.pop_front();
  num_left--;
}

/**
 *Funcion auxiliar del cutfreq que se encarga de tratar el caso en el que hay mas de 2 colores
 *con la misma componente azul y la longitud de este es mayor que el numero de elementos restantes
 *por mandar a eliminación. Para eso nos creamos un deque con los colores pero ahora ordenados por
 *verde, y se vuelve a hacer la misma comparacion, se va popeando el elemento de Bluevalues que
 *corresponda y guardandolo en Deleteitems.
 */
void Image::cf_equal_blue_case(params_equal_blu * params) {
  while (*params->num_left > 0) {
    params_same_vector_small const params_green = {.father_vector = *params->bluevalues,
                                                   .value         = 2,
                                                   .counter =
                                                       static_cast<size_t>(*params->my_meanwhile)};

    auto greenvalues = cf_same_bgr_vector(params_green);
    if (greenvalues[0].second == greenvalues[1].second) {
      *params->my_meanwhile =
          cf_check_and_delete(greenvalues, 0, *params->Deleteitems, *params->bluevalues);

      if (*params->my_meanwhile > 0) {
        params_same_vector_small const params_red = {
          .father_vector = greenvalues,
          .value         = 3,
          .counter       = static_cast<size_t>(*params->my_meanwhile)};

        auto redvalues                             = cf_same_bgr_vector(params_red);
        (*params->Deleteitems)[redvalues[0].first] = 0;

        params->my_index = cf_search_in_blue(*params->bluevalues, redvalues[0].first);
        cf_delete_from_deque(*params->bluevalues, params->my_index);
        (*params->num_left)--;
      } else {
        (*params->num_left)--;
      }
    } else {
      (*params->Deleteitems)[greenvalues[0].first] = 0;
      params->my_index = cf_search_in_blue(*params->bluevalues, greenvalues[0].first);
      cf_delete_from_deque(*params->bluevalues, params->my_index);
      (*params->num_left)--;
    }
  }
}

/**
 *Igual que la de arriba pero en version BIG, es decir, para imagenes de valor maximo de intensidad
 *de color 65535
 */
void Image::cf_equal_blue_case_BIG(params_equal_blu_BIG * params) {
  params_same_vector_BIG const params_green = {.father_vector = *params->bluevalues,
                                               .value         = 2,
                                               .counter =
                                                   static_cast<size_t>(*params->my_meanwhile)};

  auto greenvalues = cf_same_bgr_vector_BIG(params_green);
  if (greenvalues[0].second == greenvalues[1].second) {
    *params->my_meanwhile =
        cf_check_and_delete_BIG(greenvalues, 0, *params->Deleteitems, *params->bluevalues);

    if (*params->my_meanwhile > 0) {
      params_same_vector_BIG const params_red = {.father_vector = greenvalues,
                                                 .value         = 3,
                                                 .counter =
                                                     static_cast<size_t>(*params->my_meanwhile)};

      auto redvalues                             = cf_same_bgr_vector_BIG(params_red);
      (*params->Deleteitems)[redvalues[0].first] = 0;

      params->my_index = cf_search_in_blue_BIG(*params->bluevalues, redvalues[0].first);
      cf_delete_from_deque_BIG(*params->bluevalues, params->my_index);
      (*params->num_left)--;
    } else {
      (*params->num_left)--;
    }
  } else {
    (*params->Deleteitems)[greenvalues[0].first] = 0;
    params->my_index = cf_search_in_blue_BIG(*params->bluevalues, greenvalues[0].first);
    cf_delete_from_deque_BIG(*params->bluevalues, params->my_index);
    (*params->num_left)--;
  }
}

/**
 *Funcion auxiliar del cutfreq que se encarga de gestionar el caso de los colores con igual
 *frecuencia que el n-esimo. Este recibe esos colores ordenados segun el azul de sus componentes
 *y se encarga de ir eliminando segun criterios del enunciado, hasta que complete los n colores
 *solicitados
 */
unordered_map<__uint32_t, __uint32_t>
    Image::cf_check_colors_to_delete(unordered_map<__uint32_t, __uint32_t> Deleteitems,
                                     int num_left, deque<pair<__uint32_t, __uint16_t>> bluevalues) {
  while (num_left > 0) {
    if (bluevalues.size() > 1) {
      if (bluevalues[0].second == bluevalues[1].second) {
        if (int my_meanwhile = cf_check_and_delete(bluevalues, 1, Deleteitems, bluevalues);
            my_meanwhile > 0) {
          if (my_meanwhile < num_left) {
            auto iterator = static_cast<size_t>(my_meanwhile);
            for (size_t iii = 0; iii < iterator; iii++) {
              cf_delete_first_blue_value(Deleteitems, num_left, bluevalues);
            }
          } else {
            constexpr size_t my_index = 0;
            params_equal_blu params   = {
                .Deleteitems  = &Deleteitems,
                .num_left     = &num_left,
                .bluevalues   = &bluevalues,
                .my_index     = my_index,
                .my_meanwhile = &my_meanwhile,
            };
            cf_equal_blue_case(&params);
          }
        } else {
          num_left--;
        }
      } else {
        cf_delete_first_blue_value(Deleteitems, num_left, bluevalues);
      }
    } else {
      cf_delete_first_blue_value(Deleteitems, num_left, bluevalues);
    }
  }
  return Deleteitems;
}

/**
 *Igual que la de arriba pero en version BIG, es decir, para imagenes de valor maximo de intensidad
 *de color 65535
 */
unordered_map<__uint64_t, __uint64_t>
    Image::cf_check_colors_to_delete_BIG(unordered_map<__uint64_t, __uint64_t> Deleteitems,
                                         int num_left,
                                         deque<pair<__uint64_t, __uint16_t>> bluevalues) {
  while (num_left > 0) {
    if (bluevalues.size() > 1) {
      if (bluevalues[0].second == bluevalues[1].second) {
        if (int my_meanwhile = cf_check_and_delete_BIG(bluevalues, 1, Deleteitems, bluevalues);
            my_meanwhile > 0) {
          if (my_meanwhile < num_left) {
            auto iterator = static_cast<size_t>(my_meanwhile);
            for (size_t iii = 0; iii < iterator; iii++) {
              cf_delete_first_blue_value_BIG(Deleteitems, num_left, bluevalues);
            }
          } else {
            constexpr size_t my_index   = 0;
            params_equal_blu_BIG params = {
              .Deleteitems  = &Deleteitems,
              .num_left     = &num_left,
              .bluevalues   = &bluevalues,
              .my_index     = my_index,
              .my_meanwhile = &my_meanwhile,
            };
            cf_equal_blue_case_BIG(&params);
          }
        } else {
          num_left--;
        }
      } else {
        cf_delete_first_blue_value_BIG(Deleteitems, num_left, bluevalues);
      }
    } else {
      cf_delete_first_blue_value_BIG(Deleteitems, num_left, bluevalues);
    }
  }
  return Deleteitems;
}

/**
 *Funcion auxiliar del cutfreq que se encarga de buscar si existe algun elemento en los nodos
 *vecinos que tenga menor distancia euclidea que la encontrada en el nodo principal. En caso
 *de que los nodos no contengan ningun elemento y no encuentren nada, se añadirán tambien los
 *vecinos de los vecinos, realizandolo de forma iterativa
 */
__uint32_t Image::cf_find_closest_in_neighbors(cf_find_neigh_small const * params) {
  __uint32_t closest_color                           = 0;
  bool found_closest                                 = false;
  unordered_map<__uint32_t, __uint8_t> visited_local = *params->visited_node;
  vector<__uint32_t> current_neighbors               = *params->neighbors;
  double min_distance                                = *params->min_distance;
  while (!found_closest && !current_neighbors.empty()) {
    vector<__uint32_t> next_neighbors;
    for (__uint32_t const neighbor : current_neighbors) {
      if (visited_local.contains(neighbor)) { continue; }

      visited_local[neighbor] = 0;
      auto iter               = params->graph->find(neighbor);

      for (__uint32_t const candidate : iter->second.second) {
        double const distance = get_distance(params->color_to_delete, candidate);

        if (distance < min_distance) {
          min_distance  = distance;
          closest_color = candidate;
          found_closest = true;
        }
      }
      if (!found_closest) {
        next_neighbors.insert(next_neighbors.end(), iter->second.first.begin(),
                              iter->second.first.end());
      }
    }
    current_neighbors = std::move(next_neighbors);
  }
  *params->min_distance = min_distance;
  return closest_color;
}

/**
 *Igual que la de arriba pero en version BIG, es decir, para imagenes de valor maximo de intensidad
 *de color 65535
 */
__uint64_t Image::cf_find_closest_in_neighbors_BIG(cf_find_neigh_BIG const * params) {
  __uint64_t closest_color                           = 0;
  bool found_closest                                 = false;
  unordered_map<__uint64_t, __uint8_t> visited_local = *params->visited_node;
  vector<__uint64_t> current_neighbors               = *params->neighbors;
  double min_distance                                = *params->min_distance;
  while (!found_closest && !current_neighbors.empty()) {
    vector<__uint64_t> next_neighbors;
    for (__uint64_t const neighbor : current_neighbors) {
      if (visited_local.contains(neighbor)) { continue; }

      visited_local[neighbor] = 0;
      auto const iter         = params->graph->find(neighbor);

      for (__uint64_t const candidate : iter->second.second) {
        if (double const distance = get_distance_BIG(params->color_to_delete, candidate);
            distance < min_distance) {
          min_distance  = distance;
          closest_color = candidate;
          found_closest = true;
        }
      }
      if (!found_closest) {
        next_neighbors.insert(next_neighbors.end(), iter->second.first.begin(),
                              iter->second.first.end());
      }
    }
    current_neighbors = std::move(next_neighbors);
  }
  *params->min_distance = min_distance;
  return closest_color;
}

/**
 *Funcion auxiliar del cutfreq que se encarga de finalizar el grafo, añadiendo al pair.second los
 *colores NO a eliminar pertenecientes al subconjunto de colores cercanos a cada nodo, asi como
 *añadiendo de forma temporal al value de cada elemento de Deleteitems el nodo que le perteneceria
 *en caso de NO eliminarsee, para saber por donde empezar a buscar
 */
void Image::cf_finish_graph(params_finish_graph const * params) {
  for (auto const & key : *params->myMap | views::keys) {
    double distance = MAX_DIST;
    for (auto const & key1 : *params->graph | views::keys) {
      if (double const new_distance = get_distance(key, key1); new_distance <= distance) {
        distance = new_distance;
        if (!params->Deleteitems->contains(key)) {
          (*params->toSave)[key] = key1;
        } else {
          (*params->Deleteitems)[key] = key1;
        }
      }
    }
    if (!params->Deleteitems->contains(key)) {
      (*params->graph)[(*params->toSave)[key]].second.push_back(key);
    }
  }
}

/**
 *Igual que la de arriba pero en version BIG, es decir, para imagenes de valor maximo de intensidad
 *de color 65535
 */
void Image::cf_finish_graph_BIG(params_finish_graph_BIG const * params) {
  for (auto const & key : *params->myMap | views::keys) {
    double distance = MAX_DIST;

    for (auto const & key1 : *params->graph | views::keys) {
      double const new_distance = get_distance_BIG(key, key1);
      if (new_distance <= distance) {
        distance = new_distance;
        if (!params->Deleteitems->contains(key)) {
          (*params->toSave)[key] = key1;
        } else {
          (*params->Deleteitems)[key] = key1;
        }
      }
    }
    if (!params->Deleteitems->contains(key)) {
      (*params->graph)[(*params->toSave)[key]].second.push_back(key);
    }
  }
}
