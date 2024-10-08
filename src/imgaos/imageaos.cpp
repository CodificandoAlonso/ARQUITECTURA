//
// Created by beto on 4/10/24.
//

#include <iostream>
#include "imageaos.hpp"
#include "common/mtdata.hpp"

using namespace std;

ImageAOS::ImageAOS(std::string input_file, std::string output_file, std::string optype, std::vector<int> args) {
    this->input_file = input_file;
    this->output_file = output_file;
    this->optype = optype;
    this->args = args;
}

ImageAOS::~ImageAOS() {
    // Destructor de la clase, por si reservamos memoria
}

int ImageAOS::process_operation() {

    // Primera operación: leer los metadatos de la imagen de entrada. Como
    // esta función es común a AOS y SOA, será implementada en la biblioteque "common"
    if (this->optype == "info") {
        if (get_metadata(this->input_file) < 0) {
            return -1;
        }
    }

    return 0;
}




