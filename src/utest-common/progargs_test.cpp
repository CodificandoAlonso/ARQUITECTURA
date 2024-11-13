#include "common/binario.hpp"

#include <common/progargs.hpp>
#include <fstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

static constexpr __uint32_t NUMERO_BIN_1 = 0x123456;
static constexpr __uint32_t NUMERO_BIN_2 = 0x654321;
static constexpr __uint32_t NUMERO_BIN_3 = 0xabcdef;
static constexpr __uint32_t NUMERO_BIN_4 = 0xffaa56;
static constexpr __uint32_t NUMERO_BIN_5 = 0xff3456;
static constexpr __uint32_t NUMERO_BIN_6 = 0xaa3456;
static constexpr __uint64_t NUMERO_BIN_1_BIG = 0xabcdddef3369;
static constexpr __uint64_t NUMERO_BIN_2_BIG = 0x876543213542;
static constexpr __uint64_t NUMERO_BIN_3_BIG = 0xabcdef01ab34;
static constexpr __uint64_t NUMERO_BIN_4_BIG = 0xaaaaaaaa3369;
static constexpr __uint64_t NUMERO_BIN_5_BIG = 0xaaaa12343369;
static constexpr __uint64_t NUMERO_BIN_6_BIG = 0xaaaa12343369;
// Constantes
using namespace std;

#include <iostream>
#include <filesystem> // C++17

class ImageTest : public ::testing::Test {
  private:
  std::unique_ptr<Image> image;
  std::vector<std::string> argv;

  protected:
  ImageTest()
      : image(nullptr),
        argv({"imtool-soa", "data/deer-large.ppm", "data/pipupipu.ppm", "info"}) {}

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

  [[nodiscard]] std::vector<std::string> const& getArgv() const {
    return argv;
  }

  private:
  string input_file;
  string output_file;
  ifstream if_input_file;
  ofstream of_output_file;
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
  EXPECT_THROW({
    const bool result = getImage()->check_args(); //Guardamos el resultado para que no nos de error de Clang-tidy
    (void)result;
  }, runtime_error);
}

TEST_F(ImageTest, CheckArgsOpcionInvalida) {
  setArgv({"imtool-soa", "input.ppm", "output.ppm", "incorrect"});
  SetUp();
  EXPECT_THROW({
    const bool result = getImage()->check_args(); //Guardamos el resultado para que no nos de error de Clang-tidy
    (void)result;
  }, runtime_error);
}

TEST_F(ImageTest, CheckArgsCorrectoInfo) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "info"});
  SetUp();
  EXPECT_EQ(getImage()->check_args(), 0);
}

TEST_F(ImageTest, CheckArgsCorrectoMaxlevel) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "maxlevel", "573"});
  SetUp();
  EXPECT_EQ(getImage()->check_args(), 0);
}

TEST_F(ImageTest, CheckArgsCorrectoResize) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "resize", "200", "200"});
  SetUp();
  EXPECT_EQ(getImage()->check_args(), 0);
}

TEST_F(ImageTest, CheckArgsCorrectoCutfreq) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "cutfreq", "100"});
  SetUp();
  EXPECT_EQ(getImage()->check_args(), 0);
}

TEST_F(ImageTest, CheckArgsCorrectoCompress) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "compress"});
  SetUp();
  EXPECT_EQ(getImage()->check_args(), 0);
}

TEST_F(ImageTest, CfSameBgrVectorCorrectoAzul) {
  params_same_vector_small params;
  params.counter = 3;
  params.value = 1; // Azul
  params.father_vector = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_2, 0}, {NUMERO_BIN_3, 0}};

  auto result = Image::cf_same_bgr_vector(params);

  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result[0].second, 239);
  EXPECT_EQ(result[1].second, 86);
  EXPECT_EQ(result[2].second, 33);
  //El vector está ordenado en función del valor de blue que tienen
}

TEST_F(ImageTest, CfSameBgrVectorCorrectoVerde) {
  params_same_vector_small params;
  params.counter = 3;
  params.value = 2; // Verde
  params.father_vector = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_2, 0}, {NUMERO_BIN_3, 0}};

  auto result = Image::cf_same_bgr_vector(params);

  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result[0].second, 205);
  EXPECT_EQ(result[1].second, 67);
  EXPECT_EQ(result[2].second, 52);
  //El vector está ordenado en función del valor de green que tienen
}

TEST_F(ImageTest, CfSameBgrVectorCorrectoRojo) {
  params_same_vector_small params;
  params.counter = 3;
  params.value = 3; // Rojo
  params.father_vector = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_2, 0}, {NUMERO_BIN_3, 0}};

  auto result = Image::cf_same_bgr_vector(params);

  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result[0].second, 171);
  EXPECT_EQ(result[1].second, 101);
  EXPECT_EQ(result[2].second, 18);
  //El vector está ordenado en función del valor de red que tienen
}

TEST_F(ImageTest, CfSameBgrVectorBigCorrectoAzul) {
  params_same_vector_BIG params;
  params.counter = 3;
  params.value = 1; // Azul
  params.father_vector = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 0}, {NUMERO_BIN_3_BIG, 0}};

  auto result = Image::cf_same_bgr_vector_BIG(params);

  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result[0].second, 43828);
  EXPECT_EQ(result[1].second, 13634);
  EXPECT_EQ(result[2].second, 13161);
  //El vector está ordenado en función del valor de blue que tienen
}

TEST_F(ImageTest, CfSameBgrVectorBigCorrectoVerde) {
  params_same_vector_BIG params;
  params.counter = 3;
  params.value = 2; // Verde
  params.father_vector = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 0}, {NUMERO_BIN_3_BIG, 0}};

  auto result = Image::cf_same_bgr_vector_BIG(params);

  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result[0].second, 61185);
  EXPECT_EQ(result[1].second, 56815);
  EXPECT_EQ(result[2].second, 17185);
  //El vector está ordenado en función del valor de green que tienen
}

TEST_F(ImageTest, CfSameBgrVectorBigCorrectoRojo) {
  params_same_vector_BIG params;
  params.counter = 3;
  params.value = 3; // Rojo
  params.father_vector = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 0}, {NUMERO_BIN_3_BIG, 0}};

  auto result = Image::cf_same_bgr_vector_BIG(params);

  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result[0].second, 43981);
  EXPECT_EQ(result[1].second, 43981);
  EXPECT_EQ(result[2].second, 34661);
  //El vector está ordenado en función del valor de red que tienen
}

TEST_F(ImageTest, CfCheckAndDeleteAzul) {
  deque<pair<__uint32_t, __uint16_t>> color_vector = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_4,0}};
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  deque<pair<__uint32_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_4,0}};

  int const result = Image::cf_check_and_delete(color_vector, 1, Deleteitems, bluevalues);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(Deleteitems.size(), 1);
  EXPECT_EQ(Deleteitems.count(NUMERO_BIN_4), 1);
}

TEST_F(ImageTest, CfCheckAndDeleteVerde) {
  deque<pair<__uint32_t, __uint16_t>> color_vector = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_5,0}};
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  deque<pair<__uint32_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_5,0}};

  int const result = Image::cf_check_and_delete(color_vector, 1, Deleteitems, bluevalues);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(Deleteitems.size(), 1);
  EXPECT_EQ(Deleteitems.count(NUMERO_BIN_5), 1);
}

TEST_F(ImageTest, CfCheckAndDeleteTresAzul) {
  deque<pair<__uint32_t, __uint16_t>> color_vector = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_4, 0}, {NUMERO_BIN_5, 0}};
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  deque<pair<__uint32_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1, 1}, {NUMERO_BIN_4, 0}, {NUMERO_BIN_5, 0}};

  int const result = Image::cf_check_and_delete(color_vector, 1, Deleteitems, bluevalues);

  EXPECT_EQ(result, 3);
  EXPECT_EQ(Deleteitems.size(), 0);
}

TEST_F(ImageTest, CfCheckAndDeleteTresVerde) {
  deque<pair<__uint32_t, __uint16_t>> color_vector = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_4, 0}, {NUMERO_BIN_6, 0}};
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  deque<pair<__uint32_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1, 1}, {NUMERO_BIN_4, 0}, {NUMERO_BIN_6, 0}};

  int const result = Image::cf_check_and_delete(color_vector, 1, Deleteitems, bluevalues);

  EXPECT_EQ(result, 3);
  EXPECT_EQ(Deleteitems.size(), 0);
}

TEST_F(ImageTest, CfCheckAndDeleteBIGAzul) {
  deque<pair<__uint64_t, __uint16_t>> color_vector = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_4_BIG,0}};
  unordered_map<__uint64_t, __uint64_t> Deleteitems;
  deque<pair<__uint64_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_4_BIG,0}};

  int const result = Image::cf_check_and_delete_BIG(color_vector, 1, Deleteitems, bluevalues);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(Deleteitems.size(), 1);
  EXPECT_EQ(Deleteitems.count(NUMERO_BIN_1_BIG), 1);
}

TEST_F(ImageTest, CfCheckAndDeleteBIGVerde) {
  deque<pair<__uint64_t, __uint16_t>> color_vector = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_5_BIG,0}};
  unordered_map<__uint64_t, __uint64_t> Deleteitems;
  deque<pair<__uint64_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_5_BIG,0}};

  int const result = Image::cf_check_and_delete_BIG(color_vector, 1, Deleteitems, bluevalues);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(Deleteitems.size(), 1);
  EXPECT_EQ(Deleteitems.count(NUMERO_BIN_1_BIG), 1);
}

TEST_F(ImageTest, CfCheckAndDeleteBIGTresAzul) {
  deque<pair<__uint64_t, __uint16_t>> color_vector = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_4_BIG, 0}, {NUMERO_BIN_5_BIG, 0}};
  unordered_map<__uint64_t, __uint64_t> Deleteitems;
  deque<pair<__uint64_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1_BIG, 1}, {NUMERO_BIN_4_BIG, 0}, {NUMERO_BIN_5_BIG, 0}};

  int const result = Image::cf_check_and_delete_BIG(color_vector, 1, Deleteitems, bluevalues);

  EXPECT_EQ(result, 3);
  EXPECT_EQ(Deleteitems.size(), 0);
}

TEST_F(ImageTest, CfCheckAndDeleteBIGTresVerde) {
  deque<pair<__uint64_t, __uint16_t>> color_vector = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_4_BIG, 0}, {NUMERO_BIN_6_BIG, 0}};
  unordered_map<__uint64_t, __uint64_t> Deleteitems;
  deque<pair<__uint64_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1_BIG, 1}, {NUMERO_BIN_4_BIG, 0}, {NUMERO_BIN_6_BIG, 0}};

  int const result = Image::cf_check_and_delete_BIG(color_vector, 1, Deleteitems, bluevalues);

  EXPECT_EQ(result, 3);
  EXPECT_EQ(Deleteitems.size(), 0);
}
/*
TEST_F(ImageTest, CfDeleteFromDequeBIG_ValidIndex) {
  deque<pair<__uint64_t, __uint16_t>> deque_general = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 1}, {NUMERO_BIN_3_BIG, 2}};
  constexpr size_t index = 1;

  Image::cf_delete_from_deque_BIG(deque_general, index);

  ASSERT_EQ(deque_general.size(), 2);
  EXPECT_EQ(deque_general[0].first, 148868987696450);
  EXPECT_EQ(deque_general[1].first, 0);
}

TEST_F(ImageTest, CfDeleteFromDequeBIG_FirstIndex) {
  deque<pair<__uint64_t, __uint16_t>> deque_general = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 1}, {NUMERO_BIN_3_BIG, 2}};
  size_t index = 0;

  getImage()->cf_delete_from_deque_BIG(deque_general, index);

  ASSERT_EQ(deque_general.size(), 2);
  EXPECT_EQ(deque_general[0].first, NUMERO_BIN_2_BIG);
  EXPECT_EQ(deque_general[1].first, NUMERO_BIN_3_BIG);
}

TEST_F(ImageTest, CfDeleteFromDequeBIG_LastIndex) {
  deque<pair<__uint64_t, __uint16_t>> deque_general = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 1}, {NUMERO_BIN_3_BIG, 2}};
  size_t index = 2;

  getImage()->cf_delete_from_deque_BIG(deque_general, index);

  ASSERT_EQ(deque_general.size(), 2);
  EXPECT_EQ(deque_general[0].first, NUMERO_BIN_3_BIG);
  EXPECT_EQ(deque_general[1].first, NUMERO_BIN_2_BIG);
}

TEST_F(ImageTest, CfDeleteFromDequeBIG_InvalidIndex) {
  deque<pair<__uint64_t, __uint16_t>> deque_general = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 1}, {NUMERO_BIN_3_BIG, 2}};
  size_t index = 3;

  EXPECT_THROW({
    getImage()->cf_delete_from_deque_BIG(deque_general, index);
  }, std::out_of_range);
}
*/