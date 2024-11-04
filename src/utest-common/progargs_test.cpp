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
  private:
  std::unique_ptr<Image> image;
  std::vector<std::string> argv;

  protected:
  ImageTest() : image(nullptr), argv({"imtool-soa", "input.ppm", "output.ppm", "info"}) {}

  void SetUp() override {
    image = std::make_unique<Image>(static_cast<int>(argv.size()), argv);
  }

  void TearDown() override {
    image.reset();
  }

  public:
  [[nodiscard]] Image* getImage() const {
    return image.get();
  }

  void setArgv(std::vector<std::string> const& newArgv) {
    argv = newArgv;
  }
};

TEST_F(ImageTest, InfoConstraintsArgumentosCorrectos) {
  // Custom setup for this test
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "info"});
  SetUp();
  EXPECT_FALSE(getImage()->info_constraints(static_cast<int>(getImage()->getArgv().size())));
}

TEST_F(ImageTest, InfoConstraintsArgumentosInferiorAlPermitido) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm"});
  SetUp();
  EXPECT_TRUE(getImage()->info_constraints(static_cast<int>(getImage()->getArgv().size())));
}

TEST_F(ImageTest, InfoConstraintsArgumentosSuperiorAlPermitido) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "info", "extra"});
  SetUp();
  EXPECT_TRUE(getImage()->info_constraints(static_cast<int>(getImage()->getArgv().size())));
}

TEST_F(ImageTest, MaxValConstraintsArgumentosCorrectos) {
  setArgv({"progname", "input.ppm", "output.ppm", "maxlevel", "857"});
  SetUp();
  EXPECT_FALSE(getImage()->maxval_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, MaxValConstraintsNumeroArgumentosIncorrecto) {
  setArgv({"progname", "input.ppm", "output.ppm", "maxlevel"});
  SetUp();
  EXPECT_TRUE(getImage()->maxval_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}
