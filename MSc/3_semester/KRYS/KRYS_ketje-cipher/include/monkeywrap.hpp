#pragma once

#include <utility>
#include <vector>

#include "bitstring.h"
#include "monkeyduplex.hpp"


namespace Krys {
    using Plaintext = BitString;
    using Ciphertext = BitString;
    using Tag = BitString;

    class MonkeyWrap {
      public:
        MonkeyWrap(uint rho, uint nstart, uint nstep, uint nstride);
        void initialize(const BitString& key, const BitString& nonce);
        std::pair<Ciphertext, Tag> wrap(
            const BitString& associated_data, const BitString& plaintext, uint extract_bits
        );
        Plaintext unwrap(
            const BitString& associated_data, const BitString& ciphertext, const BitString& tag
        );
      private:
        uint rho;
        MonkeyDuplex duplex;
        std::vector<BitString> make_blocks(const BitString& data);
    };
}
