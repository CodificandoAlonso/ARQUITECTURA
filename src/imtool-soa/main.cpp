//
// Created by beto on 4/10/24.
//
#include <iostream>
#include "common/progargs.hpp"

using namespace std;

int main (int argc, char *argv[]) {
    Checker checker(argc, argv);
    if (checker.check_args() == -1) {
        return -1;
    }

    return 0;
}
