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



map<__uint32_t, __uint8_t> ImageSOA::load_and_map_8(int width, ifstream input_file, int height) {
  map<__uint32_t, __uint8_t> myMap;
  soa_rgb_small mysoa;
  unsigned char red = 0;
  unsigned char grn = 0;
  unsigned char blu = 0;
  for (int i = 0; i < width * height; i++) {
    red = read_binary_8(input_file);
    grn = read_binary_8(input_file);
    blu = read_binary_8(input_file);
    if (__uint32_t const rgb = packRGB(red, grn, blu); myMap.contains(rgb)) {
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

map<__uint64_t, __uint8_t> ImageSOA::load_and_map_8BIG(int width, ifstream input_file,
        int height){
    soa_rgb_big mysoa;
    unsigned short red = 0;
    unsigned short grn = 0;
    unsigned short blu = 0;
    map<__uint64_t, __uint8_t> myMap;
    for(int i=0; i< width * height; i++) {
      red = read_binary_16(input_file);
      grn = read_binary_16(input_file);
      blu = read_binary_16(input_file);
      if (__uint64_t const rgb = packRGBBIG(red, grn, blu); myMap.contains(rgb)) {
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

deque<pair<string, __uint8_t>> ImageSOA::same_bgr_vector(deque<pair<string, __uint8_t>>
                                          father_vector, const int value, const size_t counter) {
    //Value será 1 para blue, 2 para green y 3 para red
    deque<pair<string, __uint8_t>> color_vector;
    __uint8_t color = 0;
    for (size_t i = 0; i < counter; i++) {
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


int ImageSOA::check_and_delete(deque<pair<string, __uint8_t>> &color_vector, deque<pair<string,
                      __uint8_t>> left_elems, int color, deque<pair<string, string>> &Deleteitems) {
  //1 para azul, 0 para verde
  size_t meanwhile = 1;
  while (color_vector[meanwhile].second == color_vector[meanwhile + 1].second) { meanwhile++; }
  if (meanwhile == 1) {
    __uint8_t value0 = 0;
    __uint8_t value1 = 0;
    if(color ==1) {
      value0 = extractgreen(left_elems[0].first);
      value1 = extractgreen(left_elems[1].first);
    }
    else {
      value0 = extractred(left_elems[0].first);
      value1 = extractred(left_elems[1].first);
    }
    if (value0 - value1> 0) {
      Deleteitems.emplace_back(color_vector[0].first, "");
      color_vector.pop_front();
    } else {
      Deleteitems.emplace_back(color_vector[1].first, "");
      swap(color_vector[0], color_vector[1]);
      color_vector.pop_front();
    }
    return 0;
  }
  return static_cast<int>(meanwhile);
}

void ImageSOA::delete_from_deque(deque<pair<string, __uint8_t>> &deque_general, size_t index) {
  swap(deque_general[0], deque_general[index]);
  deque_general.pop_front();
}

size_t ImageSOA::search_in_blue(deque<pair<string, unsigned char>> & pairs, string & first) {
  for (size_t i = 0; i < pairs.size(); i++) {
    if (pairs[i].first == first) { return i; }
  }
  return 0;
}

deque<pair<string, string>>
    ImageSOA::check_colors_to_delete(deque<pair<string, string>> Deleteitems, int num_left,
                                     deque<pair<string, unsigned char>> const & left_elems,
                                     deque<pair<string, __uint8_t>> bluevalues) {
  size_t my_index = 0;
  while (num_left > 0) {
    if (bluevalues[0].second == bluevalues[1].second) {
      if (int my_meanwhile = check_and_delete(bluevalues, left_elems, 1, Deleteitems);
          my_meanwhile > 0) {
        auto greenvalues = same_bgr_vector(left_elems, 2, static_cast<size_t>(my_meanwhile));
        if (greenvalues[0].second == greenvalues[1].second) {
          my_meanwhile = check_and_delete(greenvalues, left_elems, 0, Deleteitems);
          if (my_meanwhile > 0) {
            auto redvalues = same_bgr_vector(left_elems, 3, static_cast<size_t>(my_meanwhile));
            Deleteitems.emplace_back(redvalues[0].first, "");
            my_index = search_in_blue(bluevalues, redvalues[0].first);
            delete_from_deque(bluevalues, my_index);
            num_left--;
          } else {
            num_left--;
          }
        } else {
          Deleteitems.emplace_back(greenvalues[0].first, "");
          my_index = search_in_blue(bluevalues, greenvalues[0].first);
          delete_from_deque(bluevalues, my_index);
          num_left--;
        }
      } else {
        num_left--;
      }
    } else {
      Deleteitems.emplace_back(bluevalues[0].first, "");
      bluevalues.pop_front();
      num_left--;
    }
  }
  return Deleteitems;
}

void ImageSOA::cutfreq_min(map<__uint32_t, __uint8_t> myMap) {
  // Convierto myMap a vector de pares y ordeno
  vector<pair<__uint32_t, __uint8_t>> myVector(myMap.begin(), myMap.end());
  ranges::sort(myVector, [](auto const & op1, auto const & op2) {
    return op1.second < op2.second;
  });

  // Me paso a size_t el numero de elementos a eliminar y me creo un vector delete
  vector<pair<string, __uint8_t>> VectorDelete;
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
  deque<pair<string, string>> Deleteitems;
  for (auto & [fst, snd] : VectorDelete) {
    if (snd < pivot) {
      Deleteitems.emplace_back(fst, "");
      elem_deleted++;
    }
  }

  int const new_n    = static_cast<int>(elems_to_delete);
  int const num_left = new_n - elem_deleted;
  auto const new_e_d = static_cast<long int>(elem_deleted);  // elem_deleted

  deque const left_elems(VectorDelete.begin() + new_e_d, VectorDelete.end());

  auto bluevalues = same_bgr_vector(left_elems, 1, left_elems.size());
  // Para saber que elemento de bluevalues utilizar
  Deleteitems = check_colors_to_delete(Deleteitems, num_left, left_elems, bluevalues);

    /*
     * Si tenemos los colores c1=(r1,g1,b1) y c2=(r2,g2,b2), la distancia euclídea entre ambos colores
     * no depende de su posición en la imagen sino de sus valores RGB.
     * d(c1,c2) = sqrt((r1-r2)² + (g1-g2)² + (b1-b2)²)
     */}
void ImageSOA::cutfreq_max(map<__uint64_t, __uint8_t> myMapBIG) {
  cout << "Pinga";
}

int ImageSOA::cutfreq()  {

  get_imgdata();
  ifstream input_file(this->get_input_file(), ios::binary);
  ofstream output_file(this->get_output_file(), ios::binary);

  if (!input_file || !output_file) {
    cerr << "Error al abrir los archivos de entrada/salida"
         << "\n";
    return -1;
  }

  write_out(get_maxval());


  int const width  = this->get_width();
  int const height = this->get_height();
  int const maxval = this->get_maxval();
  //ofstream output_file(this->get_output_file(), ios::binary);
  map<__uint32_t, __uint8_t> myMap;
  map<__uint64_t, __uint8_t> myMapBIG;
  if (maxval == MIN_LEVEL) {
    myMap = load_and_map_8(width, move(input_file), height);
    cutfreq_min(myMap);
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

  get_imgdata();
  ifstream input_file(this->get_input_file(), ios::binary);
  ofstream output_file(this->get_output_file(), ios::binary);

  if (!input_file || !output_file) {
    cerr << "Error al abrir los archivos de entrada/salida"
         << "\n";
    return -1;
  }

  int const width  = this->get_width();
  int const height = this->get_height();
  int const maxval = this->get_maxval();
  auto const width_height = static_cast<unsigned int>(width * height);
  if (maxval <= MIN_LEVEL) {
    /*
     * Usaremos un árbol AVL como si fuera un catálogo de colores para almacenar los colores
     * DISTINTOS de la imagen. Éstos también se almacenan un struct_soa, para recorrelo
     * posteriormente. Esta implementación hará que la complejidad de esta operación sea
     * O(n log(n)), donde n es el número de píxeles de la imagen.
     */
    AVLTree tree;

    soa_rgb_small image;
    for (unsigned int i = 0; i < width_height; i++) {
      __uint8_t red = 0;
      __uint8_t grn = 0;
      __uint8_t blu = 0;
      red = read_binary_8(input_file);
      grn = read_binary_8(input_file);
      blu = read_binary_8(input_file);
      if (i == 0) {  // Si es el primer elemento
        unsigned int const concatenated = red << 2 * BYTE |
                                          grn << BYTE |
                                          blu;
        element const elem = {.color = concatenated, .index = 0};
        tree.insert(elem);
        image.r.push_back(red);
        image.g.push_back(grn);
        image.b.push_back(blu);
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
      /*
      output_file.write(&image.r[i], sizeof(image.r[i]));
      output_file.write(&image.g[i], sizeof(image.g[i]));
      output_file.write(&image.b[i], sizeof(image.b[i]));
      */
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
    /*
    ifstream input_file(this->get_input_file(), ios::binary);
    input_file >> format >> width >> height >> maxval;
    input_file.ignore(1);
    */
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
}


