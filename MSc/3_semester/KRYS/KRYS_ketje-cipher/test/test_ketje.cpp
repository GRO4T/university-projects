#include <stdexcept>
#include <gtest/gtest.h>

#include "monkeyduplex.hpp"
#include "monkeywrap.hpp"
#include "utils.hpp"

#include "monkey.h"
#include "Keccak-f.h"
#include "Ketjev2.h"

/**
 * Test Krys::MonkeyDuplex against KeccakTools implementation.
*/
TEST(TestKetje, TestMonkeyDuplex){
    Krys::MonkeyDuplex md = Krys::MonkeyDuplex(20, 10, 1, 10);
    md.start(BitString("1234"));
    BitString x_step = md.step(BitString("xy"), 16);
    BitString x_stride = md.stride(BitString("xy"), 16);

    IterableTransformation<KeccakPStar> f(200);
    MonkeyDuplex md_ref = MonkeyDuplex(f, 20, 10, 1, 10);
    md_ref.start(BitString("1234"));
    BitString x_step_ref = md_ref.step(BitString("xy"), 16);
    BitString x_stride_ref = md_ref.stride(BitString("xy"), 16);

    EXPECT_EQ(x_step, x_step_ref);
    EXPECT_EQ(x_stride, x_stride_ref);
}

/**
 * Test Krys::MonkeyWrap::wrap against KeccakTools implementation.
*/
TEST(TestKetje, TestMonkeyWrapWrap) {
    std::string key = "1234";
    std::string nonce = "5678";
    std::string associated_data = "abc";
    std::string plaintext = "def";
    uint extract_bits = 8;

    Krys::MonkeyWrap mw(16, 12, 1, 6);
    mw.initialize(BitString(key), BitString(nonce));
    auto [ciphertext, tag] = mw.wrap(BitString(associated_data), BitString(plaintext), extract_bits);

    KetjeJr ketje_ref;
    ketje_ref.initialize(key, nonce);
    std::string tag_ref;
    auto ciphertext_ref = ketje_ref.wrap(associated_data, plaintext, extract_bits, tag_ref);

    EXPECT_EQ(ciphertext, BitString(ciphertext_ref));
    EXPECT_EQ(tag, BitString(tag_ref));
}

/**
 * Test Krys::MonkeyWrap::unwrap against KeccakTools implementation.
*/
TEST(TestKetje, TestMonkeyWrapUnwrap) {
    std::string ciphertext = "\x1c\x44\xce";
    std::string tag = "\x7d";
    std::string key = "1234";
    std::string nonce = "5678";
    std::string associated_data = "abc";
    std::string expected_plaintext = "def";

    Krys::MonkeyWrap mw(16, 12, 1, 6);
    mw.initialize(BitString(key), BitString(nonce));
    auto plaintext = mw.unwrap(BitString(associated_data), BitString(ciphertext), BitString(tag));

    KetjeJr ketje_ref;
    ketje_ref.initialize(key, nonce);
    auto plaintext_ref = ketje_ref.unwrap(associated_data, ciphertext, tag);

    EXPECT_EQ(plaintext, plaintext_ref);
    EXPECT_EQ(plaintext.str(), expected_plaintext);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
