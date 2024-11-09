#include <iostream>
#include <gtest/gtest.h>
#include "imgsoa/imagesoa.hpp"

class ImageSOATest : public ::testing::Test {
protected:
    ImageSOA *imageSOA;
    std::vector<std::string> args;

    void SetUp() override {
        args = {"imtool-soa", "input.ppm", "output.ppm", "default"};
        imageSOA = new ImageSOA(static_cast<int>(args.size()), args);
    }

    void TearDown() override {
        delete imageSOA;
    }

    void setArgs(const std::vector<std::string>& newArgs) {
        delete imageSOA;
        args = newArgs;
        imageSOA = new ImageSOA(static_cast<int>(args.size()), args);
    }
};



//Test compress
TEST_F(ImageSOATest, CompressOperation) {
    // Assuming the input file and output file are set up correctly
    EXPECT_EQ(imageSOA->compress(), 0);
}

// Test case for the "info" operation

TEST_F(ImageSOATest, ProcessOperationInfo) {
    std::vector<std::string> argv = {"imtool-soa", "input.jpg", "output.jpg", "info"};
    ImageSOA imageSOA(static_cast<int>(argv.size()), argv);
    int result = imageSOA.process_operation();
    EXPECT_EQ(result, 0);
}

TEST_F(ImageSOATest, ProcessOperationResize) {
    vector<std::string> argv = {"imtool-soa", "input.jpg", "output.jpg", "resize"};
    ImageSOA imageSOA(static_cast<int>(argv.size()), argv);
    int result = imageSOA.process_operation();
    EXPECT_EQ(result, 0);
}

// Add more tests for other operations like maxlevel, cutfreq, compress, etc.

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}