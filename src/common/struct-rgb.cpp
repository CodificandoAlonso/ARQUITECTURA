//
// Created by hector on 9/10/24.
//

#include "struct-rgb.hpp"

#include <iostream>

void soa_rgb_small::print(unsigned int index) {
  if (index < r.size()) {
    std::cout << static_cast<int>(static_cast<unsigned char>(r[index])) << " "
            << static_cast<int>(static_cast<unsigned char>(g[index])) << " "
            << static_cast<int>(static_cast<unsigned char>(b[index])) << '\n';
  } else {
    std::cout << "Index " << index << " out of bounds." << '\n';
  }
}
