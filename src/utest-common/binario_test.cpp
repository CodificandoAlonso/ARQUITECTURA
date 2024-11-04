#include <gtest/gtest.h>
#include <fstream>
#include "common/binario.hpp"

class BinaryTest : public ::testing::Test {
};

TEST_F(BinaryTest, Read_Binary_8Correcto) {
  std::ofstream output("test.bin", std::ios::binary);
  uint8_t value = 0x00;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char*>(&value), sizeof(value));
  output.close();

  std::ifstream input("test.bin", std::ios::binary);
  uint8_t const readValue = read_binary_8(input);
  input.close();
  EXPECT_EQ(readValue, 0x00);

  if (std::remove("test.bin") != 0) {
    std::perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Read_Binary_8Incorrecto) {
  std::ofstream output("test.bin", std::ios::binary);
  uint8_t value = 0x00;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char*>(&value), sizeof(value));
  output.close();

  std::ifstream input("test.bin", std::ios::binary);
  uint8_t const readValue = read_binary_8(input);
  input.close();
  EXPECT_NE(readValue, 0xFF);

  if (std::remove("test.bin") != 0) {
    std::perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Read_Binary_16Correcto) {
  std::ofstream output("test.bin", std::ios::binary);
  uint16_t value = 0x0000;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char*>(&value), sizeof(value));
  output.close();

  std::ifstream input("test.bin", std::ios::binary);
  uint16_t const readValue = read_binary_16(input);
  input.close();
  EXPECT_EQ(readValue, 0x0000);

  if (std::remove("test.bin") != 0) {
    std::perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Read_Binary_16Incorrecto) {
  std::ofstream output("test.bin", std::ios::binary);
  uint16_t value = 0x0000;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char*>(&value), sizeof(value));
  output.close();

  std::ifstream input("test.bin", std::ios::binary);
  uint16_t const readValue = read_binary_16(input);
  input.close();
  EXPECT_NE(readValue, 0xFFFF);

  if (std::remove("test.bin") != 0) {
    std::perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Write_Binary_8Correcto) {
  std::ofstream output("test.bin", std::ios::binary);
  constexpr uint8_t value = 0x00;
  write_binary_8(output, value);
  output.close();

  std::ifstream input("test.bin", std::ios::binary);
  uint8_t readValue = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&readValue), sizeof(readValue));
  input.close();
  EXPECT_EQ(readValue, 0x00);

  if (std::remove("test.bin") != 0) {
    std::perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Write_Binary_8Incorrecto) {
  std::ofstream output("test.bin", std::ios::binary);
  constexpr uint8_t value = 0x00;
  write_binary_8(output, value);
  output.close();

  std::ifstream input("test.bin", std::ios::binary);
  uint8_t readValue = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&readValue), sizeof(readValue));
  input.close();
  EXPECT_NE(readValue, 0xFF);

  if (std::remove("test.bin") != 0) {
    std::perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Write_Binary_16Correcto) {
  std::ofstream output("test.bin", std::ios::binary);
  constexpr uint16_t value = 0x0000;
  write_binary_16(output, value);
  output.close();

  std::ifstream input("test.bin", std::ios::binary);
  uint16_t readValue = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&readValue), sizeof(readValue));
  input.close();
  EXPECT_EQ(readValue, 0x0000);

  if (std::remove("test.bin") != 0) {
    std::perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Write_Binary_16Incorrecto) {
  std::ofstream output("test.bin", std::ios::binary);
  constexpr uint16_t value = 0x0000;
  write_binary_16(output, value);
  output.close();

  std::ifstream input("test.bin", std::ios::binary);
  uint16_t readValue = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&readValue), sizeof(readValue));
  input.close();
  EXPECT_NE(readValue, 0xFFFF);

  if (std::remove("test.bin") != 0) {
    std::perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Write_Binary_32Correcto) {
  std::ofstream output("test.bin", std::ios::binary);
  constexpr uint32_t value = 0x000000;
  write_binary_32(output, value);
  output.close();

  std::ifstream input("test.bin", std::ios::binary);
  uint32_t readValue = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&readValue), sizeof(readValue));
  input.close();
  EXPECT_EQ(readValue, 0x000000);

  if (std::remove("test.bin") != 0) {
    std::perror("Error al borrar el fichero");
  }
}

TEST_F(BinaryTest, Write_Binary_32Incorrecto) {
  std::ofstream output("test.bin", std::ios::binary);
  constexpr uint32_t value = 0x000000;
  write_binary_32(output, value);
  output.close();

  std::ifstream input("test.bin", std::ios::binary);
  uint32_t readValue = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&readValue), sizeof(readValue));
  input.close();
  EXPECT_NE(readValue, 0xFFFFFF);

  if (std::remove("test.bin") != 0) {
    std::perror("Error al borrar el fichero");
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

TEST_F(BinaryTest, Mix3CharCorrecto) {
  constexpr unsigned char ch1 = 0x12; // 00010010
  constexpr unsigned char ch2 = 0x34; // 00110100
  constexpr unsigned char ch3 = 0x56; // 01010110
  std::string const result = mix3char(ch1, ch2, ch3);
  EXPECT_EQ(result, "000100100011010001010110");
}

TEST_F(BinaryTest, Mix3CharIncorrecto) {
  constexpr unsigned char ch1 = 0x12; // 00010010
  constexpr unsigned char ch2 = 0x34; // 00110100
  constexpr unsigned char ch3 = 0x56; // 01010110
  std::string const result = mix3char(ch1, ch2, ch3);
  EXPECT_NE(result, "010101100001001000110100");
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