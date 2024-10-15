//
// Created by beto on 4/10/24.
//

#include <iostream>
#include <imgaos/imageaos.hpp>
#include <common/progargs.hpp>

using namespace std;

int main (const int argc, char** argv) {
  /*
  Image checker(argc, argv);
  if (checker.check_args() < 0) {
      return -1;
  }
  */
  vector<string> arguments;
  copy(argv, next(argv, argc), back_inserter(arguments));

  ImageAOS image(argc, arguments);

  if (image.check_args() < 0) {
    return -1;
  }

  if (image.process_operation() < 0) {
    return -1;
  }

  return 0;
}
