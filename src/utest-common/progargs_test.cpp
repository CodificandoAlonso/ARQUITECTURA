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
  EXPECT_TRUE(getImage()->info_constraints(static_cast<int>(getImage()->getArgv().size())));}

TEST_F(ImageTest, InfoConstraintsArgumentosSuperiorAlPermitido) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "info", "extra"});
  SetUp();
  EXPECT_TRUE(getImage()->info_constraints(static_cast<int>(getImage()->getArgv().size())));}

TEST_F(ImageTest, MaxValConstraintsCorrecto) {
  setArgv({"progname", "deer-small.ppm", "deer-small.ppm", "maxlevel", "857"});
  SetUp();
  EXPECT_FALSE(getImage()->maxval_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, MaxValConstraintsNumeroArgumentosIncorrecto) {
  setArgv({"progname", "deer-small.ppm", "deer-small.ppm", "maxlevel"});
  EXPECT_TRUE(getImage()->maxval_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, MaxValConstraintsNumeroArgumentosCorrectosNumeroNoEntre0Y65535) {
  setArgv({"progname", "deer-small.ppm", "deer-small.ppm", "maxlevel", "65536"});
  SetUp();
  EXPECT_TRUE(getImage()->maxval_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, ResizeConstraintsCorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "resize", "100", "100"});
  SetUp();
  EXPECT_FALSE(getImage()->resize_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, ResizeConstraintsNumeroArgumentosIncorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "resize", "100"});
  SetUp();
  EXPECT_TRUE(getImage()->resize_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, ResizeConstraintsInvalidWidth) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "resize", "-100", "100"});
  EXPECT_TRUE(getImage()->resize_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, ResizeConstraintsInvalidHeigth) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "resize", "100", "-100"});
  EXPECT_TRUE(getImage()->resize_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, CutfreqConstraintsCorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "cutfreq", "100"});
  SetUp();
  EXPECT_FALSE(getImage()->cutfreq_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, CutfreqConstraintsNumeroArgumentosIncorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "cutfreq"});
  SetUp();
  EXPECT_TRUE(getImage()->cutfreq_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest,  CutfreqConstraintsCutfreqInvalido) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.ppm", "cutfreq", "-100"});
  SetUp();
  EXPECT_TRUE(getImage()->cutfreq_constraints(static_cast<int>(getImage()->getArgv().size()), getImage()->getArgv()));
}

TEST_F(ImageTest, CompressConstraintsCorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.cppm", "compress"});;
  SetUp();
  EXPECT_FALSE(getImage()->compress_constraints(static_cast<int>(getImage()->getArgv().size())));
}

TEST_F(ImageTest, CompressConstraintsNumeroArgumentosIncorrecto) {
  setArgv({"imtool-soa", "deer-small.ppm", "deer-small.cppm", "compress", "extra"});
  SetUp();
  EXPECT_TRUE(getImage()->compress_constraints(static_cast<int>(getImage()->getArgv().size())));
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

TEST_F(ImageTest, CheckArgsOpcionInvalida) {
  setArgv({"imtool-soa", "input.ppm", "output.ppm", "incorrect"});
  SetUp();
  EXPECT_EQ(getImage()->check_args(), -1);
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

TEST_F(ImageTest, CfDeleteFromDequeBIG_Correcto) {
  deque<pair<__uint64_t, __uint16_t>> deque_general = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 0}, {NUMERO_BIN_3_BIG, 0}};
  constexpr size_t index = 1;

  Image::cf_delete_from_deque_BIG(deque_general, index);

  ASSERT_EQ(deque_general.size(), 2);
  EXPECT_EQ(deque_general[0].first, 188900680086377);
  EXPECT_EQ(deque_general[1].first, 188900966509364);
}

TEST_F(ImageTest, CfDeleteFromDequeBIG_Incorrecto) {
  deque<pair<__uint64_t, __uint16_t>> deque_general = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 0}, {NUMERO_BIN_3_BIG, 0}};
  constexpr size_t index = 1;

  Image::cf_delete_from_deque_BIG(deque_general, index);

  ASSERT_EQ(deque_general.size(), 2);
  EXPECT_NE(deque_general[0].first, 0);
  EXPECT_NE(deque_general[1].first, 0);
}

TEST_F(ImageTest, CfDeleteFromDeque_Correcto) {
  deque<pair<__uint32_t, __uint16_t>> deque_general = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_2, 0}, {NUMERO_BIN_3, 0}};
  constexpr size_t index = 1;

  Image::cf_delete_from_deque(deque_general, index);

  ASSERT_EQ(deque_general.size(), 2);
  EXPECT_EQ(deque_general[0].first, 1193046);
  EXPECT_EQ(deque_general[1].first, 11259375);
}

TEST_F(ImageTest, CfDeleteFromDeque_Incorrecto) {
  deque<pair<__uint32_t, __uint16_t>> deque_general = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_2, 0}, {NUMERO_BIN_3, 0}};
  constexpr size_t index = 1;

  Image::cf_delete_from_deque(deque_general, index);

  ASSERT_EQ(deque_general.size(), 2);
  EXPECT_NE(deque_general[0].first, 0);
  EXPECT_NE(deque_general[1].first, 0);
}

TEST_F(ImageTest, CfSearchInBlueBIG_Encontrado) {
  deque<pair<__uint64_t, unsigned short>> pairs = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 1}, {NUMERO_BIN_3_BIG, 2}};
  __uint64_t first = NUMERO_BIN_2_BIG;

  size_t const result = Image::cf_search_in_blue_BIG(pairs, first);

  EXPECT_EQ(result, 1);
}

TEST_F(ImageTest, CfSearchInBlueBIG_NoEncontrado) {
  deque<pair<__uint64_t, unsigned short>> pairs = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 1}, {NUMERO_BIN_3_BIG, 2}};
  __uint64_t first = NUMERO_BIN_4_BIG;

  size_t const result = Image::cf_search_in_blue_BIG(pairs, first);

  EXPECT_EQ(result, 0);
}

TEST_F(ImageTest, CfSearchInBlueBIG_ColaVacía) {
  deque<pair<__uint64_t, unsigned short>> pairs;
  __uint64_t first = NUMERO_BIN_1_BIG;

  size_t const result = Image::cf_search_in_blue_BIG(pairs, first);

  EXPECT_EQ(result, 0);
}

TEST_F(ImageTest, CfSearchInBlue_Encontrado) {
  deque<pair<__uint64_t, unsigned short>> pairs = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 1}, {NUMERO_BIN_3_BIG, 2}};
  __uint64_t first = NUMERO_BIN_2_BIG;

  size_t const result = Image::cf_search_in_blue_BIG(pairs, first);

  EXPECT_EQ(result, 1);
}

TEST_F(ImageTest, CfSearchInBlue_NoEncontrado) {
  deque<pair<__uint64_t, unsigned short>> pairs = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 1}, {NUMERO_BIN_3_BIG, 2}};
  __uint64_t first = NUMERO_BIN_4_BIG;

  size_t const result = Image::cf_search_in_blue_BIG(pairs, first);

  EXPECT_EQ(result, 0);
}

TEST_F(ImageTest, CfSearchInBlue_ColaVacia) {
  deque<pair<__uint64_t, unsigned short>> pairs;
  __uint64_t first = NUMERO_BIN_1_BIG;

  size_t const result = Image::cf_search_in_blue_BIG(pairs, first);

  EXPECT_EQ(result, 0);
}

TEST_F(ImageTest, CfDeleteAndRest_IndiceValido) {
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  int num_left = 3;
  deque<pair<__uint32_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_2, 1}, {NUMERO_BIN_3, 2}};
  constexpr size_t index = 1;

  Image::cf_delete_and_rest(Deleteitems, num_left, bluevalues, index);

  EXPECT_EQ(Deleteitems[NUMERO_BIN_2], 0);
  EXPECT_EQ(bluevalues.size(), 2);
  EXPECT_EQ(num_left, 2);
}

TEST_F(ImageTest, CfDeleteAndRest_PrimerIndice) {
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  int num_left = 3;
  deque<pair<__uint32_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_2, 1}, {NUMERO_BIN_3, 2}};
  constexpr size_t index = 0;

  Image::cf_delete_and_rest(Deleteitems, num_left, bluevalues, index);

  EXPECT_EQ(Deleteitems[NUMERO_BIN_1], 0);
  EXPECT_EQ(bluevalues.size(), 2);
  EXPECT_EQ(num_left, 2);
}

TEST_F(ImageTest, CfDeleteAndRest_UltimoIndice) {
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  int num_left = 3;
  deque<pair<__uint32_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_2, 1}, {NUMERO_BIN_3, 2}};
  constexpr size_t index = 2;

  Image::cf_delete_and_rest(Deleteitems, num_left, bluevalues, index);

  EXPECT_EQ(Deleteitems[NUMERO_BIN_3], 0);
  EXPECT_EQ(bluevalues.size(), 2);
  EXPECT_EQ(num_left, 2);
}

TEST_F(ImageTest, CfCheckJustBlue_TresAzulesDistintos) {
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  int num_left = 3;
  deque<pair<__uint32_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_2, 0}, {NUMERO_BIN_3, 0}};

  int const result = Image::cf_check_just_blue(Deleteitems, bluevalues, num_left);

  EXPECT_EQ(result, 4);
  EXPECT_EQ(Deleteitems.size(), 0);
  EXPECT_EQ(bluevalues.size(), 3);
  EXPECT_EQ(num_left, 3);
}

TEST_F(ImageTest, CfCheckJustBlue_TresAzulesIguales) {
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  int num_left = 3;
  deque<pair<__uint32_t, __uint16_t>> bluevalues = {{NUMERO_BIN_5, 0}, {NUMERO_BIN_4, 0}, {NUMERO_BIN_6, 0}};

  int const result = Image::cf_check_just_blue(Deleteitems, bluevalues, num_left);

  EXPECT_EQ(result, 4);
  EXPECT_EQ(Deleteitems.size(), 0);
  EXPECT_EQ(bluevalues.size(), 3);
  EXPECT_EQ(num_left, 3);
}

TEST_F(ImageTest, CfDeleteFirstBlueValueCorrecto) {
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  int num_left = 3;
  deque<pair<__uint32_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_2, 0}, {NUMERO_BIN_3, 0}};

  Image::cf_delete_first_blue_value(Deleteitems, num_left, bluevalues);

  EXPECT_EQ(Deleteitems[NUMERO_BIN_1], 0);
  EXPECT_EQ(bluevalues.size(), 2);
  EXPECT_EQ(num_left, 2);
}

TEST_F(ImageTest, CfDeleteFirstBlueValue1Elemento) {
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  int num_left = 1;
  deque<pair<__uint32_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1, 0}};

  Image::cf_delete_first_blue_value(Deleteitems, num_left, bluevalues);

  EXPECT_EQ(Deleteitems[NUMERO_BIN_1], 0);
  EXPECT_EQ(bluevalues.size(), 0);
  EXPECT_EQ(num_left, 0);
}

TEST_F(ImageTest, CfDeleteFirstBlueValueBIGCorrecto) {
  unordered_map<__uint64_t, __uint64_t> Deleteitems;
  int num_left = 3;
  deque<pair<__uint64_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 0}, {NUMERO_BIN_3_BIG, 0}};

  Image::cf_delete_first_blue_value_BIG(Deleteitems, num_left, bluevalues);

  EXPECT_EQ(Deleteitems[NUMERO_BIN_1_BIG], 0);
  EXPECT_EQ(bluevalues.size(), 2);
  EXPECT_EQ(num_left, 2);
}

TEST_F(ImageTest, CfDeleteFirstBlueValueBIG1Elemento) {
  unordered_map<__uint64_t, __uint64_t> Deleteitems;
  int num_left = 1;
  deque<pair<__uint64_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1_BIG, 0}};

  Image::cf_delete_first_blue_value_BIG(Deleteitems, num_left, bluevalues);

  EXPECT_EQ(Deleteitems[NUMERO_BIN_1_BIG], 0);
  EXPECT_EQ(bluevalues.size(), 0);
  EXPECT_EQ(num_left, 0);
}

TEST_F(ImageTest, CfEqualBlueCaseCorrecto) {
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  int num_left = 3;
  deque<pair<__uint32_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_4, 0}, {NUMERO_BIN_6, 0}};
  int my_meanwhile = 2;
  constexpr size_t my_index = 0;

  params_equal_blu params = {
    .Deleteitems = &Deleteitems,
    .num_left = &num_left,
    .bluevalues = &bluevalues,
    .my_index = my_index,
    .my_meanwhile = &my_meanwhile
  };

  Image::cf_equal_blue_case(&params);

  EXPECT_EQ(bluevalues.size(), 0);
  EXPECT_EQ(num_left, 0);
}

TEST_F(ImageTest, CfEqualBlueCaseBIGCorrecto) {
  unordered_map<__uint64_t, __uint64_t> Deleteitems;
  int num_left = 3;
  deque<pair<__uint64_t, __uint16_t>> bluevalues = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_4_BIG, 0}, {NUMERO_BIN_6_BIG, 0}};
  int my_meanwhile = 2;
  constexpr size_t my_index = 0;

  params_equal_blu_BIG params = {
    .Deleteitems = &Deleteitems,
    .num_left = &num_left,
    .bluevalues = &bluevalues,
    .my_index = my_index,
    .my_meanwhile = &my_meanwhile
  };

  Image::cf_equal_blue_case_BIG(&params);

  EXPECT_EQ(bluevalues.size(), 2);
  EXPECT_EQ(num_left, 2);
}

TEST_F(ImageTest, CfCheckColorsToDeleteCorrecto) {
  unordered_map<__uint32_t, __uint32_t> const Deleteitems;
  int const num_left = 3;
  deque<pair<__uint32_t, __uint16_t>> const bluevalues = {{NUMERO_BIN_1, 0}, {NUMERO_BIN_2, 0}, {NUMERO_BIN_3, 0}};

  auto result = Image::cf_check_colors_to_delete(Deleteitems, num_left, bluevalues);

  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result[NUMERO_BIN_1], 0);
  EXPECT_EQ(result[NUMERO_BIN_2], 0);
  EXPECT_EQ(result[NUMERO_BIN_3], 0);
  EXPECT_EQ(bluevalues.size(), 3);
  EXPECT_EQ(num_left, 3);
}

TEST_F(ImageTest, CfCheckColorsToDelete1Elemento) {
  unordered_map<__uint32_t, __uint32_t> const Deleteitems;
  constexpr int num_left = 1;
  deque<pair<__uint32_t, __uint16_t>> const bluevalues = {{NUMERO_BIN_1, 0}};

  auto result = Image::cf_check_colors_to_delete(Deleteitems, num_left, bluevalues);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result[NUMERO_BIN_1], 0);
  EXPECT_EQ(bluevalues.size(), 1);
  EXPECT_EQ(num_left, 1);
}

TEST_F(ImageTest, CfCheckColorsToDeleteBIGCorrecto) {
  unordered_map<__uint64_t, __uint64_t> const Deleteitems;
  int const num_left = 3;
  deque<pair<__uint64_t, __uint16_t>> const bluevalues = {{NUMERO_BIN_1_BIG, 0}, {NUMERO_BIN_2_BIG, 0}, {NUMERO_BIN_3_BIG, 0}};

  auto result = Image::cf_check_colors_to_delete_BIG(Deleteitems, num_left, bluevalues);

  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result[NUMERO_BIN_1_BIG], 0);
  EXPECT_EQ(result[NUMERO_BIN_2_BIG], 0);
  EXPECT_EQ(result[NUMERO_BIN_3_BIG], 0);
  EXPECT_EQ(bluevalues.size(), 3);
  EXPECT_EQ(num_left, 3);
}

TEST_F(ImageTest, CfCheckColorsToDeleteBIG1Elemento) {
  unordered_map<__uint64_t, __uint64_t> const Deleteitems;
  constexpr int num_left = 1;
  deque<pair<__uint64_t, __uint16_t>> const bluevalues = {{NUMERO_BIN_1_BIG, 0}};

  auto result = Image::cf_check_colors_to_delete_BIG(Deleteitems, num_left, bluevalues);

  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result[NUMERO_BIN_1_BIG], 0);
  EXPECT_EQ(bluevalues.size(), 1);
  EXPECT_EQ(num_left, 1);
}

TEST_F(ImageTest, CfFindClosestInNeighborsCorrecto) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> const graph = {
    {1, {{2, 3}, {4, 5}}},
    {2, {{1, 3}, {6, 7}}},
    {3, {{1, 2}, {8, 9}}},
    {4, {{1}, {10}}},
    {5, {{1}, {11}}},
    {6, {{2}, {12}}},
    {7, {{2}, {13}}},
    {8, {{3}, {14}}},
    {9, {{3}, {15}}}
  };
  unordered_map<__uint32_t, __uint8_t> visited_node;
  vector<__uint32_t> const neighbors = {1};
  double min_distance = numeric_limits<double>::max();
  constexpr __uint32_t color_to_delete = 5;

  cf_find_neigh_small const params = {
    .color_to_delete = color_to_delete,
    .graph = &graph,
    .neighbors = &neighbors,
    .min_distance = &min_distance,
    .visited_node = &visited_node
  };

  __uint32_t const result = Image::cf_find_closest_in_neighbors(&params);

  EXPECT_EQ(result, 5);
  EXPECT_LT(min_distance, numeric_limits<double>::max());
}

TEST_F(ImageTest, CfFindClosestInNeighborsVariosCandidatos) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> const graph = {
    {1, {{2, 3}, {4, 5}}},
    {2, {{1, 3}, {6, 7}}},
    {3, {{1, 2}, {8, 9}}},
    {4, {{1}, {10}}},
    {5, {{1}, {11}}},
    {6, {{2}, {12}}},
    {7, {{2}, {13}}},
    {8, {{3}, {14}}},
    {9, {{3}, {15}}},
    {10, {{4}, {16}}},
    {11, {{5}, {17}}}
  };
  unordered_map<__uint32_t, __uint8_t> visited_node;
  vector<__uint32_t> const neighbors = {1};
  double min_distance = numeric_limits<double>::max();
  constexpr __uint32_t color_to_delete = 5;

  cf_find_neigh_small const params = {
    .color_to_delete = color_to_delete,
    .graph = &graph,
    .neighbors = &neighbors,
    .min_distance = &min_distance,
    .visited_node = &visited_node
  };

  __uint32_t const result = Image::cf_find_closest_in_neighbors(&params);

  EXPECT_EQ(result, 5);
  EXPECT_LT(min_distance, numeric_limits<double>::max());
}

TEST_F(ImageTest, CfFindClosestInNeighborsBIGCorrecto) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> const graph = {
    {1, {{2, 3}, {4, 5}}},
    {2, {{1, 3}, {6, 7}}},
    {3, {{1, 2}, {8, 9}}},
    {4, {{1}, {10}}},
    {5, {{1}, {11}}},
    {6, {{2}, {12}}},
    {7, {{2}, {13}}},
    {8, {{3}, {14}}},
    {9, {{3}, {15}}}
  };
  unordered_map<__uint64_t, __uint8_t> visited_node;
  vector<__uint64_t> const neighbors = {1};
  double min_distance = numeric_limits<double>::max();
  constexpr __uint64_t color_to_delete = 5;

  cf_find_neigh_BIG const params = {
    .color_to_delete = color_to_delete,
    .graph = &graph,
    .neighbors = &neighbors,
    .min_distance = &min_distance,
    .visited_node = &visited_node
  };

  __uint64_t const result = Image::cf_find_closest_in_neighbors_BIG(&params);

  EXPECT_EQ(result, 5);
  EXPECT_LT(min_distance, numeric_limits<double>::max());
}

TEST_F(ImageTest, CfFindClosestInNeighborsBIGVariosCandidatos) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> const graph = {
    {1, {{2, 3}, {4, 5}}},
    {2, {{1, 3}, {6, 7}}},
    {3, {{1, 2}, {8, 9}}},
    {4, {{1}, {10}}},
    {5, {{1}, {11}}},
    {6, {{2}, {12}}},
    {7, {{2}, {13}}},
    {8, {{3}, {14}}},
    {9, {{3}, {15}}},
    {10, {{4}, {16}}},
    {11, {{5}, {17}}}
  };
  unordered_map<__uint64_t, __uint8_t> visited_node;
  vector<__uint64_t> const neighbors = {1};
  double min_distance = numeric_limits<double>::max();
  constexpr __uint64_t color_to_delete = 5;

  cf_find_neigh_BIG const params = {
    .color_to_delete = color_to_delete,
    .graph = &graph,
    .neighbors = &neighbors,
    .min_distance = &min_distance,
    .visited_node = &visited_node
  };

  __uint64_t const result = Image::cf_find_closest_in_neighbors_BIG(&params);

  EXPECT_EQ(result, 5);
  EXPECT_LT(min_distance, numeric_limits<double>::max());
}

TEST_F(ImageTest, CfFinishGraphCorrecto) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph = {
    {1, {{2, 3}, {}}},
    {2, {{1, 3}, {}}},
    {3, {{1, 2}, {}}}
  };
  unordered_map<__uint32_t, __uint32_t> Deleteitems;
  unordered_map<__uint32_t, __uint32_t> toSave;
  unordered_map<__uint32_t, __uint16_t> const myMap = {
    {4, 0},
    {5, 0}
  };

  params_finish_graph const params = {
    .myMap = &myMap,
    .Deleteitems = &Deleteitems,
    .toSave = &toSave,
    .graph = &graph
  };

  Image::cf_finish_graph(&params);

  EXPECT_EQ(graph[1].second.size(), 0);
  EXPECT_EQ(graph[2].second.size(), 0);
  EXPECT_EQ(graph[3].second.size(), 2);
  EXPECT_EQ(Deleteitems.size(), 0);
  EXPECT_EQ(toSave.size(), 2);
  EXPECT_EQ(toSave[4], 3);
  EXPECT_EQ(toSave[5], 3);
}

TEST_F(ImageTest, CfFinishGraphConElementosParaEliminar) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph = {
    {1, {{2, 3}, {}}},
    {2, {{1, 3}, {}}},
    {3, {{1, 2}, {}}}
  };
  unordered_map<__uint32_t, __uint32_t> Deleteitems = {
    {4, 0}
  };
  unordered_map<__uint32_t, __uint32_t> toSave;
  unordered_map<__uint32_t, __uint16_t> const myMap = {
    {4, 0},
    {5, 0}
  };

  params_finish_graph const params = {
    .myMap = &myMap,
    .Deleteitems = &Deleteitems,
    .toSave = &toSave,
    .graph = &graph
  };

  Image::cf_finish_graph(&params);

  EXPECT_EQ(graph[1].second.size(), 0);
  EXPECT_EQ(graph[2].second.size(), 0);
  EXPECT_EQ(graph[3].second.size(), 1);
  EXPECT_EQ(graph[3].second[0], 5);
  EXPECT_EQ(Deleteitems.size(), 1);
  EXPECT_EQ(Deleteitems[4], 3);
  EXPECT_EQ(toSave.size(), 1);
  EXPECT_EQ(toSave[5], 3);
}

TEST_F(ImageTest, CfFinishGraphBIGCorrecto) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph = {
    {1, {{2, 3}, {}}},
    {2, {{1, 3}, {}}},
    {3, {{1, 2}, {}}}
  };
  unordered_map<__uint64_t, __uint64_t> Deleteitems;
  unordered_map<__uint64_t, __uint64_t> toSave;
  unordered_map<__uint64_t, __uint16_t> const myMap = {
    {4, 0},
    {5, 0}
  };

  params_finish_graph_BIG const params = {
    .myMap = &myMap,
    .Deleteitems = &Deleteitems,
    .toSave = &toSave,
    .graph = &graph
  };

  Image::cf_finish_graph_BIG(&params);

  EXPECT_EQ(graph[1].second.size(), 0);
  EXPECT_EQ(graph[2].second.size(), 0);
  EXPECT_EQ(graph[3].second.size(), 2);
  EXPECT_EQ(Deleteitems.size(), 0);
  EXPECT_EQ(toSave.size(), 2);
  EXPECT_EQ(toSave[4], 3);
  EXPECT_EQ(toSave[5], 3);
}

TEST_F(ImageTest, CfFinishGraphBIGConElementosParaEliminar) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph = {
    {1, {{2, 3}, {}}},
    {2, {{1, 3}, {}}},
    {3, {{1, 2}, {}}}
  };
  unordered_map<__uint64_t, __uint64_t> Deleteitems = {
    {4, 0}
  };
  unordered_map<__uint64_t, __uint64_t> toSave;
  unordered_map<__uint64_t, __uint16_t> const myMap = {
    {4, 0},
    {5, 0}
  };

  params_finish_graph_BIG const params = {
    .myMap = &myMap,
    .Deleteitems = &Deleteitems,
    .toSave = &toSave,
    .graph = &graph
  };

  Image::cf_finish_graph_BIG(&params);

  EXPECT_EQ(graph[1].second.size(), 0);
  EXPECT_EQ(graph[2].second.size(), 0);
  EXPECT_EQ(graph[3].second.size(), 1);
  EXPECT_EQ(graph[3].second[0], 5);
  EXPECT_EQ(Deleteitems.size(), 1);
  EXPECT_EQ(Deleteitems[4], 3);
  EXPECT_EQ(toSave.size(), 1);
  EXPECT_EQ(toSave[5], 3);
}


