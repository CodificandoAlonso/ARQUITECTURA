#include <iostream>
#include <gtest/gtest.h>
#include "imgsoa/imagesoa.hpp"

class ImageSOATest : public ::testing::Test {
protected:
    ImageSOA* imageSOA;
    ImageSOATest() : imageSOA(nullptr) {}

    void SetUp() override {
        std::vector<std::string> args = {"program", "info", "input.ppm", "output.ppm"};
        imageSOA = new ImageSOA(static_cast<int>(args.size()), args);
    }

    void TearDown() override {
        delete imageSOA;
    }
};

TEST_F(ImageSOATest, ProcessOperationInfo) {
    EXPECT_EQ(imageSOA->get_optype(), "info");
    EXPECT_EQ(imageSOA->process_operation(), 0);
};