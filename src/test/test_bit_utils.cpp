#include <gtest/gtest.h>
#include <bitset>
#include <iostream>
#include "bit_utils/bit_utils.h"

using namespace bit_utils;

TEST(BitsinBytes, byte1)
{
    uint8_t my_byte = 0b00001111;
    ASSERT_EQ(get_bits<uint8_t>(&my_byte, 7, 1), 0b1);
}

TEST(BitsinBytes, bit2)
{
    uint8_t my_byte = 0b00001111;
    ASSERT_EQ(get_bits<uint8_t>(&my_byte, 6, 1), 0b1);
}

TEST(BitsinBytes, bit3)
{
    uint8_t my_byte = 0b00001011;
    ASSERT_EQ(get_bits<uint8_t>(&my_byte, 5, 1), 0b0);
}

TEST(BitsinBytes, bits1and2)
{
    uint8_t my_byte = 0b00001011;
    ASSERT_EQ(get_bits<uint8_t>(&my_byte, 6, 2), 0b11);
}

TEST(BitsinBytes, bits1and2and3and4)
{
    uint8_t my_byte = 0b00001011;
    ASSERT_EQ(get_bits<uint8_t>(&my_byte, 4, 4), 0b1011);
}

TEST(BitsinBytes, bits2and3and4)
{
    uint8_t my_byte = 0b00001011;
    ASSERT_EQ(get_bits<uint8_t>(&my_byte, 4, 3), 0b101);
}

TEST(GetBitsMSB, Get_MSB_1)
{
    uint32_t number = 0xFF000000;
    ASSERT_EQ(get_bits<uint8_t>(&number, 0, 1), 1);
}

TEST(GetBitsMSB, Get_LSB_0)
{
    uint32_t number = 0xFF000000;
    ASSERT_EQ(get_bits<uint8_t>(&number, 31, 1), 0);
}

class BitTestFixture: public ::testing::TestWithParam<int>
{
    protected:
        uint8_t gps_preamble = 139;
        uint32_t tlm_word = 583029304;
        uint32_t hov_word = 709937912;
};

TEST_P(BitTestFixture, GPS_Preamble)
{
    int bit = GetParam();
    size_t data_size = sizeof(gps_preamble)*8;
    ASSERT_EQ(get_bits<uint8_t>(&gps_preamble, bit, 1), ((gps_preamble>>(data_size-bit-1)) & 1U));
}

TEST_P(BitTestFixture, TLM_Word)
{
    int bit = GetParam();
    size_t data_size = sizeof(tlm_word)*8;
    ASSERT_EQ(get_bits<uint8_t>(&tlm_word, bit, 1), ((tlm_word>>(data_size-bit-1)) & 1U));
}

TEST_P(BitTestFixture, HOV_Word)
{
    int bit = GetParam();
    size_t data_size = sizeof(hov_word)*8;
    ASSERT_EQ(get_bits<uint8_t>(&hov_word, bit, 1), ((hov_word>>(data_size-bit-1)) & 1U));
}

INSTANTIATE_TEST_CASE_P
(
    MSB,
    BitTestFixture,
    ::testing::Values
    (
        0,1,2,3,4,5,6,7
    )
);

TEST(LeftShift, From128_To256)
{
    uint8_t number = 128;
    ASSERT_EQ(number<<1, 256);
}

TEST(GetBitsMSB, StraddleBytes)
{
    uint8_t numbers[2];
    numbers[0] = 0x01;
    numbers[1] = 0b10000000;
    int start_bit = 7;
    int num_bits = 2;

    uint8_t answer = get_bits<uint8_t>(numbers, start_bit, num_bits);
    ASSERT_EQ(answer, 3);
}

TEST(GetBitsMSB, StraddleByte2)
{
    uint8_t numbers[2];
    numbers[0] = 0x01;
    numbers[1] = 0b11000000;
    int start_bit = 7;
    int num_bits = 3;

    uint8_t answer = get_bits<uint8_t>(numbers, start_bit, num_bits);
    ASSERT_EQ(answer, 7);
}

TEST(GetBitsMSB, StraddleByte3)
{
    uint8_t numbers[2];
    numbers[0] = 0x01;
    numbers[1] = 0b01000000;
    
    uint8_t answer = get_bits<uint8_t>(numbers, 7, 3);
    ASSERT_EQ(answer, 5);
}

TEST(Unsigned_to_Signed, From32769_to_32767)
{
    uint16_t number = 0b1000000000000001;
    ASSERT_EQ(static_cast<int16_t>(number), -32767);
}

TEST(Unsigned_to_Signed, From128_to_128)
{
    uint8_t number = 128;
    ASSERT_EQ(static_cast<int8_t>(number), -128);
}

TEST(IncreaseSignedTypeSize, FiveBitSignedTo_Negative11)
{
    uint8_t number = 0;
    number |= (1UL << (0));
    number |= (1UL << (2));
    number |= (1UL << (4));

    int8_t answer = get_bits<int8_t>(&number, 3, 5);
    ASSERT_EQ(answer, -11);
}

TEST(IncreaseSignedTypeSize, FiveBitSignedTo_5)
{
    uint8_t number = 0;
    number |= 1UL << 0;
    number |= 1UL << 2;

    int8_t answer = get_bits<int8_t>(&number, 3, 5);
    ASSERT_EQ(answer, 5);
}

TEST(IncreaseSignedTypeSize, SixBitsSignedTo_Negative11)
{
    uint8_t six_bits = 0;
    six_bits |= 1UL << 0;
    six_bits |= 1UL << 2;
    six_bits |= 1UL << 4;
    six_bits |= 1UL << 5;
    
    int8_t answer = get_bits<int8_t>(&six_bits, 2, 6);
    ASSERT_EQ(answer, -11);
}