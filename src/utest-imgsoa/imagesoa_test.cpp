#include "common/binario.hpp"
#include "imgsoa/imagesoa.hpp"

#include <array>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <gsl/gsl>
#include <gtest/gtest.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
//NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
static constexpr int NUM_100  = 100;
static constexpr int MNUM_100 = -100;
static constexpr int FOTO     = 256;
static constexpr int NUM_5    = 5;
static constexpr int NUM_6    = 6;
static constexpr int NUM_7    = 7;
static constexpr int NUM_8    = 8;
static constexpr int NUM_9    = 9;
static constexpr int NUM_10   = 10;
static constexpr int NUM_11   = 11;
static constexpr int NUM_12   = 12;
static constexpr int NUM_13   = 13;
static constexpr int NUM_14   = 14;
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
static constexpr int NUM_160  = 160;
static constexpr int NUM_170  = 170;
static constexpr int NUM_180  = 180;

static constexpr int NUM_M75  = -75;
static constexpr int NUM_M150 = -150;
static constexpr int NUM_M240 = -240;
static constexpr int NUM_75   = 75;
static constexpr int NUM_150  = 150;
static constexpr int NUM_240  = 240;
static constexpr int NUM_255  = 255;
static constexpr int NUM_1000 = 1000;
static constexpr int NUM_2000 = 2000;
static constexpr int NUM_3000 = 3000;
static constexpr int NUM_4000 = 4000;
static constexpr int NUM_5000 = 5000;
static constexpr int NUM_6000 = 6000;
static constexpr int NUM_7000 = 7000;
static constexpr int NUM_8000 = 8000;
static constexpr int NUM_9000 = 9000;
static constexpr int NUM_10000 = 10000;
static constexpr int NUM_11000 = 11000;
static constexpr int NUM_12000 = 12000;
static constexpr int NUM_13000 = 13000;
static constexpr int NUM_14000 = 14000;
static constexpr int NUM_15000 = 15000;
static constexpr int NUM_16000 = 16000;
static constexpr int NUM_17000 = 17000;
static constexpr int NUM_18000 = 18000;
static constexpr int NUM_65535 = 65535;
static constexpr int NUM_70000 = 70000;

class ImageSOATest : public ::testing::Test {
  private:
    gsl::owner<ImageSOA *> imageSOA = nullptr;
    string test_image_path;

  protected:
    void SetUp() override {
      vector<string> const args = {"param", "test_image.ppm", "output_image.ppm"};
      imageSOA                  = new ImageSOA(static_cast<int>(args.size()), args);

      test_image_path = "test_image.ppm";
      ofstream output_file(test_image_path, ios::binary);
      if (!output_file) { FAIL() << "Failed to create test image file."; }
      output_file << "P6\n" << NUM_100 << " " << NUM_100 << "\n" << FOTO - 1 << "\n";

      for (int i = 0; i < NUM_100 * NUM_100; ++i) {
        output_file.put(static_cast<char>(i % FOTO));
        output_file.put(static_cast<char>((i + 1) % FOTO));
        output_file.put(static_cast<char>((i + 2) % FOTO));
      }
      output_file.close();
      imageSOA->cf_add_nodes();

      // Añade nodos necesarios
      imageSOA->cf_add_nodes_BIG(NUM_75, NUM_150, NUM_240);
    }

    void TearDown() override {
      if (imageSOA != nullptr) {
        delete imageSOA;
        imageSOA = nullptr;
      }
      if (ifstream(test_image_path.c_str()).good()) {
        if (remove(test_image_path.c_str()) != 0) { perror("Error deleting file"); }
      }

      if (ifstream("test_image.ppm").good()) {
        if (remove("test_image.ppm") != 0) { perror("Error deleting file"); }
      }
    }

  public:
    ImageSOATest() = default;

    ~ImageSOATest() override { delete imageSOA; }

    ImageSOATest(ImageSOATest const & other)
      : imageSOA(new ImageSOA(std::move(*other.imageSOA))), test_image_path(other.test_image_path) {
    }

    ImageSOATest & operator=(ImageSOATest const & other) {
      if (this != &other) {
        delete imageSOA;
        imageSOA        = new ImageSOA(std::move(*other.imageSOA));
        test_image_path = other.test_image_path;
      }
      return *this;
    }

    ImageSOATest(ImageSOATest && other) noexcept
      : imageSOA(other.imageSOA), test_image_path(std::move(other.test_image_path)) {
      other.imageSOA = nullptr;
    }

    ImageSOATest & operator=(ImageSOATest && other) noexcept {
      if (this != &other) {
        delete imageSOA;
        imageSOA        = other.imageSOA;
        test_image_path = std::move(other.test_image_path);
        other.imageSOA  = nullptr;
      }
      return *this;
    }

    [[nodiscard]] string const & getTestImagePath() const { return test_image_path; }

    [[nodiscard]] gsl::owner<ImageSOA *> getImageSOA() const { return imageSOA; }
};

// Test con el metodo rsz_obtain_square_min que funciona correctamente
TEST_F(ImageSOATest, RszObtainSquareMin) {
  soa_rgb_small image;
  image.r = {0,     1,     2,      3,      4,      NUM_5,  NUM_6, NUM_7,
             NUM_8, NUM_9, NUM_10, NUM_11, NUM_12, NUM_13, NUM_14};
  image.g = {NUM_15, NUM_16, NUM_17, NUM_18, NUM_19, NUM_20, NUM_21, NUM_22, NUM_23, NUM_24};
  image.b = {NUM_25, NUM_26, NUM_27, NUM_28, NUM_29, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9};

  array<unsigned int, 5> const args = {4, 5, 7, 8, 3};

  array<rgb_small, 4> result = ImageSOA::rsz_obtain_square_min(image, args);

  EXPECT_EQ(result[0].r, 14);
  EXPECT_EQ(result[0].g, 0);
  EXPECT_EQ(result[0].b, 0);

  EXPECT_EQ(result[1].r, 14);
  EXPECT_EQ(result[1].g, 0);
  EXPECT_EQ(result[1].b, 0);

  EXPECT_EQ(result[2].r, 14);
  EXPECT_EQ(result[2].g, 0);
  EXPECT_EQ(result[2].b, 0);

  EXPECT_EQ(result[3].r, 14);
  EXPECT_EQ(result[3].g, 0);
  EXPECT_EQ(result[3].b, 0);
}

// Test con el metodo rsz_obtain_square_min que no funciona
TEST_F(ImageSOATest, RszObtainSquareMinFailure) {
  soa_rgb_small image;
  image.r = {0,     1,     2,      3,      4,      NUM_5,  NUM_6, NUM_7,
             NUM_8, NUM_9, NUM_10, NUM_11, NUM_12, NUM_13, NUM_14};
  image.g = {NUM_15, NUM_16, NUM_17, NUM_18, NUM_19, NUM_20, NUM_21, NUM_22, NUM_23, NUM_24};
  image.b = {NUM_25, NUM_26, NUM_27, NUM_28, NUM_29, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9};

  array<unsigned int, 5> const args = {4, 5, 7, 8, 3};

  array<rgb_small, 4> result = ImageSOA::rsz_obtain_square_min(image, args);

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
TEST_F(ImageSOATest, RszObtainSquareMax) {
  soa_rgb_big image;
  image.r = {0,     1,     2,      3,      4,      NUM_5,  NUM_6, NUM_7,
             NUM_8, NUM_9, NUM_10, NUM_11, NUM_12, NUM_13, NUM_14};
  image.g = {NUM_15, NUM_16, NUM_17, NUM_18, NUM_19, NUM_20, NUM_21, NUM_22, NUM_23, NUM_24};
  image.b = {NUM_25, NUM_26, NUM_27, NUM_28, NUM_29, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9};

  array<unsigned int, 5> const args = {4, 5, 7, 8, 3};

  array<rgb_big, 4> result = ImageSOA::rsz_obtain_square_max(image, args);

  EXPECT_EQ(result[0].r, 14);
  EXPECT_EQ(result[0].g, 0);
  EXPECT_EQ(result[0].b, 0);

  EXPECT_EQ(result[1].r, 14);
  EXPECT_EQ(result[1].g, 0);
  EXPECT_EQ(result[1].b, 0);

  EXPECT_EQ(result[2].r, 14);
  EXPECT_EQ(result[2].g, 0);
  EXPECT_EQ(result[2].b, 0);

  EXPECT_EQ(result[3].r, 14);
  EXPECT_EQ(result[3].g, 0);
  EXPECT_EQ(result[3].b, 0);
}

// Test con el metodo rsz_obtain_square_max que no funciona
TEST_F(ImageSOATest, RszObtainSquareMaxFailure) {
  soa_rgb_big image;
  image.r = {0,     1,     2,      3,      4,      NUM_5,  NUM_6, NUM_7,
             NUM_8, NUM_9, NUM_10, NUM_11, NUM_12, NUM_13, NUM_14};
  image.g = {NUM_15, NUM_16, NUM_17, NUM_18, NUM_19, NUM_20, NUM_21, NUM_22, NUM_23, NUM_24};
  image.b = {NUM_25, NUM_26, NUM_27, NUM_28, NUM_29, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9};

  array<unsigned int, 5> const args = {4, 5, 7, 8, 3};

  array<rgb_big, 4> result = ImageSOA::rsz_obtain_square_max(image, args);

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
TEST_F(ImageSOATest, RszInterpolateMinSuccess) {
  constexpr array<rgb_small, 4> square = {
    rgb_small{ .r = 10,  .g = 20,  .b = 30},
    rgb_small{ .r = 40,  .g = 50,  .b = 60},
    rgb_small{ .r = 70,  .g = 80,  .b = 90},
    rgb_small{.r = 100, .g = 110, .b = 120}
  };

  constexpr double u_param = 0.5;
  constexpr double t_param = 0.5;

  rgb_small const result = ImageSOA::rsz_interpolate_min(u_param, square, t_param);

  EXPECT_EQ(result.r, 55);
  EXPECT_EQ(result.g, 65);
  EXPECT_EQ(result.b, 75);
}

// Test con el metodo rsz_interpolate_min que no funciona
TEST_F(ImageSOATest, RszInterpolateMinFailure) {
  constexpr array<rgb_small, 4> square = {
    rgb_small{ .r = 10,  .g = 20,  .b = 30},
    rgb_small{ .r = 40,  .g = 50,  .b = 60},
    rgb_small{ .r = 70,  .g = 80,  .b = 90},
    rgb_small{.r = 100, .g = 110, .b = 120}
  };

  constexpr double u_param = 0.5;
  constexpr double t_param = 0.5;

  rgb_small const result = ImageSOA::rsz_interpolate_min(u_param, square, t_param);

  EXPECT_NE(result.r, 10);
  EXPECT_NE(result.g, 20);
  EXPECT_NE(result.b, 30);
}

// Test con el metodo rsz_interpolate_max que funciona correctamente
TEST_F(ImageSOATest, RszInterpolateMaxSuccess) {
  constexpr array<rgb_big, 4> square = {
    rgb_big{ .r = 10,  .g = 20,  .b = 30},
    rgb_big{ .r = 40,  .g = 50,  .b = 60},
    rgb_big{ .r = 70,  .g = 80,  .b = 90},
    rgb_big{.r = 100, .g = 110, .b = 120}
  };

  constexpr double u_param = 0.5;
  constexpr double t_param = 0.5;

  rgb_big const result = ImageSOA::rsz_interpolate_max(u_param, square, t_param);

  EXPECT_EQ(result.r, 55);
  EXPECT_EQ(result.g, 65);
  EXPECT_EQ(result.b, 75);
}

// Test con el metodo rsz_interpolate_max que no funciona por un error de acceso a memoria
TEST_F(ImageSOATest, RszInterpolateMaxOutOfBounds) {
  constexpr array<rgb_big, 4> square = {
    rgb_big{.r = 10, .g = 20, .b = 30},
    rgb_big{.r = 40, .g = 50, .b = 60},
    rgb_big{.r = 70, .g = 80, .b = 90},
    rgb_big{ .r = 0,  .g = 0,  .b = 0}
  };

  constexpr double u_param = 0.5;
  constexpr double t_param = 0.5;

  rgb_big const result = ImageSOA::rsz_interpolate_max(u_param, square, t_param);

  EXPECT_NE(result.r, 55);
  EXPECT_NE(result.g, 65);
  EXPECT_NE(result.b, 75);
}

// Test con el metodo rsz_interpolate_max que no funciona por un valor de u_param invalido
TEST_F(ImageSOATest, RszInterpolateMaxInvalidUParam) {
  constexpr array<rgb_big, 4> square = {
    rgb_big{ .r = 10,  .g = 20,  .b = 30},
    rgb_big{ .r = 40,  .g = 50,  .b = 60},
    rgb_big{ .r = 70,  .g = 80,  .b = 90},
    rgb_big{.r = 100, .g = 110, .b = 120}
  };

  constexpr double u_param = 1.5;
  constexpr double t_param = 0.5;

  rgb_big const result = ImageSOA::rsz_interpolate_max(u_param, square, t_param);

  EXPECT_NE(result.r, 55);
  EXPECT_NE(result.g, 65);
  EXPECT_NE(result.b, 75);
}

// Test con el methods rsz_interpolate_max que no function por un valor de t_param invalid
TEST_F(ImageSOATest, RszInterpolateMaxInvalidTParam) {
  constexpr array<rgb_big, 4> square = {
    rgb_big{ .r = 10,  .g = 20,  .b = 30},
    rgb_big{ .r = 40,  .g = 50,  .b = 60},
    rgb_big{ .r = 70,  .g = 80,  .b = 90},
    rgb_big{.r = 100, .g = 110, .b = 120}
  };

  constexpr double u_param = 0.5;
  constexpr double t_param = 1.5;

  rgb_big const result = ImageSOA::rsz_interpolate_max(u_param, square, t_param);

  EXPECT_NE(result.r, 55);
  EXPECT_NE(result.g, 65);
  EXPECT_NE(result.b, 75);
}

TEST_F(ImageSOATest, ReadImageRGBSmallSuccess) {
  ifstream input_file(getTestImagePath(), ios::binary);
  EXPECT_TRUE(input_file.is_open());

  soa_rgb_small const result = getImageSOA()->read_image_rgb_small(input_file);

  EXPECT_EQ(result.r.size(), getImageSOA()->get_width() * getImageSOA()->get_height());
  EXPECT_EQ(result.g.size(), getImageSOA()->get_width() * getImageSOA()->get_height());
  EXPECT_EQ(result.b.size(), getImageSOA()->get_width() * getImageSOA()->get_height());
}

TEST_F(ImageSOATest, ReadImageRGBSmallFileNotFound) {
  ifstream input_file("non_existent_file.rgb", ios::binary);
  EXPECT_FALSE(input_file.is_open());
  input_file.close();
  if (filesystem::exists("non_existent_file.rgb")) {
    if (!filesystem::remove("non_existent_file.rgb")) {
      cerr << "Error deleting file: non_existent_file.rgb" << '\n';
    }
  }
}

// Test con el metodo read_image_rgb_big que funciona correctamente
TEST_F(ImageSOATest, ReadImageRGBBig_Success) {
  ifstream input_file("test_image.ppm", ios::binary);
  EXPECT_TRUE(input_file.is_open());

  ImageSOA const imageSOA(0, {});
  soa_rgb_big const image = imageSOA.read_image_rgb_big(input_file);

  EXPECT_EQ(image.r.size(), imageSOA.get_width() * imageSOA.get_height());
  EXPECT_EQ(image.g.size(), imageSOA.get_width() * imageSOA.get_height());
  EXPECT_EQ(image.b.size(), imageSOA.get_width() * imageSOA.get_height());

  input_file.close();
  if (filesystem::exists("test_image.rgb")) {
    if (!filesystem::remove("test_image.rgb")) {
      cerr << "Error deleting file: test_image.rgb" << '\n';
    }
  }
}

// Test con el metodo read_image_rgb_big que no funciona porque no se puede abrir un archivo
TEST_F(ImageSOATest, ReadImageRGBBig_FileNotOpen) {
  ifstream input_file("non_existent_file.rgb", ios::binary);
  EXPECT_FALSE(input_file.is_open());

  ImageSOA const imageSOA(0, {});
  soa_rgb_big const image = imageSOA.read_image_rgb_big(input_file);

  EXPECT_TRUE(image.r.empty());
  EXPECT_TRUE(image.g.empty());
  EXPECT_TRUE(image.b.empty());
  input_file.close();
  if (filesystem::exists("non_existent_file.rgb")) {
    if (!filesystem::remove("non_existent_file.rgb")) {
      cerr << "Error deleting file: non_existent_file.rgb" << '\n';
    }
  }
}

TEST_F(ImageSOATest, LoadAndMap8_Success) {
  ifstream input_file(getTestImagePath(), ios::binary);
  EXPECT_TRUE(input_file.is_open());

  auto result = getImageSOA()->cf_load_and_map_8(NUM_100, std::move(input_file), NUM_100);
  EXPECT_FALSE(result.empty());
}

// Test cf_load_map_8_BIG funciona
TEST_F(ImageSOATest, CfLoadAndMap8BIGSuccess) {
  ifstream input_file("test_image.ppm", ios::binary);
  EXPECT_TRUE(input_file.is_open());

  ImageSOA const imageSOA(0, {});
  auto result = getImageSOA()->cf_load_and_map_8BIG(NUM_100, std::move(input_file), NUM_100);
  EXPECT_FALSE(result.empty());
  if (filesystem::exists("test_image.ppm")) {
    if (!filesystem::remove("test_image.ppm")) {
      cerr << "Error deleting file: test_image.ppm" << '\n';
    }
  }
}

TEST_F(ImageSOATest, CfLoadAndMap8BIG_FileNotOpen) {
  ifstream input_file("non_existent.ppm", ios::binary);
  EXPECT_FALSE(input_file.is_open());

  ImageSOA const imageSOA(0, {});
  auto result = getImageSOA()->cf_load_and_map_8BIG(NUM_100, std::move(input_file), NUM_100);

  EXPECT_FALSE(result.empty())
      << "Expected result to be not empty when input file cannot be opened.";
  if (filesystem::exists("non_existent.ppm")) {
    if (!filesystem::remove("non_existent.ppm")) {
      cerr << "Error deleting file: non_existent.ppm" << '\n';
    }
  }
}

TEST_F(ImageSOATest, CfLoadAndMap8BIG_InvalidWidth) {
  ifstream input_file("test_image.ppm", ios::binary);
  EXPECT_TRUE(input_file.is_open());

  ImageSOA const imageSOA(0, {});
  auto result = getImageSOA()->cf_load_and_map_8BIG(0, std::move(input_file), NUM_100);

  EXPECT_TRUE(result.empty()) << "Expected result to be empty when width is zero.";
}

TEST_F(ImageSOATest, CfLoadAndMap8BIG_InvalidHeight) {
  ifstream input_file("test_image.ppm", ios::binary);
  EXPECT_TRUE(input_file.is_open());

  ImageSOA const imageSOA(0, {});
  auto result = getImageSOA()->cf_load_and_map_8BIG(NUM_100, std::move(input_file), 0);

  EXPECT_TRUE(result.empty()) << "Expected result to be empty when height is zero.";
}

TEST_F(ImageSOATest, CfLoadAndMap8BIG_NegativeWidth) {
  ifstream input_file("test_image.ppm", ios::binary);
  EXPECT_TRUE(input_file.is_open());

  ImageSOA const imageSOA(0, {});
  auto result = getImageSOA()->cf_load_and_map_8BIG(MNUM_100, std::move(input_file), NUM_100);

  EXPECT_TRUE(result.empty()) << "Expected result to be empty when width is negative.";
}

TEST_F(ImageSOATest, CfLoadAndMap8BIG_NegativeHeight) {
  ifstream input_file("test_image.ppm", ios::binary);
  EXPECT_TRUE(input_file.is_open());

  ImageSOA const imageSOA(0, {});
  auto result = getImageSOA()->cf_load_and_map_8BIG(NUM_100, std::move(input_file), MNUM_100);

  EXPECT_TRUE(result.empty()) << "Expected result to be empty when height is negative.";
}

TEST_F(ImageSOATest, CfAddNodes) {
  ImageSOA imageSOA(0, {});
  imageSOA.cf_add_nodes();

  ASSERT_EQ(imageSOA.nod.size(), 27);

  EXPECT_EQ(imageSOA.nod[0], packRGB(75, 75, 75));
  EXPECT_EQ(imageSOA.nod[1], packRGB(75, 75, 150));
  EXPECT_EQ(imageSOA.nod[2], packRGB(75, 75, 240));
  EXPECT_EQ(imageSOA.nod[3], packRGB(75, 150, 75));
  EXPECT_EQ(imageSOA.nod[4], packRGB(75, 150, 150));
  EXPECT_EQ(imageSOA.nod[5], packRGB(75, 150, 240));
  EXPECT_EQ(imageSOA.nod[6], packRGB(75, 240, 75));
  EXPECT_EQ(imageSOA.nod[7], packRGB(75, 240, 150));
  EXPECT_EQ(imageSOA.nod[8], packRGB(75, 240, 240));
  EXPECT_EQ(imageSOA.nod[9], packRGB(150, 75, 75));
  EXPECT_EQ(imageSOA.nod[10], packRGB(150, 75, 150));
  EXPECT_EQ(imageSOA.nod[11], packRGB(150, 75, 240));
  EXPECT_EQ(imageSOA.nod[12], packRGB(150, 150, 75));
  EXPECT_EQ(imageSOA.nod[13], packRGB(150, 150, 150));
  EXPECT_EQ(imageSOA.nod[14], packRGB(150, 150, 240));
  EXPECT_EQ(imageSOA.nod[15], packRGB(150, 240, 75));
  EXPECT_EQ(imageSOA.nod[16], packRGB(150, 240, 150));
  EXPECT_EQ(imageSOA.nod[17], packRGB(150, 240, 240));
  EXPECT_EQ(imageSOA.nod[18], packRGB(240, 75, 75));
  EXPECT_EQ(imageSOA.nod[19], packRGB(240, 75, 150));
  EXPECT_EQ(imageSOA.nod[20], packRGB(240, 75, 240));
  EXPECT_EQ(imageSOA.nod[21], packRGB(240, 150, 75));
  EXPECT_EQ(imageSOA.nod[22], packRGB(240, 150, 150));
  EXPECT_EQ(imageSOA.nod[23], packRGB(240, 150, 240));
  EXPECT_EQ(imageSOA.nod[24], packRGB(240, 240, 75));
  EXPECT_EQ(imageSOA.nod[25], packRGB(240, 240, 150));
  EXPECT_EQ(imageSOA.nod[26], packRGB(240, 240, 240));
}

TEST_F(ImageSOATest, CfAddNodes_Failure) {
  ImageSOA imageSOA(0, {});
  imageSOA.cf_add_nodes();

  ASSERT_NE(imageSOA.nod.size(), 21);

  EXPECT_NE(imageSOA.nod[0], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[1], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[2], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[3], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[4], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[5], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[6], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[7], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[8], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[9], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[10], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[11], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[12], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[13], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[14], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[15], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[16], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[17], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[18], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[19], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[20], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[21], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[22], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[23], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[24], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[25], packRGB(0, 0, 0));
  EXPECT_NE(imageSOA.nod[26], packRGB(0, 0, 0));
}


TEST_F(ImageSOATest, CfAddNodesBIG_Success) {
  ImageSOA imageSOA(0, {});
  imageSOA.cf_add_nodes_BIG(NUM_75, NUM_150, NUM_240);

  ASSERT_EQ(imageSOA.nodBIG.size(), 27);

  EXPECT_EQ(imageSOA.nodBIG[0], packRGBIG(75, 75, 75));
  EXPECT_EQ(imageSOA.nodBIG[1], packRGBIG(75, 75, 150));
  EXPECT_EQ(imageSOA.nodBIG[2], packRGBIG(75, 75, 240));
  EXPECT_EQ(imageSOA.nodBIG[3], packRGBIG(75, 150, 75));
  EXPECT_EQ(imageSOA.nodBIG[4], packRGBIG(75, 150, 150));
  EXPECT_EQ(imageSOA.nodBIG[5], packRGBIG(75, 150, 240));
  EXPECT_EQ(imageSOA.nodBIG[6], packRGBIG(75, 240, 75));
  EXPECT_EQ(imageSOA.nodBIG[7], packRGBIG(75, 240, 150));
  EXPECT_EQ(imageSOA.nodBIG[8], packRGBIG(75, 240, 240));
  EXPECT_EQ(imageSOA.nodBIG[9], packRGBIG(150, 75, 75));
  EXPECT_EQ(imageSOA.nodBIG[10], packRGBIG(150, 75, 150));
  EXPECT_EQ(imageSOA.nodBIG[11], packRGBIG(150, 75, 240));
  EXPECT_EQ(imageSOA.nodBIG[12], packRGBIG(150, 150, 75));
  EXPECT_EQ(imageSOA.nodBIG[13], packRGBIG(150, 150, 150));
  EXPECT_EQ(imageSOA.nodBIG[14], packRGBIG(150, 150, 240));
  EXPECT_EQ(imageSOA.nodBIG[15], packRGBIG(150, 240, 75));
  EXPECT_EQ(imageSOA.nodBIG[16], packRGBIG(150, 240, 150));
  EXPECT_EQ(imageSOA.nodBIG[17], packRGBIG(150, 240, 240));
  EXPECT_EQ(imageSOA.nodBIG[18], packRGBIG(240, 75, 75));
  EXPECT_EQ(imageSOA.nodBIG[19], packRGBIG(240, 75, 150));
  EXPECT_EQ(imageSOA.nodBIG[20], packRGBIG(240, 75, 240));
  EXPECT_EQ(imageSOA.nodBIG[21], packRGBIG(240, 150, 75));
  EXPECT_EQ(imageSOA.nodBIG[22], packRGBIG(240, 150, 150));
  EXPECT_EQ(imageSOA.nodBIG[23], packRGBIG(240, 150, 240));
  EXPECT_EQ(imageSOA.nodBIG[24], packRGBIG(240, 240, 75));
  EXPECT_EQ(imageSOA.nodBIG[25], packRGBIG(240, 240, 150));
  EXPECT_EQ(imageSOA.nodBIG[26], packRGBIG(240, 240, 240));
}

TEST_F(ImageSOATest, CfAddNodesBIG_Failure) {
  ImageSOA const imageSOA(0, {});
  getImageSOA()->cf_add_nodes_BIG(static_cast<__uint16_t>(NUM_M75),
                                  static_cast<__uint16_t>(NUM_M150),
                                  static_cast<__uint16_t>(NUM_M240));

  EXPECT_EQ(imageSOA.nodBIG.size(), 0) << "Expected nodBIG to be empty when given negative values.";
}

TEST_F(ImageSOATest, CfGenerateGraph_Success) {
  auto graph = getImageSOA()->cf_generate_graph();

  EXPECT_EQ(graph.size(), 7);

  EXPECT_EQ(graph[getImageSOA()->nod[0]].first.size(), 3);
  EXPECT_EQ(graph[getImageSOA()->nod[1]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nod[2]].first.size(), 3);
  EXPECT_EQ(graph[getImageSOA()->nod[3]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nod[4]].first.size(), 5);
  EXPECT_EQ(graph[getImageSOA()->nod[5]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nod[6]].first.size(), 3);
}

TEST_F(ImageSOATest, CfGenerateGraph_Failure) {
  auto graph = getImageSOA()->cf_generate_graph();

  EXPECT_NE(graph.size(), 5);

  EXPECT_NE(graph[getImageSOA()->nod[0]].first.size(), 5);
  EXPECT_NE(graph[getImageSOA()->nod[1]].first.size(), 3);
  EXPECT_NE(graph[getImageSOA()->nod[2]].first.size(), 4);
  EXPECT_NE(graph[getImageSOA()->nod[3]].first.size(), 2);
  EXPECT_NE(graph[getImageSOA()->nod[4]].first.size(), 7);
  EXPECT_NE(graph[getImageSOA()->nod[5]].first.size(), 6);
  EXPECT_NE(graph[getImageSOA()->nod[6]].first.size(), 4);
}

TEST_F(ImageSOATest, CfGenerateGraph2_Success) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph;
  getImageSOA()->cf_generate_graph_2(graph);

  EXPECT_EQ(graph.size(), 7);
  EXPECT_EQ(graph[getImageSOA()->nod[7]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nod[8]].first.size(), 3);
  EXPECT_EQ(graph[getImageSOA()->nod[9]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nod[10]].first.size(), 5);
  EXPECT_EQ(graph[getImageSOA()->nod[11]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nod[12]].first.size(), 7);
  EXPECT_EQ(graph[getImageSOA()->nod[13]].first.size(), 7);
}

TEST_F(ImageSOATest, CfGenerateGraph2_Failure) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph;
  getImageSOA()->cf_generate_graph_2(graph);

  EXPECT_NE(graph.size(), 5);

  EXPECT_NE(graph[getImageSOA()->nod[7]].first.size(), 5);
  EXPECT_NE(graph[getImageSOA()->nod[8]].first.size(), 1);
  EXPECT_NE(graph[getImageSOA()->nod[9]].first.size(), 7);
  EXPECT_NE(graph[getImageSOA()->nod[10]].first.size(), 2);
  EXPECT_NE(graph[getImageSOA()->nod[11]].first.size(), 2);
  EXPECT_NE(graph[getImageSOA()->nod[12]].first.size(), 3);
  EXPECT_NE(graph[getImageSOA()->nod[13]].first.size(), 4);
}

TEST_F(ImageSOATest, CfGenerateGraph3_Success) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph;
  getImageSOA()->cf_generate_graph_3(graph);

  EXPECT_EQ(graph.size(), 8);
  EXPECT_EQ(graph[getImageSOA()->nod[14]].first.size(), 5);
  EXPECT_EQ(graph[getImageSOA()->nod[15]].first.size(), 6);
  EXPECT_EQ(graph[getImageSOA()->nod[16]].first.size(), 5);
  EXPECT_EQ(graph[getImageSOA()->nod[17]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nod[18]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nod[19]].first.size(), 5);
  EXPECT_EQ(graph[getImageSOA()->nod[20]].first.size(), 3);
}

TEST_F(ImageSOATest, CfGenerateGraph3_Failure) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph;
  getImageSOA()->cf_generate_graph_3(graph);

  EXPECT_NE(graph.size(), 4);

  EXPECT_NE(graph[getImageSOA()->nod[14]].first.size(), 1);
  EXPECT_NE(graph[getImageSOA()->nod[15]].first.size(), 2);
  EXPECT_NE(graph[getImageSOA()->nod[16]].first.size(), 3);
  EXPECT_NE(graph[getImageSOA()->nod[17]].first.size(), 7);
  EXPECT_NE(graph[getImageSOA()->nod[18]].first.size(), 5);
  EXPECT_NE(graph[getImageSOA()->nod[19]].first.size(), 8);
  EXPECT_NE(graph[getImageSOA()->nod[20]].first.size(), 7);
}

TEST_F(ImageSOATest, CfGenerateGraph4_Success) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph;
  getImageSOA()->cf_generate_graph_4(graph);

  EXPECT_EQ(graph.size(), 5);
  EXPECT_EQ(graph[getImageSOA()->nod[21]].first.size(), 0);
  EXPECT_EQ(graph[getImageSOA()->nod[22]].first.size(), 5);
  EXPECT_EQ(graph[getImageSOA()->nod[23]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nod[24]].first.size(), 3);
  EXPECT_EQ(graph[getImageSOA()->nod[25]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nod[26]].first.size(), 3);
}

TEST_F(ImageSOATest, CfGenerateGraph4_Failure) {
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph;
  getImageSOA()->cf_generate_graph_4(graph);

  EXPECT_NE(graph.size(), 7);

  EXPECT_NE(graph[getImageSOA()->nod[21]].first.size(), 1);
  EXPECT_NE(graph[getImageSOA()->nod[22]].first.size(), 7);
  EXPECT_NE(graph[getImageSOA()->nod[23]].first.size(), 1);
  EXPECT_NE(graph[getImageSOA()->nod[24]].first.size(), 5);
  EXPECT_NE(graph[getImageSOA()->nod[25]].first.size(), 7);
  EXPECT_NE(graph[getImageSOA()->nod[26]].first.size(), 6);
}

TEST_F(ImageSOATest, CfGenerateGraphBIG_Success) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph;
  getImageSOA()->cf_generate_graph_BIG();

  EXPECT_EQ(graph.size(), 0);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[0]].first.size(), 0);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[1]].first.size(), 0);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[2]].first.size(), 0);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[3]].first.size(), 0);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[4]].first.size(), 0);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[5]].first.size(), 0);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[6]].first.size(), 0);
}

TEST_F(ImageSOATest, CfGenerateGraphBIG_Failure) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph;
  getImageSOA()->cf_generate_graph_BIG();

  EXPECT_NE(graph.size(), 7);
  EXPECT_NE(graph[getImageSOA()->nodBIG[0]].first.size(), 3);
  EXPECT_NE(graph[getImageSOA()->nodBIG[1]].first.size(), 4);
  EXPECT_NE(graph[getImageSOA()->nodBIG[2]].first.size(), 3);
  EXPECT_NE(graph[getImageSOA()->nodBIG[3]].first.size(), 4);
  EXPECT_NE(graph[getImageSOA()->nodBIG[4]].first.size(), 5);
  EXPECT_NE(graph[getImageSOA()->nodBIG[5]].first.size(), 4);
  EXPECT_NE(graph[getImageSOA()->nodBIG[6]].first.size(), 3);
}

TEST_F(ImageSOATest, CfGenerateGraphBIG2_Success) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph =
      getImageSOA()->cf_generate_graph_BIG();

  getImageSOA()->cf_generate_graph_BIG_2(graph);
  EXPECT_EQ(graph.size(), 14);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[7]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[8]].first.size(), 3);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[9]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[10]].first.size(), 5);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[11]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[12]].first.size(), 5);
}

TEST_F(ImageSOATest, CfGenerateGraphBIG2_Failure) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph =
      getImageSOA()->cf_generate_graph_BIG();

  getImageSOA()->cf_generate_graph_BIG_2(graph);

  EXPECT_NE(graph.size(), 12);
  EXPECT_NE(graph[getImageSOA()->nodBIG[7]].first.size(), 5);
  EXPECT_NE(graph[getImageSOA()->nodBIG[8]].first.size(), 4);
  EXPECT_NE(graph[getImageSOA()->nodBIG[9]].first.size(), 3);
  EXPECT_NE(graph[getImageSOA()->nodBIG[10]].first.size(), 2);
  EXPECT_NE(graph[getImageSOA()->nodBIG[11]].first.size(), 1);
  EXPECT_NE(graph[getImageSOA()->nodBIG[12]].first.size(), 0);
}

TEST_F(ImageSOATest, CfGenerateGraphBIG3_Success) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph;
  graph = getImageSOA()->cf_generate_graph_BIG();

  getImageSOA()->cf_generate_graph_BIG_2(graph);

  getImageSOA()->cf_generate_graph_BIG_3(graph);

  EXPECT_EQ(graph.size(), 22);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[14]].first.size(), 5);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[15]].first.size(), 5);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[16]].first.size(), 5);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[17]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[18]].first.size(), 3);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[19]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[20]].first.size(), 3);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[21]].first.size(), 3);
}

TEST_F(ImageSOATest, CfGenerateGraphBIG3_Failure) {

  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph;
  graph = getImageSOA()->cf_generate_graph_BIG();

  getImageSOA()->cf_generate_graph_BIG_2(graph);

  getImageSOA()->cf_generate_graph_BIG_3(graph);

  EXPECT_NE(graph.size(), 2);
  EXPECT_NE(graph[getImageSOA()->nodBIG[14]].first.size(), 6);
  EXPECT_NE(graph[getImageSOA()->nodBIG[15]].first.size(), 4);
  EXPECT_NE(graph[getImageSOA()->nodBIG[16]].first.size(), 3);
  EXPECT_NE(graph[getImageSOA()->nodBIG[17]].first.size(), 2);
  EXPECT_NE(graph[getImageSOA()->nodBIG[18]].first.size(), 4);
  EXPECT_NE(graph[getImageSOA()->nodBIG[19]].first.size(), 6);
  EXPECT_NE(graph[getImageSOA()->nodBIG[20]].first.size(), 5);
  EXPECT_NE(graph[getImageSOA()->nodBIG[21]].first.size(), 4);
}

TEST_F(ImageSOATest, CfGenerateGraphBIG4_Success) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph;
  graph = getImageSOA()->cf_generate_graph_BIG();

  getImageSOA()->cf_generate_graph_BIG_2(graph);

  getImageSOA()->cf_generate_graph_BIG_3(graph);

  getImageSOA()->cf_generate_graph_BIG_4(graph);

  EXPECT_EQ(graph.size(), 27);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[22]].first.size(), 5);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[23]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[24]].first.size(), 3);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[25]].first.size(), 4);
  EXPECT_EQ(graph[getImageSOA()->nodBIG[26]].first.size(), 3);
}

TEST_F(ImageSOATest, CfGenerateGraphBIG4_Failure) {
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph;
  graph = getImageSOA()->cf_generate_graph_BIG();

  getImageSOA()->cf_generate_graph_BIG_2(graph);

  getImageSOA()->cf_generate_graph_BIG_3(graph);

  getImageSOA()->cf_generate_graph_BIG_4(graph);

  EXPECT_NE(graph.size(), 25);
  EXPECT_NE(graph[getImageSOA()->nodBIG[22]].first.size(), 4);
  EXPECT_NE(graph[getImageSOA()->nodBIG[23]].first.size(), 2);
  EXPECT_NE(graph[getImageSOA()->nodBIG[24]].first.size(), 6);
  EXPECT_NE(graph[getImageSOA()->nodBIG[25]].first.size(), 7);
  EXPECT_NE(graph[getImageSOA()->nodBIG[26]].first.size(), 2);
}


TEST_F(ImageSOATest, CfSearchInGraphSmall_SomeColorsInDeleteitems) {
  unordered_map<__uint32_t, __uint32_t> Deleteitems = {
    {packRGB(NUM_10, NUM_40, NUM_70), packRGB(NUM_100, NUM_110, NUM_120)}
  };
  unordered_map<__uint32_t, pair<vector<uint32_t>, vector<__uint32_t>>> const graph = {
    {packRGB(NUM_100, NUM_110, NUM_120),
     {{packRGB(NUM_130, NUM_140, NUM_150)}, {packRGB(NUM_160, NUM_170, NUM_180)}}},
    {packRGB(NUM_130, NUM_140, NUM_150),                                 {{}, {}}}
  };

  ImageSOA::cf_search_in_graph_small(Deleteitems, graph);

  EXPECT_EQ(Deleteitems[packRGB(NUM_10, NUM_40, NUM_70)], packRGB(NUM_160, NUM_170, NUM_180));
}

TEST_F(ImageSOATest, CfSearchInGraphSmall_AllColorsInDeleteitems) {
  unordered_map<__uint32_t, __uint32_t> Deleteitems = {
    {packRGB(NUM_10, NUM_40, NUM_70), packRGB(NUM_100, NUM_110, NUM_120)},
    {packRGB(NUM_20, NUM_50, NUM_80), packRGB(NUM_130, NUM_140, NUM_150)},
    {packRGB(NUM_30, NUM_60, NUM_90), packRGB(NUM_160, NUM_170, NUM_180)}
  };
  unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> const graph = {
    {packRGB(100, 110, 120), {{packRGB(130, 140, 150)}, {packRGB(160, 170, 180)}}},
    {packRGB(130, 140, 150), {{packRGB(160, 170, 180)}, {packRGB(190, 200, 210)}}},
    {packRGB(160, 170, 180), {{packRGB(100, 110, 120)}, {packRGB(220, 230, 240)}}}
  };
  ImageSOA::cf_search_in_graph_small(Deleteitems, graph);
  EXPECT_EQ(Deleteitems[packRGB(NUM_10, NUM_40, NUM_70)], packRGB(NUM_160, NUM_170, NUM_180));
  EXPECT_EQ(Deleteitems[packRGB(NUM_20, NUM_50, NUM_80)], packRGB(NUM_160, NUM_170, NUM_180));
  EXPECT_EQ(Deleteitems[packRGB(NUM_30, NUM_60, NUM_90)], packRGB(NUM_160, NUM_170, NUM_180));
}

TEST_F(ImageSOATest, CfSearchInGraphBIG_SomeColorsInDeleteitems) {
  unordered_map<__uint64_t, __uint64_t> Deleteitems = {
    {packRGBIG(NUM_1000, NUM_4000, NUM_7000), packRGBIG(NUM_10000, NUM_11000, NUM_12000)}
  };
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> const graph = {
    {packRGBIG(NUM_10000, NUM_11000, NUM_12000),
     {{packRGBIG(NUM_13000, NUM_14000, NUM_15000)}, {packRGBIG(NUM_16000, NUM_17000, NUM_18000)}}},
    {packRGBIG(NUM_13000, NUM_14000, NUM_15000),                                         {{}, {}}}
  };

  ImageSOA::cf_search_in_graph_BIG(Deleteitems, graph);

  EXPECT_EQ(Deleteitems[packRGBIG(NUM_1000, NUM_4000, NUM_7000)],
            packRGBIG(NUM_16000, NUM_17000, NUM_18000));
}

TEST_F(ImageSOATest, CfSearchInGraphBIG_AllColorsInDeleteitems) {
  unordered_map<__uint64_t, __uint64_t> Deleteitems = {
    {packRGBIG(NUM_1000, NUM_4000, NUM_7000), packRGBIG(NUM_10000, NUM_11000, NUM_12000)},
    {packRGBIG(NUM_2000, NUM_5000, NUM_8000), packRGBIG(NUM_13000, NUM_14000, NUM_15000)},
    {packRGBIG(NUM_3000, NUM_6000, NUM_9000), packRGBIG(NUM_16000, NUM_17000, NUM_18000)}
  };
  unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> const graph = {
    {packRGBIG(10000, 11000, 12000),
     {{packRGBIG(13000, 14000, 15000)}, {packRGBIG(16000, 17000, 18000)}}},
    {packRGBIG(13000, 14000, 15000),
     {{packRGBIG(16000, 17000, 18000)}, {packRGBIG(19000, 20000, 21000)}}},
    {packRGBIG(16000, 17000, 18000),
     {{packRGBIG(10000, 11000, 12000)}, {packRGBIG(22000, 23000, 24000)}}}
  };

  ImageSOA::cf_search_in_graph_BIG(Deleteitems, graph);

  EXPECT_EQ(Deleteitems[packRGBIG(NUM_1000, NUM_4000, NUM_7000)],
            packRGBIG(NUM_16000, NUM_17000, NUM_18000));
  EXPECT_EQ(Deleteitems[packRGBIG(NUM_2000, NUM_5000, NUM_8000)],
            packRGBIG(NUM_16000, NUM_17000, NUM_18000));
  EXPECT_EQ(Deleteitems[packRGBIG(NUM_3000, NUM_6000, NUM_9000)],
            packRGBIG(NUM_16000, NUM_17000, NUM_18000));
}

TEST_F(ImageSOATest, CpExport_LessThan256Colors) {
  unordered_map<unsigned int, unsigned int> const color_map = {
    {1, 0},
    {2, 1},
    {3, 2},
    {4, 3},
    {5, 4},
    {6, 5},
    {7, 6},
    {8, 7}
  };
  list<unsigned int> const indexes = {0, 1, 2, 3, 4, 5, 6, 7};

  ofstream output_file("output_less_than_256.ppm", ios::binary);
  ImageSOA::cp_export(output_file, color_map, indexes);
  output_file.close();

  ifstream input_file("output_less_than_256.ppm", ios::binary);
  EXPECT_TRUE(input_file.is_open());

  vector<unsigned char> const file_content((istreambuf_iterator<char>(input_file)),
                                           istreambuf_iterator<char>());
  input_file.close();

  vector<unsigned char> const expected_content = {0, 1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(file_content, expected_content);

  if (filesystem::exists("output_less_than_256.ppm")) {
    if (!filesystem::remove("output_less_than_256.ppm")) {
      cerr << "Error deleting file: output_less_than_256.ppm" << '\n';
    }
  }
}

TEST_F(ImageSOATest, CpExport_LessThan65536Colors) {
  unordered_map<unsigned int, unsigned int> const color_map = {
    { 1, 0},
    { 2, 1},
    { 3, 2},
    { 4, 3},
    { 5, 4},
    { 6, 5},
    { 7, 6},
    { 8, 7},
    { 9, 8},
    {10, 9}
  };
  list<unsigned int> const indexes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  ofstream output_file("output_less_than_65536.ppm", ios::binary);
  ImageSOA::cp_export(output_file, color_map, indexes);
  output_file.close();

  ifstream input_file("output_less_than_65536.ppm", ios::binary);
  EXPECT_TRUE(input_file.is_open());

  vector<unsigned char> const file_content((istreambuf_iterator<char>(input_file)),
                                           istreambuf_iterator<char>());
  input_file.close();

  vector<unsigned char> const expected_content = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  EXPECT_EQ(file_content, expected_content);

  if (filesystem::exists("output_less_than_65536.ppm")) {
    if (!filesystem::remove("output_less_than_65536.ppm")) {
      cerr << "Error deleting file: output_less_than_65536.ppm" << '\n';
    }
  }
}

TEST_F(ImageSOATest, CpExport_MoreThan65536Colors) {
  unordered_map<unsigned int, unsigned int> color_map;
  for (unsigned int i = 0; i < NUM_70000; ++i) { color_map[i] = i; }
  list<unsigned int> indexes;
  for (unsigned int i = 0; i < NUM_70000; ++i) { indexes.push_back(i); }

  ofstream output_file("output_more_than_65536.ppm", ios::binary);
  ImageSOA::cp_export(output_file, color_map, indexes);
  output_file.close();

  ifstream input_file("output_more_than_65536.ppm", ios::binary);
  EXPECT_TRUE(input_file.is_open());

  vector<unsigned char> const file_content((istreambuf_iterator<char>(input_file)),
                                           istreambuf_iterator<char>());
  input_file.close();

  vector<unsigned char> expected_content;
  for (unsigned int i = 0; i < NUM_70000; ++i) {
    expected_content.push_back(static_cast<unsigned char>(i % FOTO));
    expected_content.push_back(static_cast<unsigned char>((i >> NUM_8) % FOTO));
    expected_content.push_back(static_cast<unsigned char>((i >> NUM_16) % FOTO));
    expected_content.push_back(static_cast<unsigned char>((i >> NUM_24) % FOTO));
  }
  EXPECT_EQ(file_content, expected_content);

  if (filesystem::exists("output_more_than_65536.ppm")) {
    if (!filesystem::remove("output_more_than_65536.ppm")) {
      cerr << "Error deleting file: output_more_than_65536.ppm" << '\n';
    }
  }
}

TEST_F(ImageSOATest, CpExportBIG_LessThan256Colors) {
  unordered_map<unsigned long int, unsigned int> color_map;
  for (unsigned long int i = 0; i < NUM_255; ++i) { color_map[i] = static_cast<unsigned int>(i); }
  list<unsigned int> indexes;
  for (unsigned int i = 0; i < NUM_10; ++i) { indexes.push_back(i); }

  ofstream output_file("output_less_than_256_colors.ppm", ios::binary);
  testing::internal::CaptureStderr();
  ImageSOA::cp_export_BIG(output_file, color_map, indexes);
  string const output = testing::internal::GetCapturedStderr();
  output_file.close();

  EXPECT_EQ(output, "");
}

TEST_F(ImageSOATest, CpExportBIG_LessThan65536Colors) {
  unordered_map<unsigned long int, unsigned int> color_map;
  for (unsigned long int i = 0; i < NUM_65535; ++i) { color_map[i] = static_cast<unsigned int>(i); }
  list<unsigned int> indexes;
  for (unsigned int i = 0; i < NUM_10; ++i) { indexes.push_back(i); }

  ofstream output_file("output_less_than_65536_colors.ppm", ios::binary);
  testing::internal::CaptureStderr();
  ImageSOA::cp_export_BIG(output_file, color_map, indexes);
  string const output = testing::internal::GetCapturedStderr();
  output_file.close();

  EXPECT_EQ(output, "");
}

TEST_F(ImageSOATest, CpExportBIG_LessThan4294967296Colors) {
  unordered_map<unsigned long int, unsigned int> color_map;
  for (unsigned long int i = 0; i < NUM_1000; ++i) {
    color_map[i] = static_cast<unsigned int>(i);
  }
  list<unsigned int> indexes;
  for (unsigned int i = 0; i < NUM_10; ++i) { indexes.push_back(i); }

  ofstream output_file("output_less_than_4294967296_colors.ppm", ios::binary);
  testing::internal::CaptureStderr();
  ImageSOA::cp_export_BIG(output_file, color_map, indexes);
  string const output = testing::internal::GetCapturedStderr();
  output_file.close();

  EXPECT_EQ(output, "");
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
//NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
//Fin del test
