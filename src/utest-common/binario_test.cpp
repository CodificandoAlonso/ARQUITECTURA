#include <gtest/gtest.h>
#include <fstream>
#include "common/binario.hpp"

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

TEST_F(BinaryTest, GetDistanceCorrecto) {
  constexpr __uint32_t item_1 = 0x123456;
  constexpr __uint32_t item_2 = 0x654321;
  double const distance = get_distance(item_1, item_2);
  EXPECT_NEAR(distance, 99.614, 0.001);
}

TEST_F(BinaryTest, GetDistanceIncorrecto) {
  constexpr __uint32_t item_1 = 0x123456;
  constexpr __uint32_t item_2 = 0x654321;
  double const distance = get_distance(item_1, item_2);
  EXPECT_NE(distance, 0.0);
}

TEST_F(BinaryTest, GetDistanceZeroCorrecto) {
  constexpr __uint32_t item_1 = 0x000000;
  constexpr __uint32_t item_2 = 0x000000;
  double const distance = get_distance(item_1, item_2);
  EXPECT_EQ(distance, 0.0);
}

TEST_F(BinaryTest, GetDistanceMaxCorrecto) {
  constexpr __uint32_t item_1 = 0xFFFFFF;
  constexpr __uint32_t item_2 = 0x000000;
  double const distance = get_distance(item_1, item_2);
  EXPECT_NEAR(distance, 441.673, 0.001);
}

TEST_F(BinaryTest, GetDistanceBIGCorrecto) {
  constexpr __uint64_t item_1 = 0x123456789ABC;
  constexpr __uint64_t item_2 = 0x654321FEDCBA;
  double const distance = get_distance_BIG(item_1, item_2);
  EXPECT_NEAR(distance, 30298.428, 0.001);
}

TEST_F(BinaryTest, GetDistanceBIGIncorrecto) {
  constexpr __uint64_t item_1 = 0x123456789ABC;
  constexpr __uint64_t item_2 = 0x654321FEDCBA;
  double const distance = get_distance_BIG(item_1, item_2);
  EXPECT_NE(distance, 0.0);
}

TEST_F(BinaryTest, GetDistanceBIGZeroCorrecto) {
  constexpr __uint64_t item_1 = 0x000000000000;
  constexpr __uint64_t item_2 = 0x000000000000;
  double const distance = get_distance_BIG(item_1, item_2);
  EXPECT_EQ(distance, 0.0);
}

TEST_F(BinaryTest, GetDistanceBIGMaxCorrecto) {
  constexpr __uint64_t item_1 = 0xFFFFFFFFFFFF;
  constexpr __uint64_t item_2 = 0x000000000000;
  double const distance = get_distance_BIG(item_1, item_2);
  EXPECT_NEAR(distance, 113509.949, 0.001);
}