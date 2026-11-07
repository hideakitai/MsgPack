# Native bounds regression

Compile with C++11 or newer, pointing the include paths at this repository and
its Arduino dependencies (ArxContainer, ArxTypeTraits, and DebugLog):

```sh
c++ -std=c++11 -fsanitize=address,undefined -fno-omit-frame-pointer \
  -I. -I/path/to/ArxContainer -I/path/to/ArxTypeTraits -I/path/to/DebugLog \
  tests/unpacker_bounds.cpp -o unpacker_bounds
./unpacker_bounds
```

A zero exit status checks exact-capacity and oversized BIN8/BIN16/BIN32 into
`std::array<uint8_t, 4>`, empty feeds, truncated length/scalar headers, oversized
BIN32 lengths, and reuse of one decoder across feeds. No Arduino hardware is
needed. The fixed-array cases reproduce a host stack-buffer-overflow with the
unpatched decoder under AddressSanitizer.
