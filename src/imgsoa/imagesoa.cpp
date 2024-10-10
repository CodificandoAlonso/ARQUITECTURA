//
// Created by beto on 4/10/24.
//

#include "imagesoa.hpp"
#include<fstream>
#include "common/mtdata.hpp"
#include "common/binario.hpp"
#include "common/struct-rgb.hpp"
#include "common/progargs.hpp"


using namespace std;

ImageSOA::ImageSOA(int argc, char *argv[]) : Checker(argc, argv) {
    check_args();  // Llama a la función check_args() de la clase base
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
        cerr << "Operación no soportada: " << optype << "\n";
        return -1;
    }
    return 0;
}


int ImageSOA::maxlevel() {

    ifstream input_file(this->input_file, ios::binary);
    ofstream output_file(this->output_file, ios::binary);

    if (!input_file || !output_file) {
        cerr << "Error al abrir los archivos de entrada/salida" << "\n";
        return -1;
    }

    string format;
    int width=0, height =0, maxval =0;
    input_file >> format >> width >> height >> maxval;
    input_file.ignore(1);

    if (constexpr int small_maxsize = 256; this->args[0] < small_maxsize) {
        output_file << format << " " << width << " " << height << " " << small_maxsize -1 << "\n";
        if(maxval< small_maxsize) {
            //Este caso es origen maxvalue <256 y destino <256
            unsigned char r =0,g = 0,b= 0;
            soa_rgb_small mysoa;

            for(int i=0; i< width * height; i++) {

                //Lectura de entrada

                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
                input_file.read(reinterpret_cast<char*>(&r), sizeof(r));
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
                input_file.read(reinterpret_cast<char*>(&g), sizeof(r));
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
                input_file.read(reinterpret_cast<char*>(&b), sizeof(r));

                //Calculo de valores

                r = static_cast<unsigned char>(r* this->args[3]/ maxval);
                g = static_cast<unsigned char>(g* this->args[3]/ maxval);
                b = static_cast<unsigned char>(b* this->args[3]/ maxval);

                //Guardado en soa

                mysoa.r.push_back(r);
                mysoa.g.push_back(g);
                mysoa.b.push_back(b);
            }

            for(vector<unsigned char>::size_type i = 0; i < mysoa.r.size();i++) {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
                output_file.write(reinterpret_cast<char*>(&mysoa.r[i]), sizeof(unsigned char));
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
                output_file.write(reinterpret_cast<char*>(&mysoa.g[i]), sizeof(unsigned char));
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
                output_file.write(reinterpret_cast<char*>(&mysoa.b[i]), sizeof(unsigned char));
            }
        }
    }
    return 0;
}



