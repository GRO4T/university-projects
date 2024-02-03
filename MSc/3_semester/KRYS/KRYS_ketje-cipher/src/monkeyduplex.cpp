#include <iostream>
#include <cassert>
#include <cstring>

#include "monkeyduplex.hpp"
#include "utils.hpp"

#include "Keccak-f.h"

using namespace Krys;

MonkeyDuplex::MonkeyDuplex(unsigned int r, unsigned int nstart, unsigned int nstep, unsigned int nstride)
      : r(r),
        nstart(nstart),
        nstep(nstep),
        nstride(nstride){
    assert(nstep < nstride);
    assert(r < STATE_BITS);
};

void MonkeyDuplex::start(BitString I){
    assert(I.size() <= STATE_BITS-2);
    I = I || BitString::pad101(STATE_BITS, I.size());
    memcpy(state, I.array(), STATE_BYTES);
    KeccakPStar(STATE_BITS,nstart)(state);
}

BitString MonkeyDuplex::step(BitString I, size_t extract_bits){
    return genericStep(I, extract_bits, nstep);
}
BitString MonkeyDuplex::stride(BitString I, size_t extract_bits){
    return genericStep(I, extract_bits, nstride);
}

BitString MonkeyDuplex::genericStep(BitString I, size_t extract_bits, unsigned int steps){
    assert(I.size() <= r-2);
    assert(extract_bits <= r);

    I = I || BitString::pad101(r, I.size());
    I = I || BitString::zeroes(STATE_BITS - r);
    const uint8_t *s = I.array();
    for(int i=0; i<STATE_BYTES; i++){
        state[i] ^= s[i];
    }

    KeccakPStar(STATE_BITS,steps)(state);
    return BitString(state, extract_bits);
}

