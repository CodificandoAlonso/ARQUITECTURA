//
// Created by beto on 4/10/24.
//

#include <iostream>
#include <fstream>
#include <cstdint>
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
    else if (this->optype == "maxlevel") {
        // Implementación de la operación de nivel máximo usando AOS (Array of Structures)
        if (escalate_intensity() < 0) {
            return -1;
        }
    }
    else {
        cerr << "Operación no soportada: " << optype << endl;
        return -1;
    }

    return 0;
}

int ImageAOS::escalate_intensity() {
    // Implementación de la operación de escalado de intensidad usando AOS (Array of Structures)
    // Se debe leer la imagen de entrada, aplicar la operación y guardar la imagen de salida

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
        // Se ha pedido que el formato de salida sea con un maxvalue de 255,
        //queda por determinar si el maxvalue de la imagen de entrada es mayor o igual a 256
        output_file << format << " " << width << " " << height << " " << maxval << endl;

        if (maxval < 256) {
            // Utilizamos unsigned char, ya que ocupan 1 byte, que es lo que precisamos para
            // almacenar cada uno de los canales de color de la imagen (si el valor máximo es 255).
            unsigned char r, g, b;
            for (int i = 0; i < width * height; i++) {
                input_file.read((char *) &r, sizeof(unsigned char));
                input_file.read((char *) &g, sizeof(unsigned char));
                input_file.read((char*)&b, sizeof(unsigned char));

                r = static_cast<unsigned char>((r * this->args[0]) / maxval);
                g = static_cast<unsigned char>((g * this->args[0]) / maxval);
                b = static_cast<unsigned char>((b * this->args[0]) / maxval);

                output_file.write((char*)&r, sizeof(unsigned char));
                output_file.write((char*)&g, sizeof(unsigned char));
                output_file.write((char*)&b, sizeof(unsigned char));
            }
        }
        else if (maxval < 65536) {
            /*
             * La imagen de entrada tiene un maxval de 65535, por lo que se deben utilizar
             * unsigned short para almacenar los canales de color de la imagen, ya que tienen 2 bytes.
             * hay que tener en cuenta que estos dos bytes están almacenados en little-endian.
             */
            unsigned short r, g, b;
            unsigned char r1, r2, g1, g2, b1, b2;
            for (int i = 0; i < width * height; i++) {
                input_file.read((char*)&r1, sizeof(unsigned char));
                input_file.read((char*)&r2, sizeof(unsigned char));
                r = (r2 << 8) | r1;

                input_file.read((char*)&g1, sizeof(unsigned char));
                input_file.read((char*)&g2, sizeof(unsigned char));
                g = (g2 << 8) | g1;

                input_file.read((char*)&b1, sizeof(unsigned char));
                input_file.read((char*)&b2, sizeof(unsigned char));
                b = (b2 << 8) | b1;

                r = static_cast<unsigned short>((r * this->args[0]) / maxval);
                g = static_cast<unsigned short>((g * this->args[0]) / maxval);
                b = static_cast<unsigned short>((b * this->args[0]) / maxval);

                output_file.write((char*)&r, sizeof(unsigned short));
                output_file.write((char*)&g, sizeof(unsigned short));
                output_file.write((char*)&b, sizeof(unsigned short));
            }
        }
    }
    else if (this->args[0] < 65536) {
        output_file << format << " " << width << " " << height << " " << maxval << endl;
    }
    else {
        cerr << "Error: nivel de intensidad no soportado" << endl;
        return -1;
    }

    input_file.close();
    output_file.close();

    cout << "Operación de escalado de intensidad completada" << endl;

    return 0;
}




