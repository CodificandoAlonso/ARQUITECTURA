#include <gtest/gtest.h>
#include <fstream>
#include "common/binario.hpp"
#include <cmath>

class BinaryTest : public ::testing::Test {
};

TEST_F(BinaryTest, Read_Binary_8Correcto) {
  ofstream output("test.bin", ios::binary);
  uint8_t value = 0x00;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char*>(&value), sizeof(value));
  output.close();

  ifstream input("test.bin", ios::binary);
  uint8_t const readValue = read_binary_8(input);
  input.close();
  EXPECT_EQ(readValue, 0x00);

  if (remove("test.bin") != 0) {
    perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Read_Binary_8Incorrecto) {
  ofstream output("test.bin", ios::binary);
  uint8_t value = 0x00;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char*>(&value), sizeof(value));
  output.close();

  ifstream input("test.bin", ios::binary);
  uint8_t const readValue = read_binary_8(input);
  input.close();
  EXPECT_NE(readValue, 0xFF);

  if (remove("test.bin") != 0) {
    perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Read_Binary_16Correcto) {
  ofstream output("test.bin", ios::binary);
  uint16_t value = 0x0000;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char*>(&value), sizeof(value));
  output.close();

  ifstream input("test.bin", ios::binary);
  uint16_t const readValue = read_binary_16(input);
  input.close();
  EXPECT_EQ(readValue, 0x0000);

  if (remove("test.bin") != 0) {
    perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Read_Binary_16Incorrecto) {
  ofstream output("test.bin", ios::binary);
  uint16_t value = 0x0000;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char*>(&value), sizeof(value));
  output.close();

  ifstream input("test.bin", ios::binary);
  uint16_t const readValue = read_binary_16(input);
  input.close();
  EXPECT_NE(readValue, 0xFFFF);

  if (remove("test.bin") != 0) {
    perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Write_Binary_8Correcto) {
  ofstream output("test.bin", ios::binary);
  constexpr uint8_t value = 0x00;
  write_binary_8(output, value);
  output.close();

  ifstream input("test.bin", ios::binary);
  uint8_t readValue = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&readValue), sizeof(readValue));
  input.close();
  EXPECT_EQ(readValue, 0x00);

  if (remove("test.bin") != 0) {
    perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Write_Binary_8Incorrecto) {
  ofstream output("test.bin", ios::binary);
  constexpr uint8_t value = 0x00;
  write_binary_8(output, value);
  output.close();

  ifstream input("test.bin", ios::binary);
  uint8_t readValue = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&readValue), sizeof(readValue));
  input.close();
  EXPECT_NE(readValue, 0xFF);

  if (remove("test.bin") != 0) {
    perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Write_Binary_16Correcto) {
  ofstream output("test.bin", ios::binary);
  constexpr uint16_t value = 0x0000;
  write_binary_16(output, value);
  output.close();

  ifstream input("test.bin", ios::binary);
  uint16_t readValue = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&readValue), sizeof(readValue));
  input.close();
  EXPECT_EQ(readValue, 0x0000);

  if (remove("test.bin") != 0) {
    perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Write_Binary_16Incorrecto) {
  ofstream output("test.bin", ios::binary);
  constexpr uint16_t value = 0x0000;
  write_binary_16(output, value);
  output.close();

  ifstream input("test.bin", ios::binary);
  uint16_t readValue = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&readValue), sizeof(readValue));
  input.close();
  EXPECT_NE(readValue, 0xFFFF);

  if (remove("test.bin") != 0) {
    perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Write_Binary_32Correcto) {
  ofstream output("test.bin", ios::binary);
  constexpr uint32_t value = 0x000000;
  write_binary_32(output, value);
  output.close();

  ifstream input("test.bin", ios::binary);
  uint32_t readValue = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&readValue), sizeof(readValue));
  input.close();
  EXPECT_EQ(readValue, 0x000000);

  if (remove("test.bin") != 0) {
    perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Write_Binary_32Incorrecto) {
  ofstream output("test.bin", ios::binary);
  constexpr uint32_t value = 0x000000;
  write_binary_32(output, value);
  output.close();

  ifstream input("test.bin", ios::binary);
  uint32_t readValue = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&readValue), sizeof(readValue));
  input.close();
  EXPECT_NE(readValue, 0xFFFFFF);

  if (remove("test.bin") != 0) {
    perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Merge16Correcto) {
  unsigned char const op1 = 0x12;
  unsigned char const op2 = 0x34;
  unsigned short const result = merge16(op1, op2);
  EXPECT_EQ(result, 0x3412);
}

TEST_F(BinaryTest, Merge16Incorrecto) {
  unsigned char const op1 = 0x12;
  unsigned char const op2 = 0x34;
  unsigned short const result = merge16(op1, op2);
  EXPECT_NE(result, 0x1234);
}

TEST_F(BinaryTest, Swap16Correcto) {
  unsigned short const opr = 0x1234;
  unsigned short const result = swap16(opr);
  EXPECT_EQ(result, 0x3412);
}

TEST_F(BinaryTest, Swap16Incorrecto) {
  unsigned short const opr = 0x1234;
  unsigned short const result = swap16(opr);
  EXPECT_NE(result, 0x1234);
}

TEST_F(BinaryTest, PackRGBIGCorrecto) {
  uint16_t const red = 0x1234;
  uint16_t const grn = 0x5678;
  uint16_t const blu = 0x9ABC;
  uint64_t const packedValue = packRGBIG(red, grn, blu);
  EXPECT_EQ(packedValue, 0x123456789ABC);
}

TEST_F(BinaryTest, PackRGBIGIncorrecto) {
  uint16_t const red = 0x1234;
  uint16_t const grn = 0x5678;
  uint16_t const blu = 0x9ABC;
  uint64_t const packedValue = packRGBIG(red, grn, blu);
  EXPECT_NE(packedValue, 0xFFFFFFFFFFFF);
}

TEST_F(BinaryTest, PackRGBCorrecto) {
  uint8_t const red = 0x12;
  uint8_t const grn = 0x34;
  uint8_t const blu = 0x56;
  uint32_t const packedValue = packRGB(red, grn, blu);
  EXPECT_EQ(packedValue, 0x123456);
}

TEST_F(BinaryTest, PackRGBIncorrecto) {
  uint8_t const red = 0x12;
  uint8_t const grn = 0x34;
  uint8_t const blu = 0x56;
  uint32_t const packedValue = packRGB(red, grn, blu);
  EXPECT_NE(packedValue, 0xFFFFFF);
}

TEST_F(BinaryTest, ExtractRedCorrecto) {
  constexpr uint32_t color = 0x123456;
  uint8_t const red = extractred(color);
  EXPECT_EQ(red, 0x12);
}

TEST_F(BinaryTest, ExtractRedIncorrecto) {
  constexpr uint32_t color = 0x123456;
  uint8_t const red = extractred(color);
  EXPECT_NE(red, 0x34);
}

TEST_F(BinaryTest, ExtractGreenCorrecto) {
  constexpr uint32_t color = 0x123456;
  uint8_t const green = extractgreen(color);
  EXPECT_EQ(green, 0x34);
}

TEST_F(BinaryTest, ExtractGreenIncorrecto) {
  constexpr uint32_t color = 0x123456;
  uint8_t const green = extractgreen(color);
  EXPECT_NE(green, 0x12);
}

TEST_F(BinaryTest, ExtractBlueCorrecto) {
  constexpr uint32_t color = 0x123456;
  uint8_t const blue = extractblue(color);
  EXPECT_EQ(blue, 0x56);
}

TEST_F(BinaryTest, ExtractBlueIncorrecto) {
  constexpr uint32_t color = 0x123456;
  uint8_t const blue = extractblue(color);
  EXPECT_NE(blue, 0x34);
}

TEST_F(BinaryTest, DistanceToBlackIncorrecto) {
  constexpr __uint32_t color = 0x123456;
  EXPECT_NE(distance_to_black(color), 0.0);
}

TEST_F(BinaryTest, ExtractRedBIGCorrecto) {
  constexpr uint64_t color = 0x123456789ABC;
  uint16_t const red = extractredBIG(color);
  EXPECT_EQ(red, 0x1234);
}

TEST_F(BinaryTest, ExtractRedBIGIncorrecto) {
  constexpr uint64_t color = 0x123456789ABC;
  uint16_t const red = extractredBIG(color);
  EXPECT_NE(red, 0x5678);
}

TEST_F(BinaryTest, ExtractBlueBIGCorrecto) {
  constexpr uint64_t color = 0x123456789ABC;
  uint16_t const blue = extractblueBIG(color);
  EXPECT_EQ(blue, 0x9ABC);
}

TEST_F(BinaryTest, ExtractBlueBIGIncorrecto) {
  constexpr uint64_t color = 0x123456789ABC;
  uint16_t const blue = extractblueBIG(color);
  EXPECT_NE(blue, 0x1234);
}

TEST_F(BinaryTest, ExtractGreenBIGCorrecto) {
  constexpr uint64_t color = 0x123456789ABC;
  uint16_t const green = extractgreenBIG(color);
  EXPECT_EQ(green, 0x5678);
}

TEST_F(BinaryTest, ExtractGreenBIGIncorrecto) {
  constexpr uint64_t color = 0x123456789ABC;
  uint16_t const green = extractgreenBIG(color);
  EXPECT_NE(green, 0x1234);
}

TEST_F(BinaryTest, GetDistanceCorrectoIguales) {
  constexpr __uint32_t color1 = 0x123456;
  constexpr __uint32_t color2 = 0x123456;
  EXPECT_DOUBLE_EQ(get_distance(color1, color2), 0.0);
}

TEST_F(BinaryTest, GetDistanceCorrectoNegroBlanco) {
  constexpr __uint32_t color1 = 0x000000;
  constexpr __uint32_t color2 = 0xFFFFFF;
  EXPECT_DOUBLE_EQ(get_distance(color1, color2), sqrt(3 * pow(255, 2)));
}

TEST_F(BinaryTest, GetDistanceCorrectoRojoVerde) {
  constexpr __uint32_t color1 = 0xFF0000;
  constexpr __uint32_t color2 = 0x00FF00;
  EXPECT_DOUBLE_EQ(get_distance(color1, color2), sqrt(2 * pow(255, 2)));
}

TEST_F(BinaryTest, GetDistanceCorrectoRojoAzul) {
  constexpr __uint32_t color1 = 0xFF0000;
  constexpr __uint32_t color2 = 0x0000FF;
  EXPECT_DOUBLE_EQ(get_distance(color1, color2), sqrt(2 * pow(255, 2)));
}

TEST_F(BinaryTest, GetDistanceCorrectoVerdeAzul) {
  constexpr __uint32_t color1 = 0x00FF00;
  constexpr __uint32_t color2 = 0x0000FF;
  EXPECT_DOUBLE_EQ(get_distance(color1, color2), sqrt(2 * pow(255, 2)));
}

TEST_F(BinaryTest, GetDistanceCorrectoRandom) {
  constexpr __uint32_t color1 = 0x123456;
  constexpr __uint32_t color2 = 0x654321;
  EXPECT_DOUBLE_EQ(get_distance(color1, color2), sqrt(pow(0x12 - 0x65, 2) + pow(0x34 - 0x43, 2) + pow(0x56 - 0x21, 2)));
}

TEST_F(BinaryTest, GetDistanceIncorrecto) {
  constexpr __uint32_t color1 = 0x123456;
  constexpr __uint32_t color2 = 0x654321;
  EXPECT_NE(get_distance(color1, color2), 0.0);
}

TEST_F(BinaryTest, GetDistanceBIGCorrectoIguales) {
  constexpr __uint64_t color1 = 0x123456789ABC;
  constexpr __uint64_t color2 = 0x123456789ABC;
  EXPECT_DOUBLE_EQ(get_distance_BIG(color1, color2), 0.0);
}

TEST_F(BinaryTest, GetDistanceBIGCorrectoNegroBlanco) {
  constexpr __uint64_t color1 = 0x000000000000;
  constexpr __uint64_t color2 = 0xFFFFFFFFFFFF;
  EXPECT_DOUBLE_EQ(get_distance_BIG(color1, color2), sqrt(3 * pow(0xFFFF, 2)));
}

TEST_F(BinaryTest, GetDistanceBIGCorrectoRojoVerde) {
  constexpr __uint64_t color1 = 0xFFFF00000000;
  constexpr __uint64_t color2 = 0x0000FFFF0000;
  EXPECT_DOUBLE_EQ(get_distance_BIG(color1, color2), sqrt(2 * pow(0xFFFF, 2)));
}

TEST_F(BinaryTest, GetDistanceBIGCorrectoRojoAzul) {
  constexpr __uint64_t color1 = 0xFFFF00000000;
  constexpr __uint64_t color2 = 0x00000000FFFF;
  EXPECT_DOUBLE_EQ(get_distance_BIG(color1, color2), sqrt(2 * pow(0xFFFF, 2)));
}

TEST_F(BinaryTest, GetDistanceBIGCorrectoVerdeAzul) {
  constexpr __uint64_t color1 = 0x0000FFFF0000;
  constexpr __uint64_t color2 = 0x00000000FFFF;
  EXPECT_DOUBLE_EQ(get_distance_BIG(color1, color2), sqrt(2 * pow(0xFFFF, 2)));
}

TEST_F(BinaryTest, GetDistanceBIGCorrectoRandom) {
  constexpr __uint64_t color1 = 0x123456789ABC;
  constexpr __uint64_t color2 = 0x654321FEDCBA;
  EXPECT_DOUBLE_EQ(get_distance_BIG(color1, color2), sqrt(pow(0x1234 - 0x6543, 2) + pow(0x5678 - 0x21FE, 2) + pow(0x9ABC - 0xDCBA, 2)));
}

TEST_F(BinaryTest, GetDistanceBIGIncorrecto) {
  constexpr __uint64_t color1 = 0x123456789ABC;
  constexpr __uint64_t color2 = 0x654321FEDCBA;
  EXPECT_NE(get_distance_BIG(color1, color2), 0.0);
}