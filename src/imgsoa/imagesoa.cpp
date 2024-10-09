//
// Created by beto on 4/10/24.
//

#include "imagesoa.hpp"
#include<fstream>
#include "common/mtdata.hpp"
#include "common/binario.hpp"




using namespace std

ImageSOA::ImageSOA(string input_file, string output_file, string optype, vector<int> args) {
    this->input_file = input_file;
    this->output_file = output_file;
    this->optype = optype;
    this->args = args;
}

ImageSOA::~ImageSOA() {
    // Destructor de la clase, por si reservamos memoria
}

int ImageSOA::process_operation() {
    // Primera operación: leer los metadatos de la imagen de entrada. Como
    // esta función es común a AOS y SOA, será implementada en la biblioteque "common"
    if (this->optype == "info") {
        if (get_metadata(this->input_file) < 0) {
            return -1;
        }
    }
    else if (this->optype == "maxlevel") {
        // Implementación de la operación de nivel máximo usando AOS (Array of Structures)
        if (maxlevel() < 0) {
            return -1;
        }
    }
    else {
        cerr << "Operación no soportada: " << optype << endl;
        return -1;
    }
    return 0;
}


int ImageSOA::maxlevel() {
    ifstream input_file(this->input_file, ios::binary);
    ofstream output_file(this->output_file, ios::binary);

    if (!input_file || !output_file) {
        cerr << "Error al abrir los archivos de entrada/salida" << endl;
        return -1;
    }

    string format;
    int width, height, maxval;
    input_file >> format >> width >> height >> maxval;
    input_file.ignore(1);

    if (this->args[0] < 256) {
        output_file << format << " " << width << " " << height << " " << 255 << endl;
        if(maxval<256) {
            unsigned char thisiteration;

        }
    }
}



