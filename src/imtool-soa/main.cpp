//
// Created by beto on 4/10/24.
//

#include "common/progargs.hpp"
#include "imgsoa/imagesoa.hpp"

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

    ImageSOA image(argc, arguments);

    if (image.check_args() < 0) {
        return -1;
    }

    if (image.process_operation() < 0) {
        return -1;
    }

    return 0;
}
