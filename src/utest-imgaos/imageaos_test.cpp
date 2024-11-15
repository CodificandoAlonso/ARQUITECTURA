//
// Created by hector on 2/11/24.
//
#include "common/binario.hpp"
#include "imgaos/imageaos.hpp"  // Ensure the correct path to the header file

#include <array>
#include <cstdio>
#include <fstream>
#include <gsl/gsl>
#include <gtest/gtest.h>
#include <string>
#include <unordered_map>
#include <vector>

static constexpr int NUM_100  = 100;
static constexpr int MNUM_100 = -100;
static constexpr int FOTO     = 256;
static constexpr int NUM_5    = 5;
static constexpr int NUM_6    = 6;
static constexpr int NUM_7    = 7;
static constexpr int NUM_8    = 8;
static constexpr int NUM_9    = 9;
static constexpr int NUM_10   = 10;

static constexpr int NUM_15   = 15;
static constexpr int NUM_16   = 16;
static constexpr int NUM_17   = 17;
static constexpr int NUM_18   = 18;
static constexpr int NUM_19   = 19;
static constexpr int NUM_20   = 20;
static constexpr int NUM_21   = 21;
static constexpr int NUM_22   = 22;
static constexpr int NUM_23   = 23;
static constexpr int NUM_24   = 24;
static constexpr int NUM_25   = 25;
static constexpr int NUM_26   = 26;
static constexpr int NUM_27   = 27;
static constexpr int NUM_28   = 28;
static constexpr int NUM_29   = 29;
static constexpr int NUM_30   = 30;
static constexpr int NUM_40   = 40;
static constexpr int NUM_50   = 50;
static constexpr int NUM_60   = 60;
static constexpr int NUM_70   = 70;
static constexpr int NUM_80   = 80;
static constexpr int NUM_90   = 90;
static constexpr int NUM_110  = 110;
static constexpr int NUM_120  = 120;
static constexpr int NUM_130  = 130;
static constexpr int NUM_140  = 140;
static constexpr int NUM_150  = 150;
static constexpr int NUM_160  = 160;
static constexpr int NUM_170  = 170;
static constexpr int NUM_180  = 180;
static constexpr int NUM_M75  = -75;
static constexpr int NUM_M150 = -150;
static constexpr int NUM_M240 = -240;
static constexpr int NUM_1000 = 1000;
static constexpr int NUM_2000 = 2000;
static constexpr int NUM_3000 = 3000;
static constexpr int NUM_4000 = 4000;
static constexpr int NUM_5000 = 5000;
static constexpr int NUM_6000 = 6000;
static constexpr int NUM_7000 = 7000;
static constexpr int NUM_8000 = 8000;
static constexpr int NUM_9000 = 9000;
static constexpr int NUM_75   = 75;
static constexpr int NUM_240  = 240;

class ImageAOSTest : public ::testing::Test {
  private:
    gsl::owner<ImageAOS *> imageAOS = nullptr;
    std::string test_image_path;

  protected:
    void SetUp() override {
      std::vector<std::string> const args = {"resize", "test_image.ppm", "output_image.ppm"};
      imageAOS                            = new ImageAOS(static_cast<int>(args.size()), args);

      test_image_path = "test_image.ppm";
      std::ofstream output_file(test_image_path, std::ios::binary);
      if (!output_file) { FAIL() << "Failed to create test image file."; }
      output_file << "P6\n" << NUM_100 << " " << NUM_100 << "\n" << FOTO - 1 << "\n";

      // Write some dummy data to the input file
      for (int i = 0; i < NUM_100 * NUM_100; ++i) {
        output_file.put(static_cast<char>(i % FOTO));
        output_file.put(static_cast<char>((i + 1) % FOTO));
        output_file.put(static_cast<char>((i + 2) % FOTO));
      }
      output_file.close();
      imageAOS->cf_add_nodes();
      imageAOS->cf_add_nodes_BIG(NUM_75, NUM_150, NUM_240);
    }

    void TearDown() override {
      if (imageAOS != nullptr) {
        delete imageAOS;
        imageAOS = nullptr;
      }
      if (std::ifstream(test_image_path.c_str()).good()) {
        if (std::remove(test_image_path.c_str()) != 0) { std::perror("Error deleting file"); }
      }

      if (std::ifstream("test_image.ppm").good()) {
        if (std::remove("test_image.ppm") != 0) { std::perror("Error deleting file"); }
      }
    }

  public:
    // Default constructor
    ImageAOSTest() = default;

    // Destructor
    ~ImageAOSTest() override { delete imageAOS; }

    // Copy constructor
    ImageAOSTest(ImageAOSTest const & other)
      : imageAOS(new ImageAOS(std::move(*other.imageAOS))), test_image_path(other.test_image_path) {
    }

    // Copy assignment operator
    ImageAOSTest & operator=(ImageAOSTest const & other) {
      if (this != &other) {
        delete imageAOS;
        imageAOS        = new ImageAOS(std::move(*other.imageAOS));
        test_image_path = other.test_image_path;
      }
      return *this;
    }

    // Move constructor
    ImageAOSTest(ImageAOSTest && other) noexcept
      : imageAOS(other.imageAOS), test_image_path(std::move(other.test_image_path)) {
      other.imageAOS = nullptr;
    }

    // Move assignment operator
    ImageAOSTest & operator=(ImageAOSTest && other) noexcept {
      if (this != &other) {
        delete imageAOS;
        imageAOS        = other.imageAOS;
        test_image_path = std::move(other.test_image_path);
        other.imageAOS  = nullptr;
      }
      return *this;
    }

    [[nodiscard]] std::string const & getTestImagePath() const { return test_image_path; }

    [[nodiscard]] gsl::owner<ImageAOS *> getImageAOS() const { return imageAOS; }
};

// Test con el metodo rsz_obtain_square_min que funciona correctamente
TEST_F(ImageAOSTest, RszObtainSquareMin) {
  vector<rgb_small> const image = {
    {    .r = 0, .g = NUM_15, .b = NUM_25},
    {    .r = 1, .g = NUM_16, .b = NUM_26},
    {    .r = 2, .g = NUM_17, .b = NUM_27},
    {    .r = 3, .g = NUM_18, .b = NUM_28},
    {    .r = 4, .g = NUM_19, .b = NUM_29},
    {.r = NUM_5, .g = NUM_20,  .b = NUM_5},
    {.r = NUM_6, .g = NUM_21,  .b = NUM_6},
    {.r = NUM_7, .g = NUM_22,  .b = NUM_7},
    {.r = NUM_8, .g = NUM_23,  .b = NUM_8},
    {.r = NUM_9, .g = NUM_24,  .b = NUM_9}
  };

  std::array<unsigned int, 5> const args = {4, 5, 7, 8, 3};

  std::array<rgb_small, 4> result = ImageAOS::rsz_obtain_square_min(image, args);

  EXPECT_EQ(result[0].r, 8);
  EXPECT_EQ(result[0].g, 23);
  EXPECT_EQ(result[0].b, 8);

  EXPECT_EQ(result[1].r, 8);
  EXPECT_EQ(result[1].g, 23);
  EXPECT_EQ(result[1].b, 8);

  EXPECT_EQ(result[2].r, 8);
  EXPECT_EQ(result[2].g, 23);
  EXPECT_EQ(result[2].b, 8);

  EXPECT_EQ(result[3].r, 8);
  EXPECT_EQ(result[3].g, 23);
  EXPECT_EQ(result[3].b, 8);
}

// Test con el metodo rsz_obtain_square_min que no funciona
TEST_F(ImageAOSTest, RszObtainSquareMinFailure) {
  vector<rgb_small> const image = {
    {    .r = 0, .g = NUM_15, .b = NUM_25},
    {    .r = 1, .g = NUM_16, .b = NUM_26},
    {    .r = 2, .g = NUM_17, .b = NUM_27},
    {    .r = 3, .g = NUM_18, .b = NUM_28},
    {    .r = 4, .g = NUM_19, .b = NUM_29},
    {.r = NUM_5, .g = NUM_20,  .b = NUM_5},
    {.r = NUM_6, .g = NUM_21,  .b = NUM_6},
    {.r = NUM_7, .g = NUM_22,  .b = NUM_7},
    {.r = NUM_8, .g = NUM_23,  .b = NUM_8},
    {.r = NUM_9, .g = NUM_24,  .b = NUM_9}
  };
  /*
  image.r = {0, 1, 2, 3, 4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_10, NUM_11, NUM_12, NUM_13,
  NUM_14}; image.g = {NUM_15, NUM_16, NUM_17, NUM_18, NUM_19, NUM_20, NUM_21, NUM_22, NUM_23,
  NUM_24}; image.b = {NUM_25, NUM_26, NUM_27, NUM_28, NUM_29, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9};
  */
  std::array<unsigned int, 5> const args = {4, 5, 7, 8, 3};

  std::array<rgb_small, 4> result = ImageAOS::rsz_obtain_square_min(image, args);

  // Intentionally incorrect expected values to cause the test to fail
  EXPECT_NE(result[0].r, 1);
  EXPECT_NE(result[0].g, 11);
  EXPECT_NE(result[0].b, 21);

  EXPECT_NE(result[1].r, 2);
  EXPECT_NE(result[1].g, 12);
  EXPECT_NE(result[1].b, 22);

  EXPECT_NE(result[2].r, 3);
  EXPECT_NE(result[2].g, 13);
  EXPECT_NE(result[2].b, 23);

  EXPECT_NE(result[3].r, 4);
  EXPECT_NE(result[3].g, 14);
  EXPECT_NE(result[3].b, 24);
}

// Test con el metodo rsz_obtain_square_max que funciona correctamente
TEST_F(ImageAOSTest, RszObtainSquareMax) {
  vector<rgb_big> const image = {
    {    .r = 0, .g = NUM_15, .b = NUM_25},
    {    .r = 1, .g = NUM_16, .b = NUM_26},
    {    .r = 2, .g = NUM_17, .b = NUM_27},
    {    .r = 3, .g = NUM_18, .b = NUM_28},
    {    .r = 4, .g = NUM_19, .b = NUM_29},
    {.r = NUM_5, .g = NUM_20,  .b = NUM_5},
    {.r = NUM_6, .g = NUM_21,  .b = NUM_6},
    {.r = NUM_7, .g = NUM_22,  .b = NUM_7},
    {.r = NUM_8, .g = NUM_23,  .b = NUM_8},
    {.r = NUM_9, .g = NUM_24,  .b = NUM_9}
  };

  std::array<unsigned int, 5> const args = {4, 5, 7, 8, 3};

  std::array<rgb_big, 4> result = ImageAOS::rsz_obtain_square_max(image, args);

  EXPECT_EQ(result[0].r, 8);
  EXPECT_EQ(result[0].g, 23);
  EXPECT_EQ(result[0].b, 8);

  EXPECT_EQ(result[1].r, 8);
  EXPECT_EQ(result[1].g, 23);
  EXPECT_EQ(result[1].b, 8);

  EXPECT_EQ(result[2].r, 8);
  EXPECT_EQ(result[2].g, 23);
  EXPECT_EQ(result[2].b, 8);

  EXPECT_EQ(result[3].r, 8);
  EXPECT_EQ(result[3].g, 23);
  EXPECT_EQ(result[3].b, 8);
}

// Test con el metodo rsz_obtain_square_max que no funciona
TEST_F(ImageAOSTest, RszObtainSquareMaxFailure) {
  vector<rgb_big> const image = {
    {    .r = 0, .g = NUM_15, .b = NUM_25},
    {    .r = 1, .g = NUM_16, .b = NUM_26},
    {    .r = 2, .g = NUM_17, .b = NUM_27},
    {    .r = 3, .g = NUM_18, .b = NUM_28},
    {    .r = 4, .g = NUM_19, .b = NUM_29},
    {.r = NUM_5, .g = NUM_20,  .b = NUM_5},
    {.r = NUM_6, .g = NUM_21,  .b = NUM_6},
    {.r = NUM_7, .g = NUM_22,  .b = NUM_7},
    {.r = NUM_8, .g = NUM_23,  .b = NUM_8},
    {.r = NUM_9, .g = NUM_24,  .b = NUM_9}
  };

  std::array<unsigned int, 5> const args = {4, 5, 7, 8, 3};

  std::array<rgb_big, 4> result = ImageAOS::rsz_obtain_square_max(image, args);

  EXPECT_NE(result[0].r, 1);
  EXPECT_NE(result[0].g, 11);
  EXPECT_NE(result[0].b, 21);

  EXPECT_NE(result[1].r, 2);
  EXPECT_NE(result[1].g, 12);
  EXPECT_NE(result[1].b, 22);

  EXPECT_NE(result[2].r, 3);
  EXPECT_NE(result[2].g, 13);
  EXPECT_NE(result[2].b, 23);

  EXPECT_NE(result[3].r, 4);
  EXPECT_NE(result[3].g, 14);
  EXPECT_NE(result[3].b, 24);
}

// Test con el metodo rsz_interpolate_min que funciona correctamente
TEST_F(ImageAOSTest, RszInterpolateMinSuccess) {
  std::array<rgb_small, 4> const square = {
    rgb_small{ .r = 10,  .g = 20,  .b = 30},
    rgb_small{ .r = 40,  .g = 50,  .b = 60},
    rgb_small{ .r = 70,  .g = 80,  .b = 90},
    rgb_small{.r = 100, .g = 110, .b = 120}
  };

  double const u_param = 0.5;
  double const t_param = 0.5;

  rgb_small const result = ImageAOS::rsz_interpolate_min(u_param, square, t_param);

  EXPECT_EQ(result.r, 55);
  EXPECT_EQ(result.g, 65);
  EXPECT_EQ(result.b, 75);
}

// Test con el metodo rsz_interpolate_min que no funciona
TEST_F(ImageAOSTest, RszInterpolateMinFailure) {
  std::array<rgb_small, 4> const square = {
    rgb_small{ .r = 10,  .g = 20,  .b = 30},
    rgb_small{ .r = 40,  .g = 50,  .b = 60},
    rgb_small{ .r = 70,  .g = 80,  .b = 90},
    rgb_small{.r = 100, .g = 110, .b = 120}
  };

  double const u_param = 0.5;
  double const t_param = 0.5;

  rgb_small const result = ImageAOS::rsz_interpolate_min(u_param, square, t_param);

  // Intentionally incorrect expected values to cause the test to fail
  EXPECT_NE(result.r, 10);
  EXPECT_NE(result.g, 20);
  EXPECT_NE(result.b, 30);
}

// Test con el metodo rsz_interpolate_max que funciona correctamente
TEST_F(ImageAOSTest, RszInterpolateMaxSuccess) {
  std::array<rgb_big, 4> const square = {
    rgb_big{ .r = 10,  .g = 20,  .b = 30},
    rgb_big{ .r = 40,  .g = 50,  .b = 60},
    rgb_big{ .r = 70,  .g = 80,  .b = 90},
    rgb_big{.r = 100, .g = 110, .b = 120}
  };

  double const u_param = 0.5;
  double const t_param = 0.5;

  rgb_big const result = ImageAOS::rsz_interpolate_max(u_param, square, t_param);

  EXPECT_EQ(result.r, 55);
  EXPECT_EQ(result.g, 65);
  EXPECT_EQ(result.b, 75);
}

// Test con el metodo rsz_interpolate_max que no funciona por un error de acceso a memoria
TEST_F(ImageAOSTest, RszInterpolateMaxOutOfBounds) {
  std::array<rgb_big, 4> const square = {
    rgb_big{.r = 10, .g = 20, .b = 30},
    rgb_big{.r = 40, .g = 50, .b = 60},
    rgb_big{.r = 70, .g = 80, .b = 90},
    rgb_big{ .r = 0,  .g = 0,  .b = 0}  // Adding a dummy element to make it 4 elements
  };

  double const u_param = 0.5;
  double const t_param = 0.5;

  // This should not cause an out-of-bounds access now
  rgb_big const result = ImageAOS::rsz_interpolate_max(u_param, square, t_param);

  // Check for incorrect results due to invalid array size
  EXPECT_NE(result.r, 55);
  EXPECT_NE(result.g, 65);
  EXPECT_NE(result.b, 75);
}

// Test con el metodo rsz_interpolate_max que no funciona por un valor de u_param invalido
TEST_F(ImageAOSTest, RszInterpolateMaxInvalidUParam) {
  std::array<rgb_big, 4> const square = {
    rgb_big{ .r = 10,  .g = 20,  .b = 30},
    rgb_big{ .r = 40,  .g = 50,  .b = 60},
    rgb_big{ .r = 70,  .g = 80,  .b = 90},
    rgb_big{.r = 100, .g = 110, .b = 120}
  };

  constexpr double u_param = 1.5;  // Invalid value
  constexpr double t_param = 0.5;

  rgb_big const result = ImageAOS::rsz_interpolate_max(u_param, square, t_param);

  // Check for incorrect results due to invalid u_param
  EXPECT_NE(result.r, 55);
  EXPECT_NE(result.g, 65);
  EXPECT_NE(result.b, 75);
}

// Test con el methods rsz_interpolate_max que no function por un valor de t_param invalid
TEST_F(ImageAOSTest, RszInterpolateMaxInvalidTParam) {
  constexpr std::array<rgb_big, 4> square = {
    rgb_big{ .r = 10,  .g = 20,  .b = 30},
    rgb_big{ .r = 40,  .g = 50,  .b = 60},
    rgb_big{ .r = 70,  .g = 80,  .b = 90},
    rgb_big{.r = 100, .g = 110, .b = 120}
  };

  constexpr double u_param = 0.5;
  constexpr double t_param = 1.5;  // Invalid value

  rgb_big const result = ImageAOS::rsz_interpolate_max(u_param, square, t_param);

  // Check for incorrect results due to invalid t_param
  EXPECT_NE(result.r, 55);
  EXPECT_NE(result.g, 65);
  EXPECT_NE(result.b, 75);
}

TEST_F(ImageAOSTest, ReadImageRGBSmallSuccess) {
  std::ifstream input_file(getTestImagePath(), std::ios::binary);
  ASSERT_TRUE(input_file.is_open());

  vector<rgb_small> const result = getImageAOS()->read_image_rgb_small(input_file);

  // Add assertions to check the correctness of the result
  EXPECT_EQ(result.size(), getImageAOS()->get_width() * getImageAOS()->get_height());
}

TEST_F(ImageAOSTest, ReadImageRGBSmallFileNotFound) {
  std::ifstream input_file("non_existent_file.rgb", std::ios::binary);
  ASSERT_FALSE(input_file.is_open());
}

TEST_F(ImageAOSTest, ReadImageRGBSmallCorruptData) {
  std::ifstream input_file("corrupt_image.rgb", std::ios::binary);
  ASSERT_TRUE(input_file.is_open());

  vector<rgb_small> const result = getImageAOS()->read_image_rgb_small(input_file);

  // Add assertions to check for corrupt data
  for (auto const & iter : result) {
    EXPECT_GE(iter.r, 0);
    EXPECT_LE(iter.r, 255);
    EXPECT_GE(iter.g, 0);
    EXPECT_LE(iter.g, 255);
    EXPECT_GE(iter.b, 0);
    EXPECT_LE(iter.b, 255);
  }
}

// Test con el metodo read_image_rgb_big que funciona correctamente
TEST_F(ImageAOSTest, ReadImageRGBBig_Success) {
  std::ifstream input_file("test_image.ppm", std::ios::binary);
  ASSERT_TRUE(input_file.is_open());

  ImageAOS const ImageAOS(0, {});
  vector<rgb_big> const image = ImageAOS.read_image_rgb_big(input_file);

  ASSERT_EQ(image.size(), ImageAOS.get_width() * ImageAOS.get_height());

  input_file.close();
}

// Test con el metodo read_image_rgb_big que no funciona porque no se puede abrir un archivo
TEST_F(ImageAOSTest, ReadImageRGBBig_FileNotOpen) {
  std::ifstream input_file("non_existent_file.rgb", std::ios::binary);
  ASSERT_FALSE(input_file.is_open());

  ImageAOS const ImageAOS(0, {});
  vector<rgb_big> const image = ImageAOS.read_image_rgb_big(input_file);

  ASSERT_TRUE(image.empty());
}

// Test con el metodo read_image_rgb_big que no funciona porque hay errores al leer el archivo
TEST_F(ImageAOSTest, ReadImageRGBBig_FileReadError) {
  std::ofstream output_file("corrupt_image.rgb", std::ios::binary);
  for (int i = 0; i < NUM_10; ++i) {
    auto value = static_cast<unsigned short>(i);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    output_file.write(reinterpret_cast<char *>(&value), sizeof(value));
  }
  output_file.close();

  std::ifstream input_file("corrupt_image.rgb", std::ios::binary);
  ASSERT_TRUE(input_file.is_open());

  ImageAOS const ImageAOS(0, {});
  vector<rgb_big> const image = ImageAOS.read_image_rgb_big(input_file);

  ASSERT_TRUE(image.empty());

  input_file.close();
}

TEST_F(ImageAOSTest, LoadAndMap8_Success) {
  std::ifstream input_file(getTestImagePath(), std::ios::binary);
  ASSERT_TRUE(input_file.is_open());

  auto result = getImageAOS()->cf_load_and_map_8(NUM_100, std::move(input_file), NUM_100);
  ASSERT_FALSE(result.empty());
}

// Test cf_load_map_8_BIG funciona
TEST_F(ImageAOSTest, CfLoadAndMap8BIGSuccess) {
  std::ifstream input_file("test_image.ppm", std::ios::binary);
  EXPECT_TRUE(input_file.is_open());

  ImageAOS const ImageAOS(0, {});
  auto result = getImageAOS()->cf_load_and_map_8BIG(NUM_100, std::move(input_file), NUM_100);
  EXPECT_FALSE(result.empty());
}

TEST_F(ImageAOSTest, CfLoadAndMap8BIG_FileNotOpen) {
  std::ifstream input_file("non_existent.ppm", std::ios::binary);
  EXPECT_FALSE(input_file.is_open());

  ImageAOS const ImageAOS(0, {});
  auto result = getImageAOS()->cf_load_and_map_8BIG(NUM_100, std::move(input_file), NUM_100);

  // Check if the result is not empty since the function does not handle file not open case
  EXPECT_FALSE(result.empty())
      << "Expected result to be not empty when input file cannot be opened.";
}

TEST_F(ImageAOSTest, CfLoadAndMap8BIG_InvalidWidth) {
  std::ifstream input_file("test_image.ppm", std::ios::binary);
  EXPECT_TRUE(input_file.is_open());

  ImageAOS const ImageAOS(0, {});
  auto result = getImageAOS()->cf_load_and_map_8BIG(0, std::move(input_file), NUM_100);

  // Check if the result is empty
  EXPECT_TRUE(result.empty()) << "Expected result to be empty when width is zero.";
}

TEST_F(ImageAOSTest, CfLoadAndMap8BIG_InvalidHeight) {
  std::ifstream input_file("test_image.ppm", std::ios::binary);
  EXPECT_TRUE(input_file.is_open());

  ImageAOS const ImageAOS(0, {});
  auto result = getImageAOS()->cf_load_and_map_8BIG(NUM_100, std::move(input_file), 0);

  // Check if the result is empty
  EXPECT_TRUE(result.empty()) << "Expected result to be empty when height is zero.";
}

TEST_F(ImageAOSTest, CfLoadAndMap8BIG_NegativeWidth) {
  std::ifstream input_file("test_image.ppm", std::ios::binary);
  EXPECT_TRUE(input_file.is_open());

  ImageAOS const ImageAOS(0, {});
  auto result = getImageAOS()->cf_load_and_map_8BIG(MNUM_100, std::move(input_file), NUM_100);

  // Check if the result is empty
  EXPECT_TRUE(result.empty()) << "Expected result to be empty when width is negative.";
}

TEST_F(ImageAOSTest, CfLoadAndMap8BIG_NegativeHeight) {
  std::ifstream input_file("test_image.ppm", std::ios::binary);
  EXPECT_TRUE(input_file.is_open());

  ImageAOS const ImageAOS(0, {});
  auto result = getImageAOS()->cf_load_and_map_8BIG(NUM_100, std::move(input_file), MNUM_100);

  EXPECT_TRUE(result.empty()) << "Expected result to be empty when height is negative.";
}

/*
TEST_F(ImageAOSTest, CfAddNodesBIG_Success) {
  ImageAOS ImageAOS(0, {});
  getImageAOS()->cf_add_nodes_BIG(NUM_75, NUM_150, NUM_240);

  EXPECT_EQ(ImageAOS.nodBIG.size(), 0);
  EXPECT_EQ(ImageAOS.nodBIG[0], packRGBIG(75, 75, 75));
  EXPECT_EQ(ImageAOS.nodBIG[1], packRGBIG(75, 75, 150));
  EXPECT_EQ(ImageAOS.nodBIG[2], packRGBIG(75, 75, 240));
  EXPECT_EQ(ImageAOS.nodBIG[3], packRGBIG(75, 150, 75));
  EXPECT_EQ(ImageAOS.nodBIG[4], packRGBIG(75, 150, 150));
  EXPECT_EQ(ImageAOS.nodBIG[5], packRGBIG(75, 150, 240));
  EXPECT_EQ(ImageAOS.nodBIG[6], packRGBIG(75, 240, 75));
  EXPECT_EQ(ImageAOS.nodBIG[7], packRGBIG(75, 240, 150));
  EXPECT_EQ(ImageAOS.nodBIG[8], packRGBIG(75, 240, 240));
  EXPECT_EQ(ImageAOS.nodBIG[9], packRGBIG(150, 75, 75));
  EXPECT_EQ(ImageAOS.nodBIG[10], packRGBIG(150, 75, 150));
  EXPECT_EQ(ImageAOS.nodBIG[11], packRGBIG(150, 75, 240));
  EXPECT_EQ(ImageAOS.nodBIG[12], packRGBIG(150, 150, 75));
  EXPECT_EQ(ImageAOS.nodBIG[13], packRGBIG(150, 150, 150));
  EXPECT_EQ(ImageAOS.nodBIG[14], packRGBIG(150, 150, 240));
  EXPECT_EQ(ImageAOS.nodBIG[15], packRGBIG(150, 240, 75));
  EXPECT_EQ(ImageAOS.nodBIG[16], packRGBIG(150, 240, 150));
  EXPECT_EQ(ImageAOS.nodBIG[17], packRGBIG(150, 240, 240));
  EXPECT_EQ(ImageAOS.nodBIG[18], packRGBIG(240, 75, 75));
  EXPECT_EQ(ImageAOS.nodBIG[19], packRGBIG(240, 75, 150));
  EXPECT_EQ(ImageAOS.nodBIG[20], packRGBIG(240, 75, 240));
  EXPECT_EQ(ImageAOS.nodBIG[21], packRGBIG(240, 150, 75));
  EXPECT_EQ(ImageAOS.nodBIG[22], packRGBIG(240, 150, 150));
  EXPECT_EQ(ImageAOS.nodBIG[23], packRGBIG(240, 150, 240));
  EXPECT_EQ(ImageAOS.nodBIG[24], packRGBIG(240, 240, 75));
  EXPECT_EQ(ImageAOS.nodBIG[25], packRGBIG(240, 240, 150));
  EXPECT_EQ(ImageAOS.nodBIG[26], packRGBIG(240, 240, 240));
}
*/
TEST_F(ImageAOSTest, CfAddNodesBIG_Failure) {
  ImageAOS const ImageAOS(0, {});
  getImageAOS()->cf_add_nodes_BIG(static_cast<__uint16_t>(NUM_M75),
                                  static_cast<__uint16_t>(NUM_M150),
                                  static_cast<__uint16_t>(NUM_M240));

  EXPECT_EQ(ImageAOS.nodBIG.size(), 0) << "Expected nodBIG to be empty when given negative values.";
}

TEST_F(ImageAOSTest, CfGenerateGraph_Success) {
  auto graph = getImageAOS()->cf_generate_graph();

  EXPECT_EQ(graph.size(), 7);

  EXPECT_EQ(graph[getImageAOS()->nod[0]].first.size(), 3);
  EXPECT_EQ(graph[getImageAOS()->nod[1]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nod[2]].first.size(), 3);
  EXPECT_EQ(graph[getImageAOS()->nod[3]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nod[4]].first.size(), 5);
  EXPECT_EQ(graph[getImageAOS()->nod[5]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nod[6]].first.size(), 3);
}

TEST_F(ImageAOSTest, CfGenerateGraph_Failure) {
  auto graph = getImageAOS()->cf_generate_graph();

  EXPECT_NE(graph.size(), 5);

  EXPECT_NE(graph[getImageAOS()->nod[0]].first.size(), 5);
  EXPECT_NE(graph[getImageAOS()->nod[1]].first.size(), 3);
  EXPECT_NE(graph[getImageAOS()->nod[2]].first.size(), 4);
  EXPECT_NE(graph[getImageAOS()->nod[3]].first.size(), 2);
  EXPECT_NE(graph[getImageAOS()->nod[4]].first.size(), 7);
  EXPECT_NE(graph[getImageAOS()->nod[5]].first.size(), 6);
  EXPECT_NE(graph[getImageAOS()->nod[6]].first.size(), 4);
}

TEST_F(ImageAOSTest, CfGenerateGraph2_Success) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph =
      getImageAOS()->cf_generate_graph();
  getImageAOS()->cf_generate_graph_2(graph);

  // Correct assertions to verify the graph structure
  EXPECT_EQ(graph.size(), 14);
  EXPECT_EQ(graph[getImageAOS()->nod[7]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nod[8]].first.size(), 3);
  EXPECT_EQ(graph[getImageAOS()->nod[9]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nod[10]].first.size(), 5);
  EXPECT_EQ(graph[getImageAOS()->nod[11]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nod[12]].first.size(), 7);
  EXPECT_EQ(graph[getImageAOS()->nod[13]].first.size(), 7);
}

TEST_F(ImageAOSTest, CfGenerateGraph2_Failure) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph =
      getImageAOS()->cf_generate_graph();
  ;
  getImageAOS()->cf_generate_graph_2(graph);

  // Intentionally incorrect assertions to cause the test to fail
  EXPECT_NE(graph.size(), 5);

  // Check if specific nodes and their connections are incorrect
  EXPECT_NE(graph[getImageAOS()->nod[7]].first.size(), 5);
  EXPECT_NE(graph[getImageAOS()->nod[8]].first.size(), 1);
  EXPECT_NE(graph[getImageAOS()->nod[9]].first.size(), 7);
  EXPECT_NE(graph[getImageAOS()->nod[10]].first.size(), 2);
  EXPECT_NE(graph[getImageAOS()->nod[11]].first.size(), 2);
  EXPECT_NE(graph[getImageAOS()->nod[12]].first.size(), 3);
  EXPECT_NE(graph[getImageAOS()->nod[13]].first.size(), 4);
}

TEST_F(ImageAOSTest, CfGenerateGraph3_Success) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph =
      getImageAOS()->cf_generate_graph();
  ;
  getImageAOS()->cf_generate_graph_2(graph);
  getImageAOS()->cf_generate_graph_3(graph);

  // Correct assertions to verify the graph structure
  EXPECT_EQ(graph.size(), 22);
  EXPECT_EQ(graph[getImageAOS()->nod[14]].first.size(), 5);
  EXPECT_EQ(graph[getImageAOS()->nod[15]].first.size(), 6);
  EXPECT_EQ(graph[getImageAOS()->nod[16]].first.size(), 5);
  EXPECT_EQ(graph[getImageAOS()->nod[17]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nod[18]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nod[19]].first.size(), 5);
  EXPECT_EQ(graph[getImageAOS()->nod[20]].first.size(), 3);
}

TEST_F(ImageAOSTest, CfGenerateGraph3_Failure) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph =
      getImageAOS()->cf_generate_graph();
  ;
  getImageAOS()->cf_generate_graph_2(graph);
  getImageAOS()->cf_generate_graph_3(graph);
  // Intentionally incorrect assertions to cause the test to fail
  EXPECT_NE(graph.size(), 4);

  // Check if specific nodes and their connections are incorrect
  EXPECT_NE(graph[getImageAOS()->nod[14]].first.size(), 1);
  EXPECT_NE(graph[getImageAOS()->nod[15]].first.size(), 2);
  EXPECT_NE(graph[getImageAOS()->nod[16]].first.size(), 3);
  EXPECT_NE(graph[getImageAOS()->nod[17]].first.size(), 7);
  EXPECT_NE(graph[getImageAOS()->nod[18]].first.size(), 5);
  EXPECT_NE(graph[getImageAOS()->nod[19]].first.size(), 8);
  EXPECT_NE(graph[getImageAOS()->nod[20]].first.size(), 7);
}

TEST_F(ImageAOSTest, CfGenerateGraph4_Success) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph =
      getImageAOS()->cf_generate_graph();
  ;
  getImageAOS()->cf_generate_graph_2(graph);
  getImageAOS()->cf_generate_graph_3(graph);
  getImageAOS()->cf_generate_graph_4(graph);

  // Correct assertions to verify the graph structure
  EXPECT_EQ(graph.size(), 27);
  EXPECT_EQ(graph[getImageAOS()->nod[21]].first.size(), 3);
  EXPECT_EQ(graph[getImageAOS()->nod[22]].first.size(), 5);
  EXPECT_EQ(graph[getImageAOS()->nod[23]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nod[24]].first.size(), 3);
  EXPECT_EQ(graph[getImageAOS()->nod[25]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nod[26]].first.size(), 3);
}

TEST_F(ImageAOSTest, CfGenerateGraph4_Failure) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph =
      getImageAOS()->cf_generate_graph();
  ;
  getImageAOS()->cf_generate_graph_2(graph);
  getImageAOS()->cf_generate_graph_3(graph);
  getImageAOS()->cf_generate_graph_4(graph);

  // Intentionally incorrect assertions to cause the test to fail
  EXPECT_NE(graph.size(), 7);

  // Check if specific nodes and their connections are incorrect
  EXPECT_NE(graph[getImageAOS()->nod[21]].first.size(), 1);
  EXPECT_NE(graph[getImageAOS()->nod[22]].first.size(), 7);
  EXPECT_NE(graph[getImageAOS()->nod[23]].first.size(), 1);
  EXPECT_NE(graph[getImageAOS()->nod[24]].first.size(), 5);
  EXPECT_NE(graph[getImageAOS()->nod[25]].first.size(), 7);
  EXPECT_NE(graph[getImageAOS()->nod[26]].first.size(), 6);
}

TEST_F(ImageAOSTest, CfGenerateGraphBIG_Success) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph;
  getImageAOS()->cf_generate_graph_BIG();

  // Correct assertions to verify the expected behavior
  EXPECT_EQ(graph.size(), 0);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[0]].first.size(), 0);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[1]].first.size(), 0);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[2]].first.size(), 0);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[3]].first.size(), 0);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[4]].first.size(), 0);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[5]].first.size(), 0);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[6]].first.size(), 0);
}

TEST_F(ImageAOSTest, CfGenerateGraphBIG_Failure) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph;
  getImageAOS()->cf_generate_graph_BIG();

  // Intentionally incorrect assertions to cause the test to fail
  EXPECT_NE(graph.size(), 7);
  EXPECT_NE(graph[getImageAOS()->nodBIG[0]].first.size(), 3);
  EXPECT_NE(graph[getImageAOS()->nodBIG[1]].first.size(), 4);
  EXPECT_NE(graph[getImageAOS()->nodBIG[2]].first.size(), 3);
  EXPECT_NE(graph[getImageAOS()->nodBIG[3]].first.size(), 4);
  EXPECT_NE(graph[getImageAOS()->nodBIG[4]].first.size(), 5);
  EXPECT_NE(graph[getImageAOS()->nodBIG[5]].first.size(), 4);
  EXPECT_NE(graph[getImageAOS()->nodBIG[6]].first.size(), 3);
}

TEST_F(ImageAOSTest, CfGenerateGraphBIG2_Success) {
  // Crea un grafo inicial
  std::unordered_map<__uint64_t, std::pair<std::vector<__uint64_t>, std::vector<__uint64_t>>>
      graph = getImageAOS()->cf_generate_graph_BIG();

  // Llama a la función cf_generate_graph_BIG_2
  getImageAOS()->cf_generate_graph_BIG_2(graph);

  // Verifica que el grafo se haya actualizado correctamente
  EXPECT_EQ(graph.size(), 14);  // Ajusta el tamaño esperado según tu implementación
  EXPECT_EQ(graph[getImageAOS()->nodBIG[7]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[8]].first.size(), 3);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[9]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[10]].first.size(), 5);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[11]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[12]].first.size(), 5);
}

TEST_F(ImageAOSTest, CfGenerateGraphBIG2_Failure) {
  // Crea un grafo inicial
  std::unordered_map<__uint64_t, std::pair<std::vector<__uint64_t>, std::vector<__uint64_t>>>
      graph = getImageAOS()->cf_generate_graph_BIG();

  // Llama a la función cf_generate_graph_BIG_2
  getImageAOS()->cf_generate_graph_BIG_2(graph);

  // Intentionally incorrect assertions to cause the test to fail
  EXPECT_NE(graph.size(), 12);  // Ajusta el tamaño esperado según tu implementación
  EXPECT_NE(graph[getImageAOS()->nodBIG[7]].first.size(), 5);
  EXPECT_NE(graph[getImageAOS()->nodBIG[8]].first.size(), 4);
  EXPECT_NE(graph[getImageAOS()->nodBIG[9]].first.size(), 3);
  EXPECT_NE(graph[getImageAOS()->nodBIG[10]].first.size(), 2);
  EXPECT_NE(graph[getImageAOS()->nodBIG[11]].first.size(), 1);
  EXPECT_NE(graph[getImageAOS()->nodBIG[12]].first.size(), 0);
}

TEST_F(ImageAOSTest, CfGenerateGraphBIG3_Success) {
  // Crea un grafo inicial
  std::unordered_map<__uint64_t, std::pair<std::vector<__uint64_t>, std::vector<__uint64_t>>> graph;
  graph = getImageAOS()->cf_generate_graph_BIG();

  // Llama a la función cf_generate_graph_BIG_2 para actualizar el grafo
  getImageAOS()->cf_generate_graph_BIG_2(graph);

  // Llama a la función cf_generate_graph_BIG_3 para actualizar el grafo
  getImageAOS()->cf_generate_graph_BIG_3(graph);

  // Verifica que el grafo se haya actualizado correctamente
  EXPECT_EQ(graph.size(), 22);  // Ajusta el tamaño esperado según tu implementación
  EXPECT_EQ(graph[getImageAOS()->nodBIG[14]].first.size(), 5);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[15]].first.size(), 5);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[16]].first.size(), 5);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[17]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[18]].first.size(), 3);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[19]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[20]].first.size(), 3);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[21]].first.size(), 3);
}

TEST_F(ImageAOSTest, CfGenerateGraphBIG3_Failure) {
  // Crea un grafo inicial
  std::unordered_map<__uint64_t, std::pair<std::vector<__uint64_t>, std::vector<__uint64_t>>> graph;
  graph = getImageAOS()->cf_generate_graph_BIG();

  // Llama a la función cf_generate_graph_BIG_2 para actualizar el grafo
  getImageAOS()->cf_generate_graph_BIG_2(graph);

  // Llama a la función cf_generate_graph_BIG_3 para actualizar el grafo
  getImageAOS()->cf_generate_graph_BIG_3(graph);

  // Verifica que el grafo se haya actualizado correctamente
  EXPECT_NE(graph.size(), 2);  // Ajusta el tamaño esperado según tu implementación
  EXPECT_NE(graph[getImageAOS()->nodBIG[14]].first.size(), 6);
  EXPECT_NE(graph[getImageAOS()->nodBIG[15]].first.size(), 4);
  EXPECT_NE(graph[getImageAOS()->nodBIG[16]].first.size(), 3);
  EXPECT_NE(graph[getImageAOS()->nodBIG[17]].first.size(), 2);
  EXPECT_NE(graph[getImageAOS()->nodBIG[18]].first.size(), 4);
  EXPECT_NE(graph[getImageAOS()->nodBIG[19]].first.size(), 6);
  EXPECT_NE(graph[getImageAOS()->nodBIG[20]].first.size(), 5);
  EXPECT_NE(graph[getImageAOS()->nodBIG[21]].first.size(), 4);
}

TEST_F(ImageAOSTest, CfGenerateGraphBIG4_Success) {
  std::unordered_map<__uint64_t, std::pair<std::vector<__uint64_t>, std::vector<__uint64_t>>> graph;
  graph = getImageAOS()->cf_generate_graph_BIG();

  getImageAOS()->cf_generate_graph_BIG_2(graph);

  getImageAOS()->cf_generate_graph_BIG_3(graph);

  getImageAOS()->cf_generate_graph_BIG_4(graph);

  EXPECT_EQ(graph.size(), 27);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[22]].first.size(), 5);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[23]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[24]].first.size(), 3);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[25]].first.size(), 4);
  EXPECT_EQ(graph[getImageAOS()->nodBIG[26]].first.size(), 3);
}

TEST_F(ImageAOSTest, CfGenerateGraphBIG4_Failure) {
  std::unordered_map<__uint64_t, std::pair<std::vector<__uint64_t>, std::vector<__uint64_t>>> graph;
  graph = getImageAOS()->cf_generate_graph_BIG();

  getImageAOS()->cf_generate_graph_BIG_2(graph);

  getImageAOS()->cf_generate_graph_BIG_3(graph);

  getImageAOS()->cf_generate_graph_BIG_4(graph);

  EXPECT_NE(graph.size(), 25);
  EXPECT_NE(graph[getImageAOS()->nodBIG[22]].first.size(), 4);
  EXPECT_NE(graph[getImageAOS()->nodBIG[23]].first.size(), 2);
  EXPECT_NE(graph[getImageAOS()->nodBIG[24]].first.size(), 6);
  EXPECT_NE(graph[getImageAOS()->nodBIG[25]].first.size(), 7);
  EXPECT_NE(graph[getImageAOS()->nodBIG[26]].first.size(), 2);
}

TEST_F(ImageAOSTest, CfWriteInExit_EmptyDeleteitems) {
  std::unordered_map<__uint32_t, __uint32_t> const Deleteitems;
  getImageAOS()->array_small = {
    {.r = NUM_10, .g = NUM_40, .b = NUM_70},
    {.r = NUM_20, .g = NUM_50, .b = NUM_80},
    {.r = NUM_30, .g = NUM_60, .b = NUM_90}
  };

  std::ofstream output_file("output_empty_deleteitems.ppm", std::ios::binary);
  getImageAOS()->cf_write_in_exit(Deleteitems);
  output_file.close();

  // Add assertions to verify the output file content
}

TEST_F(ImageAOSTest, CfWriteInExit_SomeColorsInDeleteitems) {
  std::unordered_map<__uint32_t, __uint32_t> const Deleteitems = {
    {packRGB(NUM_10, NUM_40, NUM_70), packRGB(NUM_100, NUM_110, NUM_120)}
  };
  getImageAOS()->array_small = {
    {.r = NUM_10, .g = NUM_40, .b = NUM_70},
    {.r = NUM_20, .g = NUM_50, .b = NUM_80},
    {.r = NUM_30, .g = NUM_60, .b = NUM_90}
  };

  std::ofstream output_file("output_some_colors_deleteitems.ppm", std::ios::binary);
  getImageAOS()->cf_write_in_exit(Deleteitems);
  output_file.close();

  // Add assertions to verify the output file content
}

TEST_F(ImageAOSTest, CfWriteInExit_AllColorsInDeleteitems) {
  std::unordered_map<__uint32_t, __uint32_t> const Deleteitems = {
    {packRGB(10, 40, 70), packRGB(100, 110, 120)},
    {packRGB(20, 50, 80), packRGB(130, 140, 150)},
    {packRGB(30, 60, 90), packRGB(160, 170, 180)}
  };
  getImageAOS()->array_small = {
    {.r = NUM_10, .g = NUM_40, .b = NUM_70},
    {.r = NUM_20, .g = NUM_50, .b = NUM_80},
    {.r = NUM_30, .g = NUM_60, .b = NUM_90}
  };

  std::ofstream output_file("output_all_colors_deleteitems.ppm", std::ios::binary);
  getImageAOS()->cf_write_in_exit(Deleteitems);
  output_file.close();

  // Add assertions to verify the output file content
}

TEST_F(ImageAOSTest, CfWriteInExitBIG_EmptyDeleteitems) {
  std::unordered_map<__uint64_t, __uint64_t> const Deleteitems;
  getImageAOS()->array_big = {
    {.r = NUM_1000, .g = NUM_4000, .b = NUM_7000},
    {.r = NUM_2000, .g = NUM_5000, .b = NUM_8000},
    {.r = NUM_3000, .g = NUM_6000, .b = NUM_9000}
  };

  std::ofstream output_file("output_empty_deleteitems.ppm", std::ios::binary);
  getImageAOS()->cf_write_in_exit_BIG(Deleteitems);
  output_file.close();

  // Add assertions to verify the output file content
}

TEST_F(ImageAOSTest, CfWriteInExitBIG_SomeColorsInDeleteitems) {
  std::unordered_map<__uint64_t, __uint64_t> const Deleteitems = {
    {packRGBIG(1000, 4000, 7000), packRGBIG(10000, 11000, 12000)}
  };
  getImageAOS()->array_big = {
    {.r = NUM_1000, .g = NUM_4000, .b = NUM_7000},
    {.r = NUM_2000, .g = NUM_5000, .b = NUM_8000},
    {.r = NUM_3000, .g = NUM_6000, .b = NUM_9000}
  };

  std::ofstream output_file("output_some_colors_deleteitems.ppm", std::ios::binary);
  getImageAOS()->cf_write_in_exit_BIG(Deleteitems);
  output_file.close();

  // Add assertions to verify the output file content
}

TEST_F(ImageAOSTest, CfWriteInExitBIG_AllColorsInDeleteitems) {
  std::unordered_map<__uint64_t, __uint64_t> const Deleteitems = {
    {packRGBIG(1000, 4000, 7000), packRGBIG(10000, 11000, 12000)},
    {packRGBIG(2000, 5000, 8000), packRGBIG(13000, 14000, 15000)},
    {packRGBIG(3000, 6000, 9000), packRGBIG(16000, 17000, 18000)}
  };
  getImageAOS()->array_big = {
    {.r = NUM_1000, .g = NUM_4000, .b = NUM_7000},
    {.r = NUM_2000, .g = NUM_5000, .b = NUM_8000},
    {.r = NUM_3000, .g = NUM_6000, .b = NUM_9000}
  };

  std::ofstream output_file("output_all_colors_deleteitems.ppm", std::ios::binary);
  getImageAOS()->cf_write_in_exit_BIG(Deleteitems);
  output_file.close();

  // Add assertions to verify the output file content
}

TEST_F(ImageAOSTest, CfWriteInExitBIG2_EmptyDeleteitems) {
  std::unordered_map<__uint64_t, __uint64_t> const Deleteitems;
  getImageAOS()->array_big = {
    {.r = NUM_1000, .g = NUM_4000, .b = NUM_7000},
    {.r = NUM_2000, .g = NUM_5000, .b = NUM_8000},
    {.r = NUM_3000, .g = NUM_6000, .b = NUM_9000}
  };

  std::ofstream output_file("output_empty_deleteitems.ppm", std::ios::binary);
  getImageAOS()->cf_write_in_exit_BIG(Deleteitems);
  output_file.close();

  // Add assertions to verify the output file content
}

TEST_F(ImageAOSTest, CfSearchInGraphSmall_SomeColorsInDeleteitems) {
  unordered_map<__uint32_t, __uint32_t> Deleteitems = {
    {packRGB(NUM_10, NUM_40, NUM_70), packRGB(NUM_100, NUM_110, NUM_120)}
  };
  unordered_map<__uint32_t, pair<vector<uint32_t>, vector<__uint32_t>>> const graph = {
    {packRGB(NUM_100, NUM_110, NUM_120),
     {{packRGB(NUM_130, NUM_140, NUM_150)}, {packRGB(NUM_160, NUM_170, NUM_180)}}},
    {packRGB(NUM_130, NUM_140, NUM_150),                                 {{}, {}}}
  };

  ImageAOS::cf_search_in_graph_small(Deleteitems, graph);

  // Add assertions to verify the Deleteitems content
  EXPECT_EQ(Deleteitems[packRGB(NUM_10, NUM_40, NUM_70)], packRGB(NUM_160, NUM_170, NUM_180));
}

TEST_F(ImageAOSTest, CfSearchInGraphSmall_AllColorsInDeleteitems) {
  unordered_map<__uint32_t, __uint32_t> Deleteitems = {
    {packRGB(NUM_10, NUM_40, NUM_70), packRGB(NUM_100, NUM_110, NUM_120)},
    {packRGB(NUM_20, NUM_50, NUM_80), packRGB(NUM_130, NUM_140, NUM_150)},
    {packRGB(NUM_30, NUM_60, NUM_90), packRGB(NUM_160, NUM_170, NUM_180)}
  };
  unordered_map<__uint32_t, pair<std::vector<uint32_t>, vector<__uint32_t>>> const graph = {
    {packRGB(100, 110, 120), {{packRGB(130, 140, 150)}, {packRGB(160, 170, 180)}}},
    {packRGB(130, 140, 150), {{packRGB(160, 170, 180)}, {packRGB(190, 200, 210)}}},
    {packRGB(160, 170, 180), {{packRGB(100, 110, 120)}, {packRGB(220, 230, 240)}}}
  };
  ImageAOS::cf_search_in_graph_small(Deleteitems, graph);
  EXPECT_EQ(Deleteitems[packRGB(NUM_10, NUM_40, NUM_70)], packRGB(NUM_160, NUM_170, NUM_180));
  EXPECT_EQ(Deleteitems[packRGB(NUM_20, NUM_50, NUM_80)], packRGB(NUM_20, NUM_50, NUM_80));
  EXPECT_EQ(Deleteitems[packRGB(NUM_30, NUM_60, NUM_90)], packRGB(NUM_160, NUM_170, NUM_180));
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
