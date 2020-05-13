# MsgPack

MessagePack implementation for Arduino (compatible with other C++ apps)


## Typical Usage

This library is only for serialize / deserialize.
To send / receive serialized data with `Stream` class, please use [MsgPacketizer](https://github.com/hideakitai/MsgPacketizer).

``` C++
#include <MsgPack.h>

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

## Custom Class Adaptation

To serialize / deserialize custom type you defined, please use `MSGPACK_DEFINE()` macro inside of your class.

``` C++
struct CustomClass
{
    int i;
    float f;
    MsgPack::str_t s;

    MSGPACK_DEFINE(i, f, s);
};
```

After that, you can pack your class completely same as other types.

``` C++
int i;
float f;
MsgPack::str_t s;
CustomClass c;

MsgPack::Packer packer;
packer.encode(i, f, s, c); // -> packer.encode(i, f, s, c.i, c.f, c.s)
```

### Custom Class with Inheritance

Also you can use `MSGPACK_BASE()` macro to pack values of base class.

``` C++
struct Base
{
    int i;
    float f;

    MSGPACK_DEFINE(i, f);
};

struct Derived : public Base
{
    MsgPack::str_t s;

    MSGPACK_DEFINE(s, MSGPACK_BASE(Base)); // -> packer.encode(s, Base::i, Base::f)
};
```


## Type Adaptors

### NIL

- `MsgPack::object::nil_t`

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
- `std::string` or `String(Arduino)` (`MsgPack::str_t`)

### Bin

- `unsigned char*`
- `unsigned char[]`
- `std::vector<char>` (`MsgPack::bin_t<char>`)
- `std::vector<unsigned char>` (`MsgPack::bin_t<unsigned char>`)
- `std::array<char>`
- `std::array<unsigned char>`

### Array

- `T[]`
- `std::vector` (`MsgPack::arr_t<T>`)
- `std::array`
- `std::deque`
- `std::pair`
- `std::tuple`
- `std::list`
- `std::forward_list`
- `std::set`
- `std::multiset`
- `std::unordered_set`
- `std::unordered_multiset`

### Map

- `std::map` (`MsgPack::map_t<T>`)
- `std::multimap`
- `std::unordered_map`
- `std::unordered_multimap`

### Ext

- `MsgPack::object::ext`

### TimeStamp

- `MsgPack::object::timespec`

### N/A

- `std::queue`
- `std::priority_queue`
- `std::bitset`
- `std::stack`


### Note

- `unordered_xxx` cannot be used in all Arduino
- C-style array and pointers are supported only packing.
- for NO-STL Arduino, following types can be used
  - all types of NIL, Bool, Integer, Float, Str, Bin
  - for Array, only `T[]` and `MsgPack::arr_t<T>` (= `arx::vector<T>`) can be used
  - for Map, only `MsgPack::map_t<T, U>` (= `arx::map<T, U>`) can be used
  - for the detail of `arx::xxx`, see [ArxContainer](https://github.com/hideakitai/ArxContainer)



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
And of course you can manage them by defining following macros.
But these default values are optimized for such boards, please be careful not to excess your boards storage/memory.

``` C++
// msgpack serialized binary size
#define MSGPACK_MAX_PACKET_BYTE_SIZE  128
// max size of MsgPack::arr_t
#define MSGPACK_MAX_ARRAY_SIZE          8
// max size of MsgPack::map_t
#define MSGPACK_MAX_MAP_SIZE            8
// msgpack objects size in one packet
#define MSGPACK_MAX_OBJECT_SIZE        32
```

These macros have no effect for STL enabled boards.


### STL library for Arduino Support

For such boards, there are several STL libraries, like [ArduinoSTL](https://github.com/mike-matera/ArduinoSTL), [StandardCPlusPlus](https://github.com/maniacbug/StandardCplusplus), and so on.
But such libraries are mainly based on [uClibc++](https://cxx.uclibc.org/) and it has many lack of function.
I considered to support them but I won't support them unless uClibc++ becomes much better compatibility to standard C++ library.
I reccomend to use low cost but much better performance chip like ESP series.


## Embedded Libraries

- [ArxTypeTraits v0.1.6](https://github.com/hideakitai/ArxTypeTraits)
- [ArxContainer v0.3.4](https://github.com/hideakitai/ArxContainer)
- [DebugLog v0.1.4](https://github.com/hideakitai/DebugLog)
- [TeensyDirtySTLErrorSolution v0.1.0](https://github.com/hideakitai/TeensyDirtySTLErrorSolution)


## Used Inside of

- [MsgPacketizer](https://github.com/hideakitai/MsgPacketizer)


## Reference

- [MessagePack Specification](https://github.com/msgpack/msgpack/blob/master/spec.md)
- [msgpack adaptor](https://github.com/msgpack/msgpack-c/wiki/v2_0_cpp_adaptor)
- [msgpack object](https://github.com/msgpack/msgpack-c/wiki/v2_0_cpp_object)
- [msgpack-c wiki](https://github.com/msgpack/msgpack-c/wiki)


## License

MIT
