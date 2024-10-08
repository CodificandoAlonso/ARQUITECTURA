//
// Created by beto on 4/10/24.
//

#include <iostream>
#include <fstream>
#include <cstdint>
#include "imageaos.hpp"
#include "common/mtdata.hpp"
#include "common/binario.hpp"

#include <bitset>

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

int ImageAOS::maxlevel() {
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
        output_file << format << " " << width << " " << height << " " << maxval << endl;

        if (maxval < 256) {
            /*
             * Si se desea escalar una imagen cuyo máximo nivel de intensidad es 255 a
             * otra con un nivel de intensidad menor a 256, leemos la imagen de entrada
             * de 8 bits en 8 bits y escribimos en la imagen de salida de 8 bits en 8 bits.
             */
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
             * Si se desea escalar una imagen cuyo máximo nivel de intensidad es mayor a
             * 255 a otra con un nivel de intensidad entre 0 y 255, leemos la imagen de
             * entrada de 8 bits en 8 bits (teniendo en cuenta que cada color ocupa 2 bytes)
             * y escribimos en la imagen de salida de 16 bits
             */
            unsigned char r1, r2, g1, g2, b1, b2;
            unsigned short r, g, b;
            for (int i = 0; i < width * height; i++) {
                input_file.read((char*)&r1, sizeof(unsigned char));
                input_file.read((char*)&r2, sizeof(unsigned char));
                input_file.read((char*)&g1, sizeof(unsigned char));
                input_file.read((char*)&g2, sizeof(unsigned char));
                input_file.read((char*)&b1, sizeof(unsigned char));
                input_file.read((char*)&b2, sizeof(unsigned char));

                r = merge16(r2, r1);
                g = merge16(g2, g1);
                b = merge16(b2, b1);

                r = static_cast<unsigned short>((r * this->args[0]) / maxval);
                g = static_cast<unsigned short>((g * this->args[0]) / maxval);
                b = static_cast<unsigned short>((b * this->args[0]) / maxval);

                output_file.write((char*)&r, sizeof(unsigned short));
                output_file.write((char*)&g, sizeof(unsigned short));
                output_file.write((char*)&b, sizeof(unsigned short));
            }
        }
        else {
            cerr << "Formato incorrecto" << endl;
            return -1;
        }
    }
    else if (this->args[0] < 65536) {
        output_file << format << " " << width << " " << height << " " << 65535 << endl;
        
        unsigned short r, g, b;
        if (maxval > 255) {
            /*
             * Si se desea escalar una imagen cuyo máximo nivel de intensidad es mayor a
             * 255 a otra con un nivel de intensidad entre 0 y 65535, leemos la imagen de
             * entrada de 16 bits en 16 bits y escribimos en la imagen de salida de 16 bits
             * en 16 bits.
             */
            unsigned char r1, r2, g1, g2, b1, b2;
            for (int i = 0; i < width * height; i++) {
                input_file.read((char *) &r1, sizeof(unsigned char));
                input_file.read((char *) &r2, sizeof(unsigned char));
                input_file.read((char *) &g1, sizeof(unsigned char));
                input_file.read((char *) &g2, sizeof(unsigned char));
                input_file.read((char *) &b1, sizeof(unsigned char));
                input_file.read((char *) &b2, sizeof(unsigned char));

                if (i<10) {
                    cout << "r1: " << bitset<8>(r1) << " r2: " << bitset<8>(r2) << endl;
                }

                r = merge16(r2, r1);
                g = merge16(g2, g1);
                b = merge16(b2, b1);

                if (i<10) {
                    cout << "r merged: " << bitset<16>(r) << endl;
                }

                r = static_cast<unsigned short>((r * this->args[0]) / maxval);
                g = static_cast<unsigned short>((g * this->args[0]) / maxval);
                b = static_cast<unsigned short>((b * this->args[0]) / maxval);

                if (i<10) {
                    cout << "r calc: " << bitset<16>(r) << endl;
                }

                r = swap16(r);
                g = swap16(g);
                b = swap16(b);

                if (i<10) {
                    cout << "r swapped: " << bitset<16>(r) << endl << endl;
                }

                output_file.write((char *)&r, sizeof(unsigned short));
                output_file.write((char *)&g, sizeof(unsigned short));
                output_file.write((char *)&b, sizeof(unsigned short));
            }
        }
        else if (maxval < 256) {
            /*
             * Si se desea escalar una imagen cuyo máximo nivel de intensidad es mayor a 255 a
             * otra con un nivel de intensidad entre 0 y 65535, leemos la imagen de entrada
             * de 8 bits en 8 bits y escribimos en la imagen de salida de 16 bits en 16 bits.
             */
            unsigned char r1, b1, g1;
            for (int i = 0; i < width * height; i++) {
                input_file.read((char *) &r1, sizeof(unsigned char));
                input_file.read((char *) &g1, sizeof(unsigned char));
                input_file.read((char *)&b1, sizeof(unsigned char));

                r = static_cast<unsigned short>((r1 * this->args[0]) / maxval);
                g = static_cast<unsigned short>((g1 * this->args[0]) / maxval);
                b = static_cast<unsigned short>((b1 * this->args[0]) / maxval);

                r = swap16(r);
                g = swap16(g);
                b = swap16(b);

                output_file.write((char *)&r, sizeof(unsigned short));
                output_file.write((char *)&g, sizeof(unsigned short));
                output_file.write((char *)&b, sizeof(unsigned short));
            }
        }
        else {
            cerr << "Incorret Format" << endl;
            return -1;
        }
    }
    else {
        cerr << "Error: nivel de intensidad no soportado" << endl;
        return -1;
    }

    input_file.close();
    output_file.close();

    cout << "Operación de escalado de intensidad completada." << endl;

    return 0;
}
