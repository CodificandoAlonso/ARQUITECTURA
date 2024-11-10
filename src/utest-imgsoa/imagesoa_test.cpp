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
    image.r = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    image.g = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    image.b = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29};

    std::array<unsigned int, 5> args = {4, 5, 7, 8, 3};

    std::array<rgb_small, 4> result = ImageSOA::rsz_obtain_square_min(image, args);

    EXPECT_EQ(result[0].r, 8);
    EXPECT_EQ(result[0].g, 18);
    EXPECT_EQ(result[0].b, 28);

    EXPECT_EQ(result[1].r, 8);
    EXPECT_EQ(result[1].g, 18);
    EXPECT_EQ(result[1].b, 28);

    EXPECT_EQ(result[2].r, 8);
    EXPECT_EQ(result[2].g, 18);
    EXPECT_EQ(result[2].b, 28);

    EXPECT_EQ(result[3].r, 8);
    EXPECT_EQ(result[3].g, 18);
    EXPECT_EQ(result[3].b, 28);
}

// Test con el metodo rsz_obtain_square_min que no funciona
TEST_F(ImageSOATest, RszObtainSquareMinFailure) {
    soa_rgb_small image;
    image.r = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    image.g = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    image.b = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29};

    std::array<unsigned int, 5> args = {4, 5, 7, 8, 3};

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
    image.r = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    image.g = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    image.b = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29};

    std::array<unsigned int, 5> args = {4, 5, 7, 8, 3};

    std::array<rgb_big, 4> result = ImageSOA::rsz_obtain_square_max(image, args);

    EXPECT_EQ(result[0].r, 8);
    EXPECT_EQ(result[0].g, 18);
    EXPECT_EQ(result[0].b, 28);

    EXPECT_EQ(result[1].r, 8);
    EXPECT_EQ(result[1].g, 18);
    EXPECT_EQ(result[1].b, 28);

    EXPECT_EQ(result[2].r, 8);
    EXPECT_EQ(result[2].g, 18);
    EXPECT_EQ(result[2].b, 28);

    EXPECT_EQ(result[3].r, 8);
    EXPECT_EQ(result[3].g, 18);
    EXPECT_EQ(result[3].b, 28);
}

// Test con el metodo rsz_obtain_square_max que no funciona
TEST_F(ImageSOATest, RszObtainSquareMaxFailure) {
    soa_rgb_big image;
    image.r = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    image.g = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    image.b = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29};

    std::array<unsigned int, 5> args = {4, 5, 7, 8, 3};

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
    std::array<rgb_small, 4> square = {
        rgb_small{10, 20, 30},
        rgb_small{40, 50, 60},
        rgb_small{70, 80, 90},
        rgb_small{100, 110, 120}
    };

    double u_param = 0.5;
    double t_param = 0.5;

    rgb_small result = ImageSOA::rsz_interpolate_min(u_param, square, t_param);

    EXPECT_EQ(result.r, 55);
    EXPECT_EQ(result.g, 65);
    EXPECT_EQ(result.b, 75);
}

// Test con el metodo rsz_interpolate_min que no funciona
TEST_F(ImageSOATest, RszInterpolateMinFailure) {
    std::array<rgb_small, 4> square = {
        rgb_small{10, 20, 30},
        rgb_small{40, 50, 60},
        rgb_small{70, 80, 90},
        rgb_small{100, 110, 120}
    };

    double u_param = 0.5;
    double t_param = 0.5;

    rgb_small result = ImageSOA::rsz_interpolate_min(u_param, square, t_param);

    // Intentionally incorrect expected values to cause the test to fail
    EXPECT_NE(result.r, 10);
    EXPECT_NE(result.g, 20);
    EXPECT_NE(result.b, 30);
}

// Test con el metodo rsz_interpolate_max que funciona correctamente
TEST_F(ImageSOATest, RszInterpolateMaxSuccess) {
    std::array<rgb_big, 4> square = {
        rgb_big{10, 20, 30},
        rgb_big{40, 50, 60},
        rgb_big{70, 80, 90},
        rgb_big{100, 110, 120}
    };

    double u_param = 0.5;
    double t_param = 0.5;

    rgb_big result = ImageSOA::rsz_interpolate_max(u_param, square, t_param);

    EXPECT_EQ(result.r, 55);
    EXPECT_EQ(result.g, 65);
    EXPECT_EQ(result.b, 75);
}

// Test con el metodo rsz_interpolate_max que no funciona por un error de acceso a memoria
TEST_F(ImageSOATest, RszInterpolateMaxOutOfBounds) {
    std::array<rgb_big, 4> square = {
        rgb_big{10, 20, 30},
        rgb_big{40, 50, 60},
        rgb_big{70, 80, 90},
        rgb_big{0, 0, 0}  // Adding a dummy element to make it 4 elements
    };

    double u_param = 0.5;
    double t_param = 0.5;

    // This should not cause an out-of-bounds access now
    rgb_big result = ImageSOA::rsz_interpolate_max(u_param, square, t_param);

    // Check for incorrect results due to invalid array size
    EXPECT_NE(result.r, 55);
    EXPECT_NE(result.g, 65);
    EXPECT_NE(result.b, 75);
}

// Test con el metodo rsz_interpolate_max que no funciona por un valor de u_param invalido
TEST_F(ImageSOATest, RszInterpolateMaxInvalidUParam) {
    std::array<rgb_big, 4> square = {
        rgb_big{10, 20, 30},
        rgb_big{40, 50, 60},
        rgb_big{70, 80, 90},
        rgb_big{100, 110, 120}
    };

    double u_param = 1.5;  // Invalid value
    double t_param = 0.5;

    rgb_big result = ImageSOA::rsz_interpolate_max(u_param, square, t_param);

    // Check for incorrect results due to invalid u_param
    EXPECT_NE(result.r, 55);
    EXPECT_NE(result.g, 65);
    EXPECT_NE(result.b, 75);
}

// Test con el metodo rsz_interpolate_max que no funciona por un valor de t_param invalido
TEST_F(ImageSOATest, RszInterpolateMaxInvalidTParam) {
    std::array<rgb_big, 4> square = {
        rgb_big{10, 20, 30},
        rgb_big{40, 50, 60},
        rgb_big{70, 80, 90},
        rgb_big{100, 110, 120}
    };

    double u_param = 0.5;
    double t_param = 1.5;  // Invalid value

    rgb_big result = ImageSOA::rsz_interpolate_max(u_param, square, t_param);

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
