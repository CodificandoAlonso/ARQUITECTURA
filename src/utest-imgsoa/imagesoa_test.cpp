#include <iostream>
#include <gtest/gtest.h>
#include "imgsoa/imagesoa.hpp"

class ImageSOATest : public ::testing::Test {
protected:
    ImageSOA * imageSOA = nullptr;

    void SetUp() override {
        std::vector<std::string> args = { "/home/hector/CLionProjects/ARQUITECTURA/src/data/export/cuasimodo.ppm", "/home/hector/CLionProjects/ARQUITECTURA/src/data/export/peneee.cppm", "compress"};
        imageSOA = new ImageSOA(static_cast<int>(args.size()), args);
    }

    void TearDown() override {
        delete imageSOA;
    }

};

TEST_F(ImageSOATest, CompressOperation) {
    // Assuming the input file and output file are set up correctly
    EXPECT_EQ(imageSOA->compress(), 0);
}