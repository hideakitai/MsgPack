# MsgPack

MessagePack implementation for Arduino (compatible with other C++ apps)


## Typical Usage

``` C++
#include <MsgPack.h>
#include <vector>
#include <map>

// input to msgpack
int i = 123;
float f = 1.23;
String s = "str";
std::vector<int> v {1, 2, 3};
std::map<String, float> m {{"one", 1.1}, {"two", 2.2}, {"three", 3.3}};

// output from msgpack
int ri;
float rf;
String rs;
std::vector<int> rv;
std::map<String, float> rm;

void setup()
{
    delay(2000);
    Serial.begin(115200);
    Serial.println("msgpack test start");

    // encode to msgpack
    MsgPack::Packer packer;
    packer.encode(i, f, s, v, m);

    // decode from msgpack
    MsgPack::Unpacker unpacker;
    unpacker.feed(packer.data(), packer.size());
    unpacker.decode(ri, rf, rs, rv, rm);

    if (i != ri) Serial.println("failed: int");
    if (f != rf) Serial.println("failed: float");
    if (s != rs) Serial.println("failed: string");
    if (v != rv) Serial.println("failed: vector<int>");
    if (m != rm) Serial.println("failed: map<string, int>");

    Serial.println("msgpack test success");
}

void loop()
{
}
```

## Type Adaptors

### NIL

- N/A

### Bool

- `bool`

### Integer

- `char (signed/unsigned)`
- `ints (signed/unsigned)`

### Float

- `float`
- `double`

### Str
- `char*`
- `char[]`
- `std::string`

### Bin
- `unsigned char*`
- `unsigned char[]`
- `std::vector<char>`
- `std::vector<unsigned char>`
- `std::array<char>`
- `std::array<unsigned char>`

### Array
- `T[]`
- `std::vector`
- `std::array`
- `std::deque`
- `std::pair`
- `std::tuple`
- `std::list`
- `std::forward_list`
- `std::set`
- `std::unordered_set` *
- `std::multiset`
- `std::unordered_multiset` *

### Map

- `std::map`
- `std::unordered_map`
- `std::multimap` *
- `std::unordered_multimap` *

### Ext

- N/A

### TimeStamp

- N/A

### N/A
- `std::queue`
- `std::priority_queue`
- `std::bitset`
- `std::stack`


C-style array and pointers are supported only packing.
For unpacking, please use `std::vector<T>` or `std::array<T, N>`.
`unordered_xxx` are not supported in Arduino.

## Other Options

### Packet Data Storage Class Inside

STL is used to handle packet data by default, but for following boards/architectures, [ArxContainer](https://github.com/hideakitai/ArxContainer) is used to store the packet data because STL can not be used for such boards.
The storage size of such boards for max packet binary size and number of msgpack objects are limited.

- AVR
- megaAVR
- SAMD
- SPRESENSE


### Memory Management (for NO-STL Boards)

As mentioned above, for such boards like Arduino Uno, the storage sizes are limited.
And of course you can manage them.

``` C++
// msgpack serialized binary size
#define MSGPACK_PACKER_MAX_BUFFER_BYTE_SIZE 256 // default: 128
// msgpack objects size
#define MSGPACK_UNPACKER_MAX_INDICES_SIZE 32    // default: 16
```

These macros have no effect for STL enabled boards.


## Embedded Libraries

- [ArxTypeTraits v0.1.1](https://github.com/hideakitai/ArxTypeTraits)
- [ArxContainer v0.3.0](https://github.com/hideakitai/ArxContainer)
- [TeensyDirtySTLErrorSolution v0.1.0](https://github.com/hideakitai/TeensyDirtySTLErrorSolution)

## TODO

- suport EXT format and custom class
- suport Timestamp format

## License

MIT
