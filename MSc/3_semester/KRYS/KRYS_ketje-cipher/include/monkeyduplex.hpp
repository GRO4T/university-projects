#pragma once

#include <string>
#include <cstdint>

#include "bitstring.h"

// Ketje Jr: keccak-pstar[200]

#ifdef KETJE_JR
    #define STATE_BITS 200
#elif KETJE_SR
    #define STATE_BITS 400
#elif KETJE_MINOR
    #define STATE_BITS 800
#elif KETJE_MAJOR
    #define STATE_BITS 1600
#endif

#define STATE_BYTES (STATE_BITS/8)

namespace Krys{
    class MonkeyDuplex{
      public:
        MonkeyDuplex(unsigned int r, unsigned int nstart, unsigned int nstep, unsigned int nstride);
        void start(BitString I);
        BitString step(BitString I, size_t extract_bits);
        BitString stride(BitString I, size_t extract_bits);
      private:
        uint8_t state[STATE_BYTES];
        unsigned int r;
        unsigned int nstart;
        unsigned int nstep;
        unsigned int nstride;
        BitString genericStep(BitString I, size_t extract_bits, unsigned int steps);
    };
}
