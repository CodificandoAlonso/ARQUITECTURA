//
// Created by beto on 8/10/24.
//

#include <iostream>
#include <fstream>
#include "mtdata.hpp"

using namespace std;

int get_metadata(string input_file) {
    /*
     * Función común a ImageAOS e ImageSOA para leer los metadatos de la imagen de entrada .ppm.
     */
    cout << "Reading metadata from file " << input_file << endl;

    ifstream file(input_file, ios::in | ios::binary);

    if (!file) {
        cerr << "Error opening file: " << input_file << endl;
        return -1;
    }

    string format;
    file >> format;

    float width, height;
    file >> width >> height;

    int max_color_value;
    file >> max_color_value;

    cout << "Format: " << format << endl;
    cout << "Width: " << width << endl;
    cout << "Height: " << height << endl;
    cout << "Max color value: " << max_color_value << endl;

    return 0;
}