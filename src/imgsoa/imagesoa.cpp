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
#include <map>
#include <string>
#include <sys/stat.h>
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
    if (info() < 0) { return -1; }
  } else if (this->get_optype() == "maxlevel") {
    // Implementación de la operación de nivel máximo usando AOS (Array of Structures)
    if (maxlevel() < 0) { return -1; }
  } else if (this->get_optype() == "cutfreq") {
    if (cutfreq() < 0) { return -1; }
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

int ImageSOA::resize(){

  obtain_args();
  ifstream input_file(this->get_input_file(), ios::binary);
  ofstream output_file(this->get_output_file(), ios::binary);

  if (!input_file || !output_file) {
    cerr << "Error al abrir los archivos de entrada/salida"
         << "\n";
    return -1;
  }

  string format;
  unsigned int width = 0;
  unsigned int height = 0;
  unsigned int maxval = 0;
  input_file >> format >> width >> height >> maxval;
  input_file.ignore(1);

  int const new_width  = this->get_args()[0];
  int const new_height = this->get_args()[1];

  output_file << format << " " << new_width << " " << new_height << " " << maxval << "\n";

  if (maxval <= MIN_LEVEL) {
    // leemos la imagen y la almacenamos en memoria
    soa_rgb_small image;
    for (unsigned int i = 0; i < width * height; i++) {
      char red = 0;
      char grn = 0;
      char blu = 0;
      input_file.read(&red, sizeof(red));
      image.r.push_back(red);
      input_file.read(&grn, sizeof(grn));
      image.g.push_back(grn);
      input_file.read(&blu, sizeof(blu));
      image.b.push_back(blu);
    }
    for (int y_prime = 0; y_prime < new_height; y_prime++) {
      for (int x_prime = 0; x_prime < new_width; x_prime++) {
        // Por como funcionan las operaciones de coma flotante, hay que redondear el valor
        // según una cierta precisión. Si no, la imagen final generará píxeles corruptos.
        double const equis = round(x_prime * (static_cast<double>(width) / new_width));
        double const ygreek = round(y_prime * (static_cast<double>(height) / new_height));

        auto xlength = static_cast<unsigned int>(floor(equis));
        auto xheight = static_cast<unsigned int>(ceil(equis));
        auto ylength = static_cast<unsigned int>(floor(ygreek));
        auto yheight = static_cast<unsigned int>(ceil(ygreek));

        xheight = min(xheight, width - 1);
        xlength = min(xlength, width - 1);
        yheight = min(yheight, height - 1);
        ylength = min(ylength, height - 1);

        // Obtenemos los 4 pixeles más cercanos
        rgb_small const p_1 = {.r = image.r[(static_cast<unsigned long>(ylength) * width) + xlength],
                              .g = image.g[(static_cast<unsigned long>(ylength) * width) + xlength],
                              .b = image.b[(static_cast<unsigned long>(ylength) * width) + xlength]};

        rgb_small const p_2 = {.r = image.r[(static_cast<unsigned long>(ylength) * width) + xheight],
                              .g = image.g[(static_cast<unsigned long>(ylength) * width) + xheight],
                              .b = image.b[(static_cast<unsigned long>(ylength) * width) + xheight]};

        rgb_small const p_3 = {.r = image.r[(static_cast<unsigned long>(yheight) * width) + xlength],
                              .g = image.g[(static_cast<unsigned long>(yheight) * width) + xlength],
                              .b = image.b[(static_cast<unsigned long>(yheight) * width) + xlength]};

        rgb_small const p_4 = {.r = image.r[(static_cast<unsigned long>(yheight) * width) + xheight],
                              .g = image.g[(static_cast<unsigned long>(yheight) * width) + xheight],
                              .b = image.b[(static_cast<unsigned long>(yheight) * width) + xheight]};

        // Interpolación en el eje x
        double const t_param     = equis - xlength;
        rgb_small const c_1 = {.r = static_cast<char>(((1 - t_param) * p_1.r) + (t_param * p_2.r)),
                              .g = static_cast<char>(((1 - t_param) * p_1.g) + (t_param * p_2.g)),
                              .b = static_cast<char>(((1 - t_param) * p_1.b) + (t_param * p_2.b))};

        rgb_small const c_2 = {.r = static_cast<char>(((1 - t_param) * p_3.r) + (t_param * p_4.r)),
                              .g = static_cast<char>(((1 - t_param) * p_3.g) + (t_param * p_4.g)),
                              .b = static_cast<char>(((1 - t_param) * p_3.b) + (t_param * p_4.b))};

        // Interpolación en el eje y
        double const u_param    = ygreek - ylength;
        rgb_small const c_param = {.r = static_cast<char>(((1 - u_param) * c_1.r) + (u_param * c_2.r)),
                             .g = static_cast<char>(((1 - u_param) * c_1.g) + (u_param * c_2.g)),
                             .b = static_cast<char>(((1 - u_param) * c_1.b) + (u_param * c_2.b))};

        // Escribir el pixel interpolado
        output_file.write(&c_param.r, sizeof(c_param.r));
        output_file.write(&c_param.g, sizeof(c_param.g));
        output_file.write(&c_param.b, sizeof(c_param.b));
      }
    }
  } else if (maxval <= MAX_LEVEL) {
    soa_rgb_big image;
    for (unsigned int i = 0; i < width * height; i++) {
      uint16_t red = 0;
      uint16_t grn = 0;
      uint16_t blu = 0;
      red = read_binary_16(input_file);
      grn = read_binary_16(input_file);
      blu = read_binary_16(input_file);

      image.r.push_back(red);
      image.g.push_back(grn);
      image.b.push_back(blu);
    }
    for (int y_prime = 0; y_prime < new_height; y_prime++) {
      for (int x_prime = 0; x_prime < new_width; x_prime++) {
        double const equis = round(x_prime * (static_cast<double>(width) / new_width));
        double const ygreek = round(y_prime * (static_cast<double>(height) / new_height));

        auto xlength = static_cast<unsigned int>(floor(equis));
        auto xheight = static_cast<unsigned int>(ceil(equis));
        auto ylength = static_cast<unsigned int>(floor(ygreek));
        auto yheight = static_cast<unsigned int>(ceil(ygreek));

        xheight = min(xheight, width - 1);
        xlength = min(xlength, width - 1);
        yheight = min(yheight, height - 1);
        ylength = min(ylength, height - 1);

        // Obtenemos los 4 pixeles más cercanos
        rgb_big const p_1 = {.r = image.r[(static_cast<unsigned long>(ylength) * width) + xlength],
                            .g = image.g[(static_cast<unsigned long>(ylength) * width) + xlength],
                            .b = image.b[(static_cast<unsigned long>(ylength) * width) + xlength]};

        rgb_big const p_2 = {.r = image.r[(static_cast<unsigned long>(ylength) * width) + xheight],
                            .g = image.g[(static_cast<unsigned long>(ylength) * width) + xheight],
                            .b = image.b[(static_cast<unsigned long>(ylength) * width) + xheight]};

        rgb_big const p_3 = {.r = image.r[(static_cast<unsigned long>(yheight) * width) + xlength],
                            .g = image.g[(static_cast<unsigned long>(yheight) * width) + xlength],
                            .b = image.b[(static_cast<unsigned long>(yheight) * width) + xlength]};

        rgb_big const p_4 = {.r = image.r[(static_cast<unsigned long>(yheight) * width) + xheight],
                            .g = image.g[(static_cast<unsigned long>(yheight) * width) + xheight],
                            .b = image.b[(static_cast<unsigned long>(yheight) * width) + xheight]};

        // Interpolación en el eje x
        double const t_param   = equis - xlength;
        rgb_big const c_1 = {.r = static_cast<uint16_t>(((1 - t_param) * p_1.r) + (t_param * p_2.r)),
                            .g = static_cast<uint16_t>(((1 - t_param) * p_1.g) + (t_param * p_2.g)),
                            .b = static_cast<uint16_t>(((1 - t_param) * p_1.b) + (t_param * p_2.b))};

        rgb_big const c_2 = {.r = static_cast<uint16_t>(((1 - t_param) * p_3.r) + (t_param * p_4.r)),
                            .g = static_cast<uint16_t>(((1 - t_param) * p_3.g) + (t_param * p_4.g)),
                            .b = static_cast<uint16_t>(((1 - t_param) * p_3.b) + (t_param * p_4.b))};

        double const u_param  = ygreek - ylength;
        rgb_big const c_param = {.r = static_cast<uint16_t>(((1 - u_param) * c_1.r) + (u_param * c_2.r)),
                           .g = static_cast<uint16_t>(((1 - u_param) * c_1.g) + (u_param * c_2.g)),
                           .b = static_cast<uint16_t>(((1 - u_param) * c_1.b) + (u_param * c_2.b))};

        write_binary_16(output_file, c_param.r);
        write_binary_16(output_file, c_param.g);
        write_binary_16(output_file, c_param.b);
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

bool ImageSOA::obtain_args() {
  ifstream input_file(this->get_input_file(), ios::binary);
  if (!input_file) { return false; }

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
  return true;
}

map<string, int> ImageSOA::load_and_map_8(){
  soa_rgb_small mysoa;
  map <string, int> myMap;
  char red = 0;
  char grn = 0;
  char blu = 0;
  for(int i=0; i< this->width * this->height; i++){
    this->if_input_file.read(&red, sizeof(red));
    this->if_input_file.read(&grn, sizeof(grn));
    this->if_input_file.read(&blu, sizeof(blu));

    if (string const rgb = mix3char(red, grn, blu); myMap.contains(rgb)) {
      myMap[{rgb}]++;
    } else {
      myMap[{rgb}] = 1;
    }
    mysoa.r.push_back(red);
    mysoa.g.push_back(grn);
    mysoa.b.push_back(blu);
  }
  return myMap;
}


vector<pair<string, __uint8_t>> same_bgr_vector(vector<pair<string, int>> father_vector, int value, int counter) {
    //Value será 1 para blue, 2 para green y 3 para red
    vector<pair<string, __uint8_t>> color_vector;
    __uint8_t color = 0;
    for (size_t i = 0; i <= counter; i++) {
        if(value ==1){color = extractblue(father_vector[i].first);}
        if(value ==2){color = extractgreen(father_vector[i].first);}
        if(value ==3){color = extractred(father_vector[i].first);}
      color_vector.emplace_back(father_vector[i].first, color);
    }
    ranges::sort(color_vector, [](auto const & op1, auto const & op2) {
      return get<1>(op1) > get<1>(op2);
    });
  return color_vector;
}






int ImageSOA::cutfreq(){

  if (not obtain_args()) {
    cerr << "Error al abrir los archivos de entrada"
         << "\n";
    return -1;
  }
  //ofstream output_file(this->get_output_file(), ios::binary);

  //Obtengo el diccionario con los valores rgb unicos
  map<string, int> myMap = load_and_map_8();
  this->if_input_file.close();

  vector<pair<string, int>> myVector(myMap.begin(), myMap.end());

  ranges::sort(myVector, [](auto const & op1, auto const & op2) {
    return op1.second < op2.second;
  });


  vector<pair<string, int>> VectorDelete;
  auto elems_to_delete = static_cast<size_t>(this->get_args()[0]);

  for (size_t i = 0; i < elems_to_delete; i++) { VectorDelete.push_back(myVector[i]); }
  size_t tamDelete = elems_to_delete;

  while (myVector[tamDelete].second == VectorDelete[elems_to_delete - 1].second) {
    VectorDelete.push_back(myVector[tamDelete]);
    tamDelete++;
  }

  int const pivot  = VectorDelete[elems_to_delete - 1].second;
  int elem_deleted = 0;
  vector<pair<string, string>> Deleteitems;
  for (auto & iterator : VectorDelete) {
    if (iterator.second < pivot) {
      Deleteitems.emplace_back(iterator.first, "");
      elem_deleted++;
    }
  }
  int const new_n = static_cast<int>(elems_to_delete);
  int num_left    = new_n - elem_deleted;
  auto new_e_d    = static_cast<long int>(elem_deleted);  //elem_deleted

  vector left_elems(VectorDelete.begin() + new_e_d, VectorDelete.end());

  /*
  vector<pair<string, __uint8_t>> bluevalues;
  __uint8_t blue = 0;
  for (auto & iterator : left_elems) {
    blue = extractblue(iterator.first);
    bluevalues.emplace_back(iterator.first, blue);
  }
  ranges::sort(bluevalues, [](auto const & op1, auto const & op2) {
    return get<1>(op1) > get<1>(op2);
  });
  */
  int const size = static_cast<int>(left_elems.size());
  auto bluevalues = same_bgr_vector(left_elems, 1, size);

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
        /*
        __uint8_t green = 0;
        vector<tuple<string, __uint8_t, __uint8_t>> greenvalues;
        for (size_t i = 0; i <= meanwhile; i++) {
          green = extractgreen(bluevalues[i].first);
          greenvalues.emplace_back(bluevalues[i].first, bluevalues[i].second, green);
        }

        ranges::sort(greenvalues, [](auto const & a, auto const & b) {
          return get<2>(a) > get<2>(b);
        });
        */
        auto const int_meanwhile = static_cast<int>(meanwhile);
        auto greenvalues = same_bgr_vector(left_elems, 2, int_meanwhile);

        if (greenvalues[0].second == greenvalues[1].second) {
          size_t meanwhile = 1;
          while (greenvalues[meanwhile].second == greenvalues[meanwhile + 1].second) {
            meanwhile++;
          }
          if (meanwhile == 1) {
            __uint8_t const red1 = extractred(get<0>(greenvalues[1]));
            __uint8_t const red0 = extractred(get<0>(greenvalues[0]));
            if (red1 - red0 > 0) {
              Deleteitems.emplace_back(get<0>(greenvalues[1]), "");
              greenvalues.erase(greenvalues.begin() + 1);

            } else {
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

int ImageSOA::compress() const {
  ifstream input_file(this->get_input_file(), ios::binary);
  ofstream output_file(this->get_output_file(), ios::binary);

  if (!input_file || !output_file) {
    cerr << "Error al abrir los archivos de entrada/salida"
         << "\n";
    return -1;
  }

  string format;
  unsigned int width = 0;
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
      char red = 0;
      char grn = 0;
      char blu = 0;
      input_file.read(&red, sizeof(red));
      input_file.read(&grn, sizeof(grn));
      input_file.read(&blu, sizeof(blu));

      if (i == 0) {  // Si es el primer elemento
        unsigned int const concatenated = static_cast<unsigned char>(red) << 2 * BYTE |
                                          static_cast<unsigned char>(grn) << BYTE |
                                          static_cast<unsigned char>(blu);
        element const elem = {.color = concatenated, .index = 0};
        tree.insert(elem);
        image.r.push_back(red);
        image.g.push_back(grn);
        image.b.push_back(blu);
      } else {  // Si no es el primer elemento
        // Comprobamos si el color ya está en el árbol
        unsigned int const concatenated = static_cast<unsigned char>(red) << 2 * BYTE |
                                          static_cast<unsigned char>(grn) << BYTE |
                                          static_cast<unsigned char>(blu);
        element const elem = {.color = concatenated, .index = i};
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
      output_file.write(&image.r[i], sizeof(image.r[i]));
      output_file.write(&image.g[i], sizeof(image.g[i]));
      output_file.write(&image.b[i], sizeof(image.b[i]));
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
        char red = 0;
        char grn = 0;
        char blu = 0;
        input_file.read(&red, sizeof(red));
        input_file.read(&grn, sizeof(grn));
        input_file.read(&blu, sizeof(blu));

        unsigned int const concatenated = static_cast<unsigned char>(red) << 2 * BYTE |
                                          static_cast<unsigned char>(grn) << BYTE |
                                          static_cast<unsigned char>(blu);
        element const elem = tree.search(concatenated);
        write_binary_8(output_file, static_cast<unsigned char>(elem.index));
      }
    } else if (num_colors < static_cast<unsigned long int>(pow(2, 2 * BYTE))) {
      for (unsigned int i = 0; i < width * height; i++) {
        char red = 0;
        char grn = 0;
        char blu = 0;
        input_file.read(&red, sizeof(red));
        input_file.read(&grn, sizeof(grn));
        input_file.read(&blu, sizeof(blu));

        unsigned int const concatenated = static_cast<unsigned char>(red) << 2 * BYTE |
                                          static_cast<unsigned char>(grn) << BYTE |
                                          static_cast<unsigned char>(blu);
        element const elem = tree.search(concatenated);
        write_binary_16(output_file, static_cast<uint16_t>(elem.index));
      }
    } else if (num_colors < static_cast<unsigned long int>(pow(2, 4 * BYTE))) {
      for (unsigned int i = 0; i < width * height; i++) {
        char red = 0;
        char grn = 0;
        char blu = 0;
        input_file.read(&red, sizeof(red));
        input_file.read(&grn, sizeof(grn));
        input_file.read(&blu, sizeof(blu));

        unsigned int const concatenated = static_cast<unsigned char>(red) << 2 * BYTE |
                                          static_cast<unsigned char>(grn) << BYTE |
                                          static_cast<unsigned char>(blu);
        element const elem = tree.search(concatenated);
        write_binary_32(output_file, elem.index);
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



