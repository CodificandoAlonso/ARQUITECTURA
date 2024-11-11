#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

/**
 * Script para recuperar fotos con formato ficticio .cppm a .ppm.
 * Estas fotos poseen un encabezado con el formato, el ancho, el largo, 
 * el máximo valor de color y el número de colores de la imagen. Después 
 * del encabezado se encuentran los colores de la imagen en formato RGB 
 * (si el valor máximo de color es mayor a 255, cada canal de color ocupa
 * 2 bytes en lugar de 1). Por último, se encuentran los índices de los colores 
 * de la imagen correspondientes a los píxeles de la misma. 
 * 
 * 
 * Si la imagen tiene más de 2^8 colores, los índices ocupan 1 byte.
 * Si la imagen tiene más de 2^16 colores, los índices ocupan 2 bytes.
 * Si la imagen tiene más de 2^32 colores, los índices ocupan 4 bytes.
 * Siempre en formato little endian.
 */

using namespace std;

string obtenerNombreSalida(string const & nombreEntrada) {
  size_t pos = nombreEntrada.find_last_of('.');
  if (pos == string::npos) {
    // Si no tiene extensión, simplemente añade ".ppm"
    return nombreEntrada + ".ppm";
  }
  return nombreEntrada.substr(0, pos) + ".ppm";
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo.cppm>" << std::endl;
        return 1;
    }
    ifstream file(argv[1], ios::binary);
    if (!file) {
        std::cerr << "No se pudo abrir el archivo " << argv[1] << std::endl;
        return 1;
    }
    string format;
    int width, height, max_color, num_colors;
    file >> format >> width >> height >> max_color >> num_colors;
    file.ignore(1);
    cout << "Formato: " << format << endl;
    cout << "Ancho: " << width << endl;
    cout << "Alto: " << height << endl;
    cout << "Máximo valor de color: " << max_color << endl;
    cout << "Número de colores: " << num_colors << endl;

    // renombraremos el archivo de salida para que tenga el mismo nombre que 
    // el archivo de entrada pero con extensión .ppm
    string nombreSalida = obtenerNombreSalida(argv[1]);
    ofstream out(nombreSalida, ios::binary);
    out << "P6\n" << width << " " << height << "\n" << max_color << "\n";

    int bytes_per_channel = (max_color > 255) ? 2 : 1;

    std::vector<std::array<uint16_t, 3>> colors(num_colors);
    for (int i = 0; i < num_colors; ++i) {
      uint16_t r, g, b;
      if (bytes_per_channel == 2) {
        file.read(reinterpret_cast<char *>(&r), 2);
        file.read(reinterpret_cast<char *>(&g), 2);
        file.read(reinterpret_cast<char *>(&b), 2);
      } else {
        uint8_t r8, g8, b8;
        file.read(reinterpret_cast<char *>(&r8), 1);
        file.read(reinterpret_cast<char *>(&g8), 1);
        file.read(reinterpret_cast<char *>(&b8), 1);
        r = r8;
        g = g8;
        b = b8;
      }
      colors[i] = {r, g, b};
    }

    // Determinar bytes por índice según el número de colores
    int bytes_per_index;
    if (num_colors <= 256) {
      bytes_per_index = 1;
    } else if (num_colors <= 65536) {
      bytes_per_index = 2;
    } else {
      bytes_per_index = 4;
    }
    int total_pixels = width * height;
    for (int i = 0; i < total_pixels; ++i) {
      uint32_t index = 0;
      file.read(reinterpret_cast<char *>(&index), bytes_per_index);
      auto color = colors[index];

      if (bytes_per_channel == 2) {
        uint16_t r = color[0];
        uint16_t g = color[1];
        uint16_t b = color[2];
        out.write(reinterpret_cast<char *>(&r), 2);
        out.write(reinterpret_cast<char *>(&g), 2);
        out.write(reinterpret_cast<char *>(&b), 2);
      } else {
        uint8_t r = color[0];
        uint8_t g = color[1];
        uint8_t b = color[2];
        out.write(reinterpret_cast<char *>(&r), 1);
        out.write(reinterpret_cast<char *>(&g), 1);
        out.write(reinterpret_cast<char *>(&b), 1);
      }
    }

    file.close();
    out.close();

    std::cout << "Done!" << std::endl;

    return 0;
}
