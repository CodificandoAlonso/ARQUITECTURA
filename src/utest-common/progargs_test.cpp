#include "common/binario.hpp"

#include <common/progargs.hpp>
#include <fstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Constantes
static constexpr int MAX_LEVEL    = 65535;
static constexpr int MIN_LEVEL    = 255;
static constexpr int MAX_ARGS     = 6;
static constexpr int DECIMAL_BASE = 10;

using namespace std;

class ImageTest : public ::testing::Test {
  private:
  unique_ptr<Image> image;
  vector<string> argv;

  protected:
  ImageTest() : image(nullptr), argv({"imtool-soa", "data/deer-large.ppm", "data/deer-large.ppm", "info"}) {}

  void SetUp() override {
    image = make_unique<Image>(static_cast<int>(argv.size()), argv);
  }

  void TearDown() override {
    image.reset();
  }

  public:
  [[nodiscard]] Image* getImage() const {
    return image.get();
  }

  void setArgv(vector<string> const& newArgv) {
    argv = newArgv;
  }

  [[nodiscard]] vector<string> const& getArgv() const {
    return argv;
  }
};


TEST_F(ImageTest, InfoConstraintsArgumentosCorrectos) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "info"});
  SetUp();
  EXPECT_FALSE(getImage()->info_constraints(static_cast<int>(getImage()->getArgv().size())));
}

TEST_F(ImageTest, InfoConstraintsArgumentosInferiorAlPermitido) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm"});
  SetUp();
  EXPECT_THROW({
    getImage()->resize_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv());
  }, runtime_error);
}

TEST_F(ImageTest, InfoConstraintsArgumentosSuperiorAlPermitido) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "info", "extra"});
  SetUp();
  EXPECT_THROW({
    getImage()->resize_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv());
  },runtime_error);
}

TEST_F(ImageTest, MaxValConstraintsCorrecto) {
  setArgv({"progname", "deer-small.ppm", "deer-small.ppm", "maxlevel", "857"});
  SetUp();
  EXPECT_FALSE(getImage()->maxval_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, MaxValConstraintsNumeroArgumentosIncorrecto) {
  setArgv({"progname", "deer-small.ppm", "deer-small.ppm", "maxlevel"});
  SetUp();
  EXPECT_THROW({
    getImage()->resize_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv());
  }, runtime_error);
}

TEST_F(ImageTest, MaxValConstraintsNumeroArgumentosCorrectosNumeroNoEntre0Y65535) {
  setArgv({"progname", "deer-small.ppm", "deer-small.ppm", "maxlevel", "65536"});
  SetUp();
  EXPECT_THROW({
    getImage()->resize_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv());
  }, runtime_error);
}

TEST_F(ImageTest, ResizeConstraintsCorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "resize", "100", "100"});
  SetUp();
  EXPECT_FALSE(getImage()->resize_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, ResizeConstraintsNumeroArgumentosIncorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "resize", "100"});
  SetUp();
  EXPECT_THROW({
    getImage()->resize_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv());
  }, runtime_error);
}

TEST_F(ImageTest, ImageTest_ResizeConstraintsInvalidWidth) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "resize", "-100", "100"});
  EXPECT_THROW({
    getImage()->resize_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv());
  }, runtime_error);
}

TEST_F(ImageTest, ImageTest_ImageTest_ResizeConstraintsInvalidHeigth_Test) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "resize", "100", "-100"});
  EXPECT_THROW({
    getImage()->resize_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv());
  }, runtime_error);
}

TEST_F(ImageTest, CutfreqConstraintsCorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "cutfreq", "100"});
  SetUp();
  EXPECT_FALSE(getImage()->cutfreq_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, CutfreqConstraintsNumeroArgumentosIncorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "cutfreq"});
  SetUp();
  EXPECT_THROW({
    getImage()->cutfreq_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv());
  }, runtime_error);
}

TEST_F(ImageTest,  CutfreqConstraintsCutfreqInvalido) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "cutfreq", "-100"});
  SetUp();
  EXPECT_THROW({
    getImage()->cutfreq_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv());
  }, runtime_error);
}

TEST_F(ImageTest, CompressConstraintsCorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.cppm", "compress"});;
  SetUp();
  EXPECT_FALSE(getImage()->compress_constraints(static_cast<int>(getImage()->getArgv().size())));
}

TEST_F(ImageTest, CompressConstraintsNumeroArgumentosIncorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.cppm", "compress", "extra"});
  SetUp();
  EXPECT_THROW({
    getImage()->compress_constraints(static_cast<int>(getImage()->getArgv().size()));
  }, runtime_error);
}

TEST_F(ImageTest, CheckArgsInfoCorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "info"});
  SetUp();
  EXPECT_FALSE(getImage()->check_args());
}

TEST_F(ImageTest, CheckArgsMaxlevelCorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "maxlevel", "573"});
  SetUp();
  EXPECT_FALSE(getImage()->check_args());
}

TEST_F(ImageTest, CheckArgsResizeCorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "resize", "200", "200"});
  SetUp();
  EXPECT_FALSE(getImage()->check_args());
}

TEST_F(ImageTest, CheckArgsCutfreqCorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "cutfreq", "100"});
  SetUp();
  EXPECT_FALSE(getImage()->check_args());
}

TEST_F(ImageTest, CheckArgsCompressCorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "compress"});
  SetUp();
  EXPECT_FALSE(getImage()->check_args());
}

TEST_F(ImageTest, CheckArgsNumeroArgumentosIncorrecto) {
  setArgv({"imtool-soa", "input.ppm", "output.ppm"});
  SetUp();
  EXPECT_THROW(getImage()->check_args(), runtime_error);
}

TEST_F(ImageTest, CheckArgsOpcionInvalida) {
  setArgv({"imtool-soa", "input.ppm", "output.ppm", "incorrect"});
  SetUp();
  EXPECT_THROW(getImage()->check_args(), runtime_error);
}

TEST_F(ImageTest, InfoCorrecto) {
  setArgv({"imtool-aos", "data/deer-large.ppm","data/deer-large.ppm", "info"});
  SetUp();
  EXPECT_EQ(getImage()->info(), 0);;
}

TEST_F(ImageTest, InfoIncorrecto) {
  setArgv({"imtool-soa", "./data/deer-large.ppm","./data/deer-large.ppm", "info"});
  SetUp();
  EXPECT_EQ(getImage()->info(), -1);
}


