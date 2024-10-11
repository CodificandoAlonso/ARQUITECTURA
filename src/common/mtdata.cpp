//
// Created by beto on 8/10/24.
//

#include <iostream>
#include <fstream>
#include "mtdata.hpp"
#include <cmath>

using namespace std;

int get_metadata(const string& input_file) {
    /*
     * Función común a ImageAOS e ImageSOA para leer los metadatos de la imagen de entrada .ppm.
     */
    cout << "Reading metadata from file " << input_file << '\n';

    ifstream file(input_file, ios::in | ios::binary);

    if (!file) {
        cerr << "Error opening file: " << input_file << '\n';
        return -1;
    }

    string format;
    file >> format;

    float width = NAN;
    float height = NAN;
    file >> width >> height;

    int max_color_value = 0;
    file >> max_color_value;

    cout << "Format: " << format << '\n';
    cout << "Width: " << width << '\n';
    cout << "Height: " << height << '\n';
    cout << "Max color value: " << max_color_value << '\n';

    return 0;
}