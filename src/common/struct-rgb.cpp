//
// Created by hector on 9/10/24.
//

#include "struct-rgb.hpp"

#include <vector>
using namespace std;

size_t soa_rgb_small::find_color(rgb_small color) const {
  size_t left  = 0;
  size_t right = this->r.size();

  while (left < right) {
    size_t const mid = left + (right - left) / 2;

    if (tie(this->r[mid], this->g[mid], this->b[mid]) < tie(color.r, color.g, color.b)) {
      left = mid + 1;
    } else {
      right = mid;
    }
  }
  if (left < this->r.size() &&
      tie(this->r[left], this->g[left], this->b[left]) == tie(color.r, color.g, color.b)) {
    return left;
  }
  return this->r.size();
}
