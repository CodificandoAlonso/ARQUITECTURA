#include "common/binario.hpp"

#include <common/progargs.hpp>
#include <fstream>
#include <gtest/gtest.h>

// Constantes
static constexpr int MAX_LEVEL    = 65535;
static constexpr int MIN_LEVEL    = 255;
static constexpr int MAX_ARGS     = 6;
static constexpr int DECIMAL_BASE = 10;

using namespace std;

class ImageTest : public ::testing::Test {
  protected:
  std::unique_ptr<Image> image;

  // Constructor
  ImageTest() : image(nullptr) {}

  void SetUp() override {
    // Initialize the argv vector with default values
    std::vector<std::string> const argv = {"imtool-soa", "input.ppm", "output.ppm", "info"};
    // Create a new Image object with the default arguments
    image = std::make_unique<Image>(static_cast<int>(argv.size()), argv);
  }

  void TearDown() override {
    // Clean up the Image object
    image.reset();
  }
};

TEST_F(ImageTest, InfoConstraintsCorrectArgs) {
  // Custom setup for this test
  std::vector<std::string> const argv = {"progname", "input.ppm", "output.ppm", "info"};
  image = std::make_unique<Image>(static_cast<int>(argv.size()), argv);
  EXPECT_FALSE(image->info_constraints(static_cast<int>(image->getArgv().size())));
}

TEST_F(ImageTest, InfoConstraintsIncorrectArgs) {
  // Custom setup for this test
  std::vector<std::string> const argv = {"progname", "input.ppm", "output.ppm"};
  image = std::make_unique<Image>(static_cast<int>(argv.size()), argv);
  EXPECT_TRUE(image->info_constraints(static_cast<int>(image->getArgv().size())));
}
