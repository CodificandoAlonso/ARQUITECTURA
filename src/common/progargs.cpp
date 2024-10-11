//
// Created by beto on 4/10/24.
//

#include <iostream>
#include "progargs.hpp"

// Constantes
static const int MAX_LEVEL = 65535;
static const int MAX_ARGS = 6;
static const int DECIMAL_BASE = 10;


using namespace std;

Image::Image(int argc, const vector<string> &argv) : argc(argc), argv(argv) {
}


int Image::check_args() {
    /*
     * La aplicación tomará los siguientes parámetros:
     *      - Ruta del archivo de entrada
     *      - Ruta del archivo de salida
     *      - Opción a ejecutar (info, maxlevel, resize, cutfreq, compress)
     *      - Parámetros adicionales
     */
    // Para mayor facilidad de uso
    int const argc = this->argc;
    vector<string> argv = this->argv;

    if (argc <= 3) {
        cerr << "Error: Invalid number of arguments: " << argc << '\n';
        return -1;
    }
    this->input_file = argv[1];
    this->output_file = argv[2];

    // Comprobaremos ahora que el tercer argumento corresponde con la opción a ejecutar
    string const& option = argv[3];
    if (option != "info" && option != "maxlevel" && option != "resize" && option != "cutfreq" && option != "compress") {
        cerr << "Error: Invalid option: " << option << '\n';
        return -1;
    }
    this->optype = option;

    // Si la opción es info, deben ser exactamente tres argumentos
    if (option == "info" && argc != 4) {
        cerr << "Error: Invalid number of arguments for option info: " << argc << '\n';
        return -1;
    }

    // Si la opción es maxlevel, el número de argumentos debe ser exactamente cuatro. El cuarto argumento
    // debe ser un número entero entre los valores 0 y 65535.
    else if (option == "maxlevel") {
        if (argc != MAX_ARGS - 1) {
            cerr << "Error: Invalid number of arguments for option maxlevel: " << argc << '\n';
            return -1;
        }
        // Comprobamos que el cuarto argumento sea un número entero entre 0 y 65535, teniendo en cuenta
        // que tampoco puede ser una palabra
        char *end = nullptr;
        long argument = strtol(argv[4].c_str(), &end, DECIMAL_BASE);
        if (argument < 0 || argument > MAX_LEVEL || (argument == 0 && argv[4][0] != '0')) {
            cerr << "Error: Invalid argument for option maxlevel: " << argv[4] << '\n';
            return -1;
        }
        this->args.push_back(static_cast<int>(argument));
    }

    // Si la opción es resize, el número de argumentos debe ser exactamente cinco. El cuarto y quinto argumento
    // deben ser números enteros positivos.
    else if (option == "resize") {
        if (argc != MAX_ARGS) {
            cerr << "Error: Invalid number of arguments for option resize: " << argc << '\n';
            return -1;
        }
        // Comprobamos que el cuarto y quinto argumento sean números enteros positivos
        char *end = nullptr;
        long argument1 = strtol(argv[4].c_str(), &end, DECIMAL_BASE);
        long argument2 = strtol(argv[MAX_ARGS - 1].c_str(), &end, DECIMAL_BASE);
        if (argument1 <= 0) {
            cerr << "Error: Invalid resize width: " << argv[4] << '\n';
            return -1;
        }
        if (argument2 <= 0) {
            cerr << "Error: Invalid resize height: " << argv[MAX_ARGS - 1] << '\n';
            return -1;
        }
        this->args.push_back(static_cast<int>(argument1));
        this->args.push_back(static_cast<int>(argument2));
    }

    // Si la opción es cutfreq, el número de argumentos debe ser exactamente cuatro. El cuarto argumento
    // debe ser un número entero positivo.
    else if (option == "cutfreq") {
        if (argc != MAX_ARGS - 1) {
            cerr << "Error: Invalid number of arguments for cutfreq: " << argc << '\n';
            return -1;
        }
        // Comprobamos que el cuarto argumento sea un número entero positivo
        char *end = nullptr;
        long argument = strtol(argv[4].c_str(), &end, DECIMAL_BASE);
        if (argument <= 0) {
            cerr << "Error: Invalid cutfreq: " << argv[4] << '\n';
            return -1;
        }
        this->args.push_back(static_cast<int>(argument));
    }

    // Si la opción es compress, el número de argumentos debe ser exactamente tres.
    else if (option == "compress" && argc != 4) {
        cerr << "Error: Invalid extra arguments for compress: " << argc << '\n';
        return -1;
    }

    return 0;
}
