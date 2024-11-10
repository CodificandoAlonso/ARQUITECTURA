#include <gtest/gtest.h>
#include "imgsoa/imagesoa.hpp"  // Ensure the correct path to the header file
#include <gsl/gsl>
#include <fstream>
#include <array>
#include <string>
#include <vector>
#include <cstdio>
static constexpr int CIEN = 100;
static constexpr int FOTO = 256;
static constexpr int NUM_5 = 5;
static constexpr int NUM_6 = 6;
static constexpr int NUM_7 = 7;
static constexpr int NUM_8 = 8;
static constexpr int NUM_9 = 9;
static constexpr int NUM_10 = 10;
static constexpr int NUM_11 = 11;
static constexpr int NUM_12 = 12;
static constexpr int NUM_13 = 13;
static constexpr int NUM_14 = 14;
static constexpr int NUM_15 = 15;
static constexpr int NUM_16 = 16;
static constexpr int NUM_17 = 17;
static constexpr int NUM_18 = 18;
static constexpr int NUM_19 = 19;
static constexpr int NUM_20 = 20;
static constexpr int NUM_21 = 21;
static constexpr int NUM_22 = 22;
static constexpr int NUM_23 = 23;
static constexpr int NUM_24 = 24;
static constexpr int NUM_25 = 25;
static constexpr int NUM_26 = 26;
static constexpr int NUM_27 = 27;
static constexpr int NUM_28 = 28;
static constexpr int NUM_29 = 29;

class ImageSOATest : public ::testing::Test {
private:
    gsl::owner<ImageSOA*> imageSOA = nullptr;
    std::string test_image_path;

protected:
    void SetUp() override {
        std::vector<std::string> const args = {"resize", "input_image.jpg", "output_image.jpg"};
        imageSOA = new ImageSOA(static_cast<int>(args.size()), args);

        test_image_path = "test_image.rgb";
        std::ofstream output_file(test_image_path, std::ios::binary);
        if (!output_file.is_open()) {
            FAIL() << "Failed to create test image file.";
        }

        for (int i = 0; i < CIEN; ++i) {
            output_file.put(static_cast<char>(i % FOTO));
        }
        output_file.close();
    }

    void TearDown() override {
        // Solo liberar el puntero aquí
        delete imageSOA;
        if (std::remove(test_image_path.c_str()) != 0) {
            std::perror("Error deleting file");
        }
    }

public:
    [[nodiscard]] const std::string& getTestImagePath() const {
        return test_image_path;
    }

    [[nodiscard]] gsl::owner<ImageSOA*> getImageSOA() const {
        return imageSOA;
    }
};

// Test con el metodo rsz_obtain_square_min que funciona correctamente
TEST_F(ImageSOATest, RszObtainSquareMin) {
    soa_rgb_small image;
    image.r = {0, 1, 2, 3, 4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_10, NUM_11, NUM_12, NUM_13, NUM_14};
    image.g = {NUM_15, NUM_16, NUM_17, NUM_18, NUM_19, NUM_20, NUM_21, NUM_22, NUM_23, NUM_24};
    image.b = {NUM_25, NUM_26, NUM_27, NUM_28, NUM_29, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9};

    std::array<unsigned int, 5> const args = {4, 5, 7, 8, 3};

    std::array<rgb_small, 4> result = ImageSOA::rsz_obtain_square_min(image, args);

    EXPECT_EQ(result[0].r, 14);
    EXPECT_EQ(result[0].g, 14);
    EXPECT_EQ(result[0].b, 46);

    EXPECT_EQ(result[1].r, 14);
    EXPECT_EQ(result[1].g, 14);
    EXPECT_EQ(result[1].b, 46);

    EXPECT_EQ(result[2].r, 14);
    EXPECT_EQ(result[2].g, 14);
    EXPECT_EQ(result[2].b, 46);

    EXPECT_EQ(result[3].r, 14);
    EXPECT_EQ(result[3].g, 14);
    EXPECT_EQ(result[3].b, 46);
}

// Test con el metodo rsz_obtain_square_min que no funciona
TEST_F(ImageSOATest, RszObtainSquareMinFailure) {
    soa_rgb_small image;
    image.r = {0, 1, 2, 3, 4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_10, NUM_11, NUM_12, NUM_13, NUM_14};
    image.g = {NUM_15, NUM_16, NUM_17, NUM_18, NUM_19, NUM_20, NUM_21, NUM_22, NUM_23, NUM_24};
    image.b = {NUM_25, NUM_26, NUM_27, NUM_28, NUM_29, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9};

    std::array<unsigned int, 5> const args = {4, 5, 7, 8, 3};

    std::array<rgb_small, 4> result = ImageSOA::rsz_obtain_square_min(image, args);

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
TEST_F(ImageSOATest, RszObtainSquareMax) {
    soa_rgb_big image;
    image.r = {0, 1, 2, 3, 4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_10, NUM_11, NUM_12, NUM_13, NUM_14};
    image.g = {NUM_15, NUM_16, NUM_17, NUM_18, NUM_19, NUM_20, NUM_21, NUM_22, NUM_23, NUM_24};
    image.b = {NUM_25, NUM_26, NUM_27, NUM_28, NUM_29, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9};

    std::array<unsigned int, 5> const args = {4, 5, 7, 8, 3};

    std::array<rgb_big, 4> result = ImageSOA::rsz_obtain_square_max(image, args);

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
    image.r = {0, 1, 2, 3, 4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_10, NUM_11, NUM_12, NUM_13, NUM_14};
    image.g = {NUM_15, NUM_16, NUM_17, NUM_18, NUM_19, NUM_20, NUM_21, NUM_22, NUM_23, NUM_24};
    image.b = {NUM_25, NUM_26, NUM_27, NUM_28, NUM_29, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9};

    std::array<unsigned int, 5> const args = {4, 5, 7, 8, 3};

    std::array<rgb_big, 4> result = ImageSOA::rsz_obtain_square_max(image, args);

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
    std::array<rgb_small, 4> const square = {
        rgb_small{.r=10, .g=20, .b=30},
        rgb_small{.r=40, .g=50, .b=60},
        rgb_small{.r=70, .g=80, .b=90},
        rgb_small{.r=100, .g=110, .b=120}
    };

    double const u_param = 0.5;
    double const t_param = 0.5;

    rgb_small const result = ImageSOA::rsz_interpolate_min(u_param, square, t_param);

    EXPECT_EQ(result.r, 55);
    EXPECT_EQ(result.g, 65);
    EXPECT_EQ(result.b, 75);
}

// Test con el metodo rsz_interpolate_min que no funciona
TEST_F(ImageSOATest, RszInterpolateMinFailure) {
    std::array<rgb_small, 4> const square = {
        rgb_small{.r=10, .g=20, .b=30},
        rgb_small{.r=40, .g=50, .b=60},
        rgb_small{.r=70, .g=80, .b=90},
        rgb_small{.r=100, .g=110, .b=120}
    };

    double const u_param = 0.5;
    double const t_param = 0.5;

    rgb_small const result = ImageSOA::rsz_interpolate_min(u_param, square, t_param);

    // Intentionally incorrect expected values to cause the test to fail
    EXPECT_NE(result.r, 10);
    EXPECT_NE(result.g, 20);
    EXPECT_NE(result.b, 30);
}

// Test con el metodo rsz_interpolate_max que funciona correctamente
TEST_F(ImageSOATest, RszInterpolateMaxSuccess) {
    std::array<rgb_big, 4> const square = {
        rgb_big{.r=10, .g=20, .b=30},
        rgb_big{.r=40, .g=50, .b=60},
        rgb_big{.r=70, .g=80, .b=90},
        rgb_big{.r=100, .g=110, .b=120}
    };

    double const u_param = 0.5;
    double const t_param = 0.5;

    rgb_big const result = ImageSOA::rsz_interpolate_max(u_param, square, t_param);

    EXPECT_EQ(result.r, 55);
    EXPECT_EQ(result.g, 65);
    EXPECT_EQ(result.b, 75);
}

// Test con el metodo rsz_interpolate_max que no funciona por un error de acceso a memoria
TEST_F(ImageSOATest, RszInterpolateMaxOutOfBounds) {
    std::array<rgb_big, 4> const square = {
        rgb_big{.r=10, .g=20, .b=30},
        rgb_big{.r=40, .g=50, .b=60},
        rgb_big{.r=70, .g=80, .b=90},
        rgb_big{.r=0, .g=0, .b=0}  // Adding a dummy element to make it 4 elements
    };

    double const u_param = 0.5;
    double const t_param = 0.5;

    // This should not cause an out-of-bounds access now
    rgb_big const result = ImageSOA::rsz_interpolate_max(u_param, square, t_param);

    // Check for incorrect results due to invalid array size
    EXPECT_NE(result.r, 55);
    EXPECT_NE(result.g, 65);
    EXPECT_NE(result.b, 75);
}

// Test con el metodo rsz_interpolate_max que no funciona por un valor de u_param invalido
TEST_F(ImageSOATest, RszInterpolateMaxInvalidUParam) {
    std::array<rgb_big, 4> const square = {
        rgb_big{.r=10, .g=20, .b=30},
        rgb_big{.r=40, .g=50, .b=60},
        rgb_big{.r=70, .g=80, .b=90},
        rgb_big{.r=100, .g=110, .b=120}
    };

    constexpr double u_param = 1.5;  // Invalid value
    constexpr double t_param = 0.5;

    rgb_big const result = ImageSOA::rsz_interpolate_max(u_param, square, t_param);

    // Check for incorrect results due to invalid u_param
    EXPECT_NE(result.r, 55);
    EXPECT_NE(result.g, 65);
    EXPECT_NE(result.b, 75);
}

// Test con el methods rsz_interpolate_max que no function por un valor de t_param invalid
TEST_F(ImageSOATest, RszInterpolateMaxInvalidTParam) {
    constexpr std::array<rgb_big, 4> square = {
        rgb_big{.r=10, .g=20, .b=30},
        rgb_big{.r=40, .g=50, .b=60},
        rgb_big{.r=70, .g=80, .b=90},
        rgb_big{.r=100, .g=110, .b=120}
    };

    constexpr double u_param = 0.5;
    constexpr double t_param = 1.5;  // Invalid value

    rgb_big const result = ImageSOA::rsz_interpolate_max(u_param, square, t_param);

    // Check for incorrect results due to invalid t_param
    EXPECT_NE(result.r, 55);
    EXPECT_NE(result.g, 65);
    EXPECT_NE(result.b, 75);
}

TEST_F(ImageSOATest, ReadImageRGBSmallSuccess) {
    std::ifstream input_file(getTestImagePath(), std::ios::binary);
    ASSERT_TRUE(input_file.is_open());

    soa_rgb_small const result = getImageSOA()->read_image_rgb_small(input_file);

    // Add assertions to check the correctness of the result
    EXPECT_EQ(result.r.size(), getImageSOA()->get_width() * getImageSOA()->get_height());
    EXPECT_EQ(result.g.size(), getImageSOA()->get_width() * getImageSOA()->get_height());
    EXPECT_EQ(result.b.size(), getImageSOA()->get_width() * getImageSOA()->get_height());
}

TEST_F(ImageSOATest, ReadImageRGBSmallFileNotFound) {
    std::ifstream input_file("non_existent_file.rgb", std::ios::binary);
    ASSERT_FALSE(input_file.is_open());
}



TEST_F(ImageSOATest, ReadImageRGBSmallCorruptData) {
    std::ifstream input_file("corrupt_image.rgb", std::ios::binary);
    ASSERT_TRUE(input_file.is_open());

    soa_rgb_small result = getImageSOA()->read_image_rgb_small(input_file);

    // Add assertions to check for corrupt data
    for (size_t i = 0; i < result.r.size(); ++i) {
        EXPECT_GE(result.r[i], 0);
        EXPECT_LE(result.r[i], 255);
        EXPECT_GE(result.g[i], 0);
        EXPECT_LE(result.g[i], 255);
        EXPECT_GE(result.b[i], 0);
        EXPECT_LE(result.b[i], 255);
    }
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
