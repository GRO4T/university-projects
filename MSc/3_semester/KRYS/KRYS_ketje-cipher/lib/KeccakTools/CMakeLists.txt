add_library(KeccakTools
  bitstring.cpp
  monkey.cpp
  transformations.cpp
  Keccak-f.cpp
  Ketjev2.cpp
)

target_include_directories(KeccakTools INTERFACE .)
