#include <gtest/gtest.h>
#include <fstream>
#include "common/binario.hpp"

// Constantes
static constexpr int MAX_LEVEL    = 65535;
static constexpr int MIN_LEVEL    = 255;
static constexpr int MAX_ARGS     = 6;
static constexpr int DECIMAL_BASE = 10;

using namespace std;


class ImageTest : public ::testing::Test {
  protected:
  Image tree;
};
