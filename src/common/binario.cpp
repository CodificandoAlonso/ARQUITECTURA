//
// Created by beto on 4/10/24.
//

#include "binario.hpp"
#include <iostream>
#include <bitset>
#include <vector>
using namespace std;
static constexpr int BYTE = 8;

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

namespace quick {
  int partition(vector<pair<string, int>>& vec, const int low, const int high) {
    auto pivot = vec[high];  // Usamos el último elemento como pivote
    int i = low - 1;

    for (int j = low; j < high; j++) {
      if (vec[j].first < pivot.first) {  // Comparamos solo el primer elemento del par
        i++;
        swap(vec[i], vec[j]);
      }
    }
    swap(vec[i + 1], vec[high]);
    return i + 1;
  }

  void quicksort(vector<pair<string, int>>& vec, const int low,const int high) {
    if (low < high) {
      int pivotIndex = partition(vec, low, high);
      quicksort(vec, low, pivotIndex - 1);
      quicksort(vec, pivotIndex + 1, high);
    }
  }
}










