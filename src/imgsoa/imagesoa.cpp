//
// Created by beto on 4/10/24.
//

#include "imagesoa.hpp"

#include "common/mtdata.hpp"
#include "common/progargs.hpp"
#include "common/struct-rgb.hpp"

#include <algorithm>
#include <common/binario.hpp>
#include <fstream>
#include <iostream>
#include <map>
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
    if (get_metadata(this->get_input_file()) < 0) { return -1; }
  } else if (this->get_optype() == "maxlevel") {
    // Implementación de la operación de nivel máximo usando AOS (Array of Structures)
    if (maxlevel() < 0) { return -1; }
  } else if (this->get_optype() == "cutfreq") {
    if (cutfreq() < 0) { return -1; }
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
    string const rgb = mix3char(r, g, b);
    // cout << rgb << "\n";
    /* Se actualizan las repeticines del misimo codigo rgb en el diccionario
     * y si no está se añade
     * */
    if (myMap.contains(rgb)) {
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
    return a.second > b.second;
  });

  // imprimo el vector por pantalla, independientemente de nada

  vector<pair<string, int>> VectorDelete;
  auto n = static_cast<size_t>(this->get_args()[0]);

  for (size_t i = 0; i < n; i++) {
    VectorDelete.push_back(myVector[i]);
  }
  size_t tamDelete = n;
  while(myVector[tamDelete].second == VectorDelete[n-1].second) {
    VectorDelete.push_back(myVector[tamDelete]);
    tamDelete ++;

  }
  //imprimo VectorDelete
  for(auto & i : VectorDelete) {
    cout << i.first << " " << i.second << "\n";
  }
  const int pivot = VectorDelete[n-1].second;
  int elem_deleted = 0;

  for (size_t i = 0; i< VectorDelete.size(); i++) {
    if (VectorDelete[i].second > pivot){
      cout << "Te elimino jueputa " << VectorDelete[i].second  << "\n";
      elem_deleted ++;
    }
  }
  auto new_e_d= static_cast<long int>(elem_deleted);

  vector<pair<string, int>> left_elems(VectorDelete.begin() + new_e_d, VectorDelete.end());
  //imprime left_elems
  for(auto & i : left_elems) {
      cout << i.first << " " << i.second << "\n";
  }

  return 0;
}






