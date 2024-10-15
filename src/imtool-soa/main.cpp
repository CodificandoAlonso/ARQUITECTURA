//
// Created by beto on 4/10/24.
//

#include "imgsoa/imagesoa.hpp"

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

int main(int const argc, char ** argv) {
  vector<string> arguments;
  copy(argv, next(argv, argc), back_inserter(arguments));

  ImageSOA image(argc, arguments);

  if (image.check_args() < 0) { return -1; }

  if (image.process_operation() < 0) { return -1; }

  return 0;
}
