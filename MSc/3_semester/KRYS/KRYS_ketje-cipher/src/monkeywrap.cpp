#include <cassert>
#include "monkeywrap.hpp"

namespace Krys {
    MonkeyWrap::MonkeyWrap(uint rho, uint nstart, uint nstep, uint nstride)
        : rho(rho), duplex(rho + 4, nstart, nstep, nstride) {
        assert(rho < STATE_BITS - 4);
    }

    void MonkeyWrap::initialize(const BitString& key, const BitString& nonce) {
        assert(key.size() < STATE_BITS - 18);
        assert(key.size() % 8 == 0);
        assert(nonce.size() < STATE_BITS - key.size() - 18);
        duplex.start(BitString::keypack(key, key.size() + 16) || nonce);
    }

    std::pair<Ciphertext, Tag> MonkeyWrap::wrap(
        const BitString& associated_data, const BitString& plaintext, uint extract_bits
    ) {
        Ciphertext ciphertext;
        auto A = make_blocks(associated_data);
        auto B = make_blocks(plaintext);

        for (uint i = 0; i + 2 <= A.size(); ++i) {
            duplex.step(A[i] || 0 || 0, 0);
        }
        BitString Z = duplex.step(A[A.size() - 1] || 0 || 1, B[0].size());
        ciphertext = ciphertext || B[0] ^ Z;
        for ( unsigned int i = 0; i + 2 <= B.size(); ++i ) {
            Z = duplex.step(B[i] || 1 || 1, B[i + 1].size());
            ciphertext = ciphertext || B[i + 1] ^ Z;
        }
        BitString tag = duplex.stride(B[B.size() - 1] || 1 || 0, rho);
        while ( tag.size() < extract_bits ) {
            tag = tag || duplex.step(BitString(0), rho);
        }
        tag.truncate(extract_bits);

        return {ciphertext, tag};
    }

    Plaintext MonkeyWrap::unwrap(
        const BitString& associated_data, const BitString& ciphertext, const BitString& tag
    ) {
        Plaintext plaintext;
        Plaintext prev_plaintext_block;
        auto A = make_blocks(associated_data);
        auto C = make_blocks(ciphertext);

        for (uint i = 0; i + 2 <= A.size(); ++i) {
            duplex.step(A[i] || 0 || 0, 0);
        }
        BitString Z = duplex.step(A[A.size() - 1] || 0 || 1, C[0].size());
        plaintext = C[0] ^ Z;
        prev_plaintext_block = plaintext;
        for (uint i = 0; i + 2 <= C.size(); ++i) {
            Z = duplex.step(prev_plaintext_block || 1 || 1, C[i + 1].size());
            prev_plaintext_block = C[i + 1] ^ Z;
            plaintext = plaintext || prev_plaintext_block;
        }
        BitString unwraped_tag = duplex.stride(prev_plaintext_block || 1 || 0, rho);
        while (unwraped_tag.size() < tag.size()) {
            unwraped_tag = unwraped_tag || duplex.step(BitString(0), rho);
        }
        unwraped_tag.truncate(tag.size());

        if (tag != unwraped_tag) {
            throw std::runtime_error("MonkeyWrapUnwrapError: Tags do not match after unwrap.");
        }
        return plaintext;
    }

    std::vector<BitString> MonkeyWrap::make_blocks(const BitString& data) {
        std::vector<BitString> blocks;
        int i;
        for (i = 0; i + rho < data.size(); i += rho) {
            blocks.push_back(BitString::substring(data, i, rho));
        }
        if (i < data.size()) { // last block can be shorter than rho
            blocks.push_back(BitString::substring(data, i, data.size() - i));
        }
        return blocks;
    }
}

