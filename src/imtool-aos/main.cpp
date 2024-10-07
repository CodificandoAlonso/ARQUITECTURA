//
// Created by beto on 4/10/24.
//

#include <iostream>
#include <common/progargs.hpp>

int main (int argc, char *argv[]) {
    if (check_args(argc, argv) < 0) {
        return -1;
    }
    return 0;
}
