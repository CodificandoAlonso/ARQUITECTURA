//
// Created by beto on 4/10/24.
//

#include <iostream>
#include <imgaos/imageaos.hpp>
#include <common/progargs.hpp>

using namespace std;

int main (int argc, char *argv[]) {

    Checker checker(argc, argv);
    if (checker.check_args() < 0) {
        return -1;
    }

    ImageAOS image(checker.input_file, checker.output_file, checker.optype, checker.args);
    if (image.process_operation() < 0) {
        return -1;
    }

    return 0;
}
