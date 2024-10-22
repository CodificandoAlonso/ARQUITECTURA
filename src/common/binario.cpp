//
// Created by beto on 4/10/24.
//

#include "binario.hpp"
#include <cstddef>
#include <iostream>
#include <bitset>
#include <vector>
using namespace std;
static constexpr int BYTE = 8;
static constexpr int BYTE_2 = 16;

/**
 * Esta función convierte dos bytes en un número de 16 bits (2 bytes).
 * De esta forma, si tenemos dos bytes 0x12 y 0x34, al aplicar esta función
 * obtendremos 0x3412.
 */
unsigned short merge16(const unsigned char op1, const unsigned char op2) {
    return op2 << BYTE | op1;
}
/**
 * Esta función intercambia los bytes de un número de 16 bits (2 bytes).
 * De esta forma, si tenemos un número 0x1234, al aplicar esta función
 * obtendremos 0x3412.
 */
unsigned short swap16(const unsigned short op) {
    return op >> BYTE | op << BYTE;
}


string mix3char(const char ch1, const char ch2, const char ch3) {
      bitset<BYTE> const byte1(static_cast<unsigned char>(ch1));
      bitset<BYTE> const byte2(static_cast<unsigned char>(ch2));
      bitset<BYTE> const byte3(static_cast<unsigned char>(ch3));
    return byte1.to_string() + byte2.to_string() + byte3.to_string();

}

__uint8_t extractblue(const string& rgb) {
  const string blue = rgb.substr(rgb.size() - BYTE, BYTE);
  bitset<BYTE> const bluebinary(blue);
  auto const blueint = static_cast<__uint8_t>(bluebinary.to_ulong());
  return blueint;
}

__uint8_t extractgreen(const string& rgb) {
  const string green = rgb.substr(rgb.size() - BYTE_2, BYTE);
  bitset<BYTE> const greenbinary(green);
  auto const greenint = static_cast<__uint8_t>(greenbinary.to_ulong());
  return greenint;
}

__uint8_t extractred(const string& rgb) {
  const string red = rgb.substr(0, BYTE);
  bitset<BYTE> const redbinary(red);
  auto const redint = static_cast<__uint8_t>(redbinary.to_ulong());
  return redint;
}


namespace quick {
  size_t partition(vector<pair<string, int>>& vec, size_t low, size_t high) {
    if (low == high) return low; // caso base, cuando solo hay un elemento

    int pivot = vec[high].second;
    //cerr << "Pivot: " << pivot << endl;
    int i = static_cast<int>(low) - 1;

    for (size_t j = low; j < high; j++) {
      if (vec[j].second < pivot) {
        i++;
        swap(vec[static_cast<size_t>(i)], vec[j]);
      }
    }
    auto const aux = static_cast<size_t>(i + 1);
    swap(vec[aux], vec[high]); // Asegúrate de que `aux` esté dentro de límites
    return aux; // Devuelve el índice del pivote correctamente
  }



  void quicksort(vector<pair<string, int>>& vec, size_t low, size_t high) {
    if (low < high) {
      size_t const pivotIndex = partition(vec, low, high);

      // Verifica que los índices sean válidos antes de la llamada recursiva
      if (pivotIndex > 0) {
        quicksort(vec, low, pivotIndex - 1);
      }
      quicksort(vec, pivotIndex + 1, high);
    }
  }



}










