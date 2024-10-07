//
// Created by beto on 4/10/24.
//

#include <iostream>
#include "progargs.hpp"

using namespace std;

class Checker {
public:
    string input_file;
    string output_file;
    string optype;
    vector<int> args;

    int check_args() {
        /*
         * La aplicación tomará los siguientes parámetros:
         *      - Ruta del archivo de entrada
         *      - Ruta del archivo de salida
         *      - Opción a ejecutar (info, maxlevel, resize, cutfreq, compress)
         *      - Parámetros adicionales
         */
        // Para mayor facilidad de uso
        int argc = this->argc;
        char *argv[] = this->argv;

        if (argc <= 3) {
            cerr << "Error: Invalid number of arguments: " << argc << endl;
            return -1;
        }
        this->input_file = argv[1];
        this->output_file = argv[2];

        // Comprobaremos ahora que el tercer argumento corresponde con la opción a ejecutar
        string option = argv[3];
        if (option != "info" && option != "maxlevel" && option != "resize" && option != "cutfreq" && option != "compress") {
            cerr << "Error: Invalid option: " << option << endl;
            return -1;
        }
        this->optype = option;

        // Si la opción es info, deben ser exactamente tres argumentos
        if (option == "info" && argc != 4) {
            cerr << "Error: Invalid number of arguments for option info: " << argc << endl;
            return -1;
        }

        // Si la opción es maxlevel, el número de argumentos debe ser exactamente cuatro. El cuarto argumento
        // debe ser un número entero entre los valores 0 y 65535.
        else if (option == "maxlevel") {
            if (argc != 5) {
                cerr << "Error: Invalid number of arguments for option maxlevel: " << argc << endl;
                return -1;
            }
            // Comprobamos que el cuarto argumento sea un número entero entre 0 y 65535, teniendo en cuenta
            // que tampoco puede ser una palabra
            int argument = atoi(argv[4]);
            if (argument < 0 || argument > 65535 || (argument == 0 && argv[4][0] != '0')) {
                cerr << "Error: Invalid argument for option maxlevel: " << argv[4] << endl;
                return -1;
            }
            this->args.push_back(argument);
        }

        // Si la opción es resize, el número de argumentos debe ser exactamente cinco. El cuarto y quinto argumento
        // deben ser números enteros positivos.
        else if (option == "resize") {
            if (argc != 6) {
                cerr << "Error: Invalid number of arguments for option resize: " << argc << endl;
                return -1;
            }
            // Comprobamos que el cuarto y quinto argumento sean números enteros positivos
            int argument1 = atoi(argv[4]);
            int argument2 = atoi(argv[5]);
            if (argument1 <= 0) {
                cerr << "Error: Invalid resize width: " << argv[4] << endl;
                return -1;
            }
            if (argument2 <= 0) {
                cerr << "Error: Invalid resize height: " << argv[5] << endl;
                return -1;
            }
            this->args.push_back(argument1);
            this->args.push_back(argument2);
        }

        // Si la opción es cutfreq, el número de argumentos debe ser exactamente cuatro. El cuarto argumento
        // debe ser un número entero positivo.
        else if (option == "cutfreq") {
            if (argc != 5) {
                cerr << "Error: Invalid number of arguments for cutfreq: " << argc << endl;
                return -1;
            }
            // Comprobamos que el cuarto argumento sea un número entero positivo
            int argument = atoi(argv[4]);
            if (argument <= 0) {
                cerr << "Error: Invalid cutfreq: " << argv[4] << endl;
                return -1;
            }
            this->args.push_back(argument);
        }

        // Si la opción es compress, el número de argumentos debe ser exactamente tres.
        else if (option == "compress" && argc != 4) {
            cerr << "Error: Invalid extra arguments for compress: " << argc << endl;
            return -1;
        }

        return 0;
    }

    Checker(int argc, char *argv[]) {
        this->argc = argc;
        this->argv = argv;
    };
private:
    int argc;
    char *argv[];
};
