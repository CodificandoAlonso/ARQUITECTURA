#include <gtest/gtest.h>
#include "imgsoa/imagesoa.hpp"  // Ensure the correct path to the header file
#include <gsl/gsl>
#include <fstream>
#include <array>
#include <string>
#include <vector>
#include <cstdio>
#include <unordered_map>

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
        std::vector<std::string> const args = {"resize", "test_image.ppm", "output_image.ppm"};
        imageSOA = new ImageSOA(static_cast<int>(args.size()), args);

        test_image_path = "test_image.ppm";
        std::ofstream output_file(test_image_path, std::ios::binary);
        if (!output_file) {
            FAIL() << "Failed to create test image file.";
        }
        output_file << "P6\n" << CIEN << " " << CIEN << "\n" << FOTO-1 << "\n";

        // Write some dummy data to the input file
        for (int i = 0; i < CIEN * CIEN; ++i) {
            output_file.put(static_cast<char>(i % FOTO));
            output_file.put(static_cast<char>((i + 1) % FOTO));
            output_file.put(static_cast<char>((i + 2) % FOTO));
        }
        output_file.close();

    }

    void TearDown() override {
        if (imageSOA != nullptr) {
            delete imageSOA;
            imageSOA = nullptr;
        }
        if (std::ifstream(test_image_path.c_str()).good()) {
            if (std::remove(test_image_path.c_str()) != 0) {
                std::perror("Error deleting file");
            }
        }

        if (std::ifstream("test_image.ppm").good()) {
            if (std::remove("test_image.ppm") != 0) {
                std::perror("Error deleting file");
            }
        }
    }

public:
    // Default constructor
    ImageSOATest() = default;

    // Destructor
    ~ImageSOATest() override {
        delete imageSOA;
    }

    // Copy constructor
    ImageSOATest(const ImageSOATest& other)
        : imageSOA(new ImageSOA(std::move(*other.imageSOA))), test_image_path(other.test_image_path) {}

    // Copy assignment operator
    ImageSOATest& operator=(const ImageSOATest& other) {
        if (this != &other) {
            delete imageSOA;
            imageSOA = new ImageSOA(std::move(*other.imageSOA));
            test_image_path = other.test_image_path;
        }
        return *this;
    }

    // Move constructor
    ImageSOATest(ImageSOATest&& other) noexcept
        : imageSOA(other.imageSOA), test_image_path(std::move(other.test_image_path)) {
        other.imageSOA = nullptr;
    }

    // Move assignment operator
    ImageSOATest& operator=(ImageSOATest&& other) noexcept {
        if (this != &other) {
            delete imageSOA;
            imageSOA = other.imageSOA;
            test_image_path = std::move(other.test_image_path);
            other.imageSOA = nullptr;
        }
        return *this;
    }

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
    EXPECT_EQ(result[0].g, 1);
    EXPECT_EQ(result[0].b, 11);

    EXPECT_EQ(result[1].r, 14);
    EXPECT_EQ(result[1].g, 1);
    EXPECT_EQ(result[1].b, 11);

    EXPECT_EQ(result[2].r, 14);
    EXPECT_EQ(result[2].g, 1);
    EXPECT_EQ(result[2].b, 11);

    EXPECT_EQ(result[3].r, 14);
    EXPECT_EQ(result[3].g, 1);
    EXPECT_EQ(result[3].b, 11);
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

// Test con el metodo read_image_rgb_big que funciona correctamente
TEST_F(ImageSOATest, ReadImageRGBBig_Success) {
    std::ifstream input_file("test_image.ppm", std::ios::binary);
    ASSERT_TRUE(input_file.is_open());

    ImageSOA const imageSOA(0, {});
    soa_rgb_big const image = imageSOA.read_image_rgb_big(input_file);

    ASSERT_EQ(image.r.size(), imageSOA.get_width() * imageSOA.get_height());
    ASSERT_EQ(image.g.size(), imageSOA.get_width() * imageSOA.get_height());
    ASSERT_EQ(image.b.size(), imageSOA.get_width() * imageSOA.get_height());

    input_file.close();
}

// Test con el metodo read_image_rgb_big que no funciona porque no se puede abrir un archivo
TEST_F(ImageSOATest, ReadImageRGBBig_FileNotOpen) {
    std::ifstream input_file("non_existent_file.rgb", std::ios::binary);
    ASSERT_FALSE(input_file.is_open());

    ImageSOA const imageSOA(0, {});
    soa_rgb_big const image = imageSOA.read_image_rgb_big(input_file);

    ASSERT_TRUE(image.r.empty());
    ASSERT_TRUE(image.g.empty());
    ASSERT_TRUE(image.b.empty());
}

// Test con el metodo read_image_rgb_big que no funciona porque hay errores al leer el archivo
TEST_F(ImageSOATest, ReadImageRGBBig_FileReadError) {
    std::ofstream output_file("corrupt_image.rgb", std::ios::binary);
    for (int i = 0; i < NUM_10; ++i) {
        auto value = static_cast<unsigned short>(i);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        output_file.write(reinterpret_cast<char*>(&value), sizeof(value));
    }
    output_file.close();

    std::ifstream input_file("corrupt_image.rgb", std::ios::binary);
    ASSERT_TRUE(input_file.is_open());

    ImageSOA const imageSOA(0, {});
    soa_rgb_big const image = imageSOA.read_image_rgb_big(input_file);

    ASSERT_TRUE(image.r.empty());
    ASSERT_TRUE(image.g.empty());
    ASSERT_TRUE(image.b.empty());

    input_file.close();
}



TEST_F(ImageSOATest, LoadAndMap8_Success) {
    std::ifstream input_file(getTestImagePath(), std::ios::binary);
    ASSERT_TRUE(input_file.is_open());

    auto result = getImageSOA()->cf_load_and_map_8(CIEN, std::move(input_file), CIEN);
    ASSERT_FALSE(result.empty());
}
/*
//Funcion same_bgr_vector funciona
TEST_F(ImageSOATest, SameBGRVector_Success) {
    std::deque<std::pair<__uint32_t, __uint16_t>> const father_vector = {
        {0x000000, 1}, {BLUE, 2}, {GREEN, 3}, {RED, 4}
    };
    ImageSOA const imageSOA(0, {});
    auto result = ImageSOA::same_bgr_vector(father_vector, 1, father_vector.size());

    ASSERT_EQ(result.size(), 4);
    std::cout << "result[0].first: " << result[0].first << '\n';
    std::cout << "result[1].first: " << result[1].first << '\n';
    std::cout << "result[2].first: " << result[2].first << '\n';
    std::cout << "result[3].first: " << result[3].first << '\n';
    ASSERT_EQ(result[0].first, 255);
    ASSERT_EQ(result[1].first, 0);
    ASSERT_EQ(result[2].first, 65280);
    ASSERT_EQ(result[3].first, 16711680);
}

//Funcion same_bgr_vector no funciona por un vector vacío de entrada
TEST_F(ImageSOATest, SameBGRVector_EmptyInput) {
    std::deque<std::pair<__uint32_t, __uint16_t>> const father_vector;
    ImageSOA const imageSOA(0, {});
    auto result = ImageSOA::same_bgr_vector(father_vector, 1, father_vector.size());

    ASSERT_TRUE(result.empty());
}


TEST_F(ImageSOATest, SameBGRVector_InsufficientElements) {
    std::deque<std::pair<__uint32_t, __uint16_t>> const father_vector = {
        {0x000000, 1}
    };
    ImageSOA const imageSOA(0, {});
    auto result = ImageSOA::same_bgr_vector(father_vector, 1, 2);

    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0].first, 0x000000);
}
*/
int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
