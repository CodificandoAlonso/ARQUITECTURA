#include <iostream>
#include <gtest/gtest.h>
#include "imgsoa/imagesoa.hpp"

class ImgSoaTest : public ::testing::Test {
protected:
    ImageSOA soa;
    ImgSoaTest() : soa(0, std::vector<std::string>()) {
        // Cuerpo del constructor (si es necesario)
    }
};

TEST_F(ImgSoaTest, CompressTest) {
    // Configurar el entorno de prueba
    soa.set_input_file("input.ppm");
    soa.set_output_file("output.ppm");

    // Ejecutar la función a probar
    int result = soa.get_compress()();

    // Verificar el resultado
    EXPECT_EQ(result, 0);
}