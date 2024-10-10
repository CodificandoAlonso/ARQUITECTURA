//
// Created by beto on 4/10/24.
//
#include <iostream>
#include <imgaos/imageaos.hpp>

#include "common/progargs.hpp"
#include "imgsoa/imagesoa.hpp"

using namespace std;

int main (int argc, char *argv[]) {
    /*
    Checker checker(argc, argv);
    if (checker.check_args() < 0) {
        return -1;
    }
    */
    ImageSOA image(argc, argv);
    if (image.process_operation() < 0) {
        return -1;
    }

    return 0;
}
