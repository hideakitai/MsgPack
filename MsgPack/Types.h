#pragma once
#ifndef HT_SERIAL_MSGPACK_TYPES_H
#define HT_SERIAL_MSGPACK_TYPES_H

#include <stddef.h>
#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <string>
#endif

namespace ht {
namespace serial {
namespace msgpack {

    namespace object
    {
        class NIL
        {
            bool is_nil {false};
            NIL& operator=(const NIL& rhs) { this->is_nil = rhs.is_nil; return *this; }
            NIL& operator=(const bool b) { this->is_nil = b; return *this; }
            bool operator()() const { return this->is_nil; }
        };
    }
    enum class Type : uint8_t
    {
        NA = 0xC1, // never used
        NIL = 0xC0,
        BOOL = 0xC2,
        UINT7 = 0x00, // same as POSITIVE_FIXINT
        UINT8 = 0xCC,
        UINT16 = 0xCD,
        UINT32 = 0xCE,
        UINT64 = 0xCF,
        INT5 = 0xE0, // same as NEGATIVE_FIXINT
        INT8 = 0xD0,
        INT16 = 0xD1,
        INT32 = 0xD2,
        INT64 = 0xD3,
        FLOAT32 = 0xCA,
        FLOAT64 = 0xCB,
        STR5 = 0xA0, // same as FIXSTR
        STR8 = 0xD9,
        STR16 = 0xDA,
        STR32 = 0xDB,
        BIN8 = 0xC4,
        BIN16 = 0xC5,
        BIN32 = 0xC6,
        ARRAY4 = 0x90, // same as FIXARRAY
        ARRAY16 = 0xDC,
        ARRAY32 = 0xDD,
        MAP4 = 0x80, // same as FIXMAP
        MAP16 = 0xDE,
        MAP32 = 0xDF,
        FIXEXT1 = 0xD4,
        FIXEXT2 = 0xD5,
        FIXEXT4 = 0xD6,
        FIXEXT8 = 0xD7,
        FIXEXT16 = 0xD8,
        EXT8 = 0xC7,
        EXT16 = 0xC8,
        EXT32 = 0xC9,
        TIMESTAMP32 = 0xD6,
        TIMESTAMP64 = 0xD7,
        TIMESTAMP96 = 0xC7,

        POSITIVE_FIXINT = 0x00,
        NEGATIVE_FIXINT = 0xE0,
        FIXSTR = 0xA0,
        FIXARRAY = 0x90,
        FIXMAP = 0x80,
    };

    enum class BitMask : uint8_t
    {
        BOOL = 0x01,
        POSITIVE_FIXINT = 0x7F,
        UINT7 = 0x7F, // same as POSITIVE_FIXINT
        NEGATIVE_FIXINT = 0x1F,
        INT5 = 0x1F, // same as NEGATIVE_FIXINT
        FIXSTR = 0x1F,
        STR5 = 0x1F, // same as FIXSTR
        FIXARRAY = 0x0F,
        ARRAY4 = 0x0F, // same as FIXARRAY
        FIXMAP = 0x0F,
        MAP4 = 0x0F, // same as FIXMAP
    };

#ifdef ARDUINO
    using StringType = String;
#else
    using StringType = std::string;
#endif

} // msgpack
} // serial
} // ht

#endif // HT_SERIAL_MSGPACK_TYPES_H
