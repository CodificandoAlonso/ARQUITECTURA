//
// Created by beto on 4/10/24.
//

#include "imageaos.hpp"

#include "common/mtdata.hpp"

#include <fstream>
#include <iostream>

using namespace std;

ImageAOS::ImageAOS(int argc, vector<string> const & argv) : Image(argc, argv) {
  // Constructor de la clase
}

int ImageAOS::process_operation() {
  // Primera operación: leer los metadatos de la imagen de entrada. Como
  // esta función es común a AOS y SOA, será implementada en la biblioteque "common"
  if (this->get_optype() == "info") {
    if (get_metadata(this->get_input_file()) < 0) { return -1; }
  } else if (this->get_optype() == "maxlevel") {
    // Implementación de la operación de nivel máximo usando AOS (Array of Structures)
    if (maxlevel() < 0) { return -1; }
  } else {
    cerr << "Operación no soportada de momento: " << this->get_optype() << '\n';
    return -1;
  }
  return 0;
}

