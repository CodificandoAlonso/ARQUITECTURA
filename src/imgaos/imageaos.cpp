//
// Created by beto on 4/10/24.
//

#include <iostream>
#include <fstream>
#include "imageaos.hpp"
#include "common/progargs.hpp"
#include "common/mtdata.hpp"

using namespace std;

ImageAOS::ImageAOS(int argc, const vector<string> &argv) : Image(argc, argv) {
    // Constructor de la clase
}

int ImageAOS::process_operation() {

    // Primera operación: leer los metadatos de la imagen de entrada. Como
    // esta función es común a AOS y SOA, será implementada en la biblioteque "common"
    if (this->get_optype() == "info") {
        if (get_metadata(this->get_input_file()) < 0) {
            return -1;
        }
    }
    else if (this->get_optype() == "maxlevel") {
        // Implementación de la operación de nivel máximo usando AOS (Array of Structures)
        if (maxlevel() < 0) {
            return -1;
        }
    }
    else {
        cerr << "Operación no soportada de momento: " << this->get_optype() << '\n';
        return -1;
    }
    return 0;
}

int ImageAOS::maxlevel() {
    // Implementación de la operación de escalado de intensidad usando AOS (Array of Structures)
    // Se debe leer la imagen de entrada, aplicar la operación y guardar la imagen de salida

    ifstream input_file(this->get_input_file(), ios::binary);
    ofstream output_file(this->get_output_file(), ios::binary);

    if (!input_file || !output_file) {
        cerr << "Error al abrir los archivos de entrada/salida" << '\n';
        return -1;
    }

    string format;
    int width = 0;
    int height = 0;
    int maxval = 0;
    input_file >> format >> width >> height >> maxval;
    input_file.ignore(1);
    /*
    if (this->args[0] < 256) {
        output_file << format << " " << width << " " << height << " " << 255 << "\n";

        if (maxval < 256) {
            vector<rgb_small> our_aos ;
        }
        else {
            cerr << "Error: nivel de intensidad no soportado" << "\n";
            return -1;
        }
    */
        input_file.close();
        output_file.close();

        cout << "Operación de escalado de intensidad completada." << "\n";

        return 0;
    //}
}
