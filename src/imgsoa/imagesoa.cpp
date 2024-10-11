//
// Created by beto on 4/10/24.
//


#include <iostream>
#include <fstream>
#include "imagesoa.hpp"

#include <sys/stat.h>

#include "common/mtdata.hpp"
#include "common/struct-rgb.hpp"
#include "common/progargs.hpp"

static const int MAX_LEVEL = 65535;
static const int MIN_LEVEL = 255;


using namespace std;

ImageSOA::ImageSOA(int argc, const vector<string> &argv) : Image(argc, argv) {
}

int ImageSOA::process_operation() {
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
        cerr << "Operación no soportada de momento: " << this->get_optype() << "\n";
        return -1;
    }
    return 0;
}


int ImageSOA::maxlevel() {

    ifstream input_file(this->get_input_file(), ios::binary);
    ofstream output_file(this->get_output_file(), ios::binary);

    if (!input_file || !output_file) {
        cerr << "Error al abrir los archivos de entrada/salida" << "\n";
        return -1;
    }

    string format;
    int width=0, height =0, maxval =0;
    input_file >> format >> width >> height >> maxval;
    input_file.ignore(1);

    if (this->get_args()[0] < MIN_LEVEL + 1) {
        output_file << format << " " << width << " " << height << " " << MIN_LEVEL << "\n";
        if(maxval <= MIN_LEVEL) {
            //Este caso es origen maxvalue <256 y destino <256
            char r = 0, g = 0, b = 0;
            soa_rgb_small mysoa;

            // Leer y escribir en memoria
            for(int i = 0; i < width * height; i++) {

                //Lectura de entrada
                vector<char> const buffer(sizeof(r));

                input_file.read(&r, sizeof(r));
                input_file.read(&g, sizeof(r));
                input_file.read(&b, sizeof(r));

            //A ver si me deja coommitear
            //ESTO SE BORRA LUEGO.   Chati dice que a un char si le restas '0' se puede convertir dentro de una variable int y sumandole '0' viceversa.
            //Esto creo que tiene que ver con que char es cn signo y sumarle 0 es como indicar que es de signo positivo(PORRO)


                // Calculamos el nuevo valor de cada pixel teniendo en cuenta el nuevo maxval
                int new_r = 0, new_g = 0, new_b = 0;
                new_r = r - '0';
                new_g = g - '0';
                new_b = b -'0';
                new_r = (new_r * this->get_args()[0])/maxval;
                new_g = (new_g * this->get_args()[0])/maxval;
                new_b = (new_b * this->get_args()[0])/maxval;
                r = static_cast<char>(new_r + '0');
                g = static_cast<char>(new_g + '0');
                b = static_cast<char>(new_b + '0');
                //Guardado en soa
                mysoa.r.push_back(r);
                mysoa.g.push_back(g);
                mysoa.b.push_back(b);
            }

            // Leer de memoria y escribir imagen
            for(vector<char>::size_type i = 0; i < mysoa.r.size(); i++) {
                // Escribimos en el output file los datos almecenados
                output_file.write(&mysoa.r[i], sizeof(r));
                output_file.write(&mysoa.g[i], sizeof(g));
                output_file.write(&mysoa.b[i], sizeof(b));
            }
        }
    }
    return 0;
}



