#include <Arduino.h>

// #define TEST_BINARY

#ifdef TEST_BINARY
#define MSGPACK_MAX_PACKET_BYTE_SIZE 128
#define MSGPACK_MAX_ARRAY_SIZE 16
#define MSGPACK_MAX_MAP_SIZE 16
#define MSGPACK_MAX_OBJECT_SIZE 64
#else
#define MSGPACK_MAX_PACKET_BYTE_SIZE 300
#define MSGPACK_MAX_ARRAY_SIZE 16
#define MSGPACK_MAX_MAP_SIZE 16
#define MSGPACK_MAX_OBJECT_SIZE 64
#endif

// #define MSGPACK_DEBUGLOG_ENABLE
#include <MsgPack.h>

namespace debug {
void assert(bool b, const char* file, int line, const char* func, const char* expr) {
    // while (!b)
    if (!b) {
        Serial.print("[ASSERT] ");
        Serial.print(file);
        Serial.print(":");
        Serial.print(line);
        Serial.print(":");
        Serial.print(func);
        Serial.print("() : ");
        Serial.println(expr);
    }
}
}  // namespace debug

#ifndef assert
#define assert(b) ::debug::assert((b), __FILE__, __LINE__, __func__, #b)
#endif

const char str0[2] = "A";
const char str5[31] = "123456789012";
const char str8[255] PROGMEM = "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012";
const char str16[257] PROGMEM = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789001234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234";

uint8_t intu7 = 12;
uint8_t intu8 = 234;
uint16_t intu16 = 12345;
uint32_t intu32 = 1234567891;
uint64_t intu64 = 1234567891234567891;
int8_t int5 = -12;
int8_t int8 = -123;
int16_t int16 = -12345;
int32_t int32 = -1234567891;
int64_t int64 = -1234567891234567891;

float f = -12345.6789f;
double d = -12345.6789d;

uint8_t arr_size4 = 5;

uint8_t map_size4 = 3;

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("msgpack test start");

#ifndef TEST_BINARY
    {
        MsgPack::Packer packer;

        // ---------- NIL format family ----------

        packer.packNil();

        // ---------- BOOL format family ----------

        packer.packBool(false);
        packer.packBool(true);

        bool t = true;
        bool f = false;
        packer.packBool(t);
        packer.packBool(f);

        // wrappers
        packer.pack(false);
        packer.pack(true);
        packer.pack(t);
        packer.pack(f);

        // ---------- unpack ----------

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        assert(unpacker.unpackNil() == true);
        assert(unpacker.unpackBool() == false);
        assert(unpacker.unpackBool() == true);
        assert(unpacker.unpackBool() == true);
        assert(unpacker.unpackBool() == false);
        assert(unpacker.unpackBool() == false);
        assert(unpacker.unpackBool() == true);
        assert(unpacker.unpackBool() == true);
        assert(unpacker.unpackBool() == false);
    }

    {
        MsgPack::Packer packer;

        // ---------- INT format family ----------

        packer.packUInt7(12);
        packer.packUInt8(234);
        packer.packUInt16(12345);
        packer.packUInt32(1234567891);
        packer.packUInt64(1234567891234567891);
        packer.packInt5(-12);
        packer.packInt8(-123);
        packer.packInt16(-12345);
        packer.packInt32(-1234567891);
        packer.packInt64(-1234567891234567891);

        // wrappers
        packer.pack((uint8_t)12);
        packer.pack((uint8_t)234);
        packer.pack((uint16_t)12345);
        packer.pack((uint32_t)1234567891);
        packer.pack((uint64_t)1234567891234567891);
        packer.pack((int8_t)-12);
        packer.pack((int8_t)-123);
        packer.pack((int16_t)-12345);
        packer.pack((int32_t)-1234567891);
        packer.pack((int64_t)-1234567891234567891);

        // ---------- FLOAT format family ----------

        packer.packFloat32(-12345.6789f);
        packer.packFloat64(-12345.6789d);

        // wrappers
        packer.pack(-12345.6789f);
        packer.pack(-12345.6789d);

        // ---------- INT format family ----------

        packer.packUInt7(intu7);
        packer.packUInt8(intu8);
        packer.packUInt16(intu16);
        packer.packUInt32(intu32);
        // TODO:
        packer.packUInt64(intu64);
        packer.packInt5(int5);
        packer.packInt8(int8);
        packer.packInt16(int16);
        packer.packInt32(int32);
        // TODO:
        packer.packInt64(int64);

        // wrappers
        packer.pack(intu7);
        packer.pack(intu8);
        packer.pack(intu16);
        packer.pack(intu32);
        // TODO:
        packer.pack(intu64);
        packer.pack(int5);
        packer.pack(int8);
        packer.pack(int16);
        packer.pack(int32);
        // TODO:
        packer.pack(int64);

        // ---------- FLOAT format family ----------

        packer.packFloat32(f);
        packer.packFloat64(d);

        // wrappers
        packer.pack(f);
        packer.pack(d);

        // ---------- unpack ----------

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        assert(unpacker.unpackUInt7() == intu7);
        assert(unpacker.unpackUInt8() == intu8);
        assert(unpacker.unpackUInt16() == intu16);
        assert(unpacker.unpackUInt32() == intu32);
        assert(unpacker.unpackUInt64() == intu64);
        assert(unpacker.unpackInt5() == int5);
        assert(unpacker.unpackInt8() == int8);
        assert(unpacker.unpackInt16() == int16);
        assert(unpacker.unpackInt32() == int32);
        assert(unpacker.unpackInt64() == int64);
        assert(unpacker.unpackUInt7() == intu7);
        assert(unpacker.unpackUInt8() == intu8);
        assert(unpacker.unpackUInt16() == intu16);
        assert(unpacker.unpackUInt32() == intu32);
        assert(unpacker.unpackUInt64() == intu64);
        assert(unpacker.unpackInt5() == int5);
        assert(unpacker.unpackInt8() == int8);
        assert(unpacker.unpackInt16() == int16);
        assert(unpacker.unpackInt32() == int32);
        assert(unpacker.unpackInt64() == int64);
        assert(unpacker.unpackFloat32() == f);
        assert(unpacker.unpackFloat64() == d);
        assert(unpacker.unpackFloat32() == f);
        assert(unpacker.unpackFloat64() == d);

        assert(unpacker.unpackUInt7() == intu7);
        assert(unpacker.unpackUInt8() == intu8);
        assert(unpacker.unpackUInt16() == intu16);
        assert(unpacker.unpackUInt32() == intu32);
        assert(unpacker.unpackUInt64() == intu64);
        assert(unpacker.unpackInt5() == int5);
        assert(unpacker.unpackInt8() == int8);
        assert(unpacker.unpackInt16() == int16);
        assert(unpacker.unpackInt32() == int32);
        assert(unpacker.unpackInt64() == int64);
        assert(unpacker.unpackUInt7() == intu7);
        assert(unpacker.unpackUInt8() == intu8);
        assert(unpacker.unpackUInt16() == intu16);
        assert(unpacker.unpackUInt32() == intu32);
        assert(unpacker.unpackUInt64() == intu64);
        assert(unpacker.unpackInt5() == int5);
        assert(unpacker.unpackInt8() == int8);
        assert(unpacker.unpackInt16() == int16);
        assert(unpacker.unpackInt32() == int32);
        assert(unpacker.unpackInt64() == int64);
        assert(unpacker.unpackFloat32() == f);
        assert(unpacker.unpackFloat64() == d);
        assert(unpacker.unpackFloat32() == f);
        assert(unpacker.unpackFloat64() == d);
    }
    {
        MsgPack::Packer packer;

        // ---------- STR format family ----------

        packer.packString5("A");
        packer.packString5("123456789012");
        packer.packString8(F("01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012"));
        packer.packString16(F("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789001234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234"));
        // wrappers
        packer.pack("A");
        packer.pack("123456789012");
        packer.pack(F("01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012"));
        packer.pack(F("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789001234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234"));

        packer.packString5(str0);
        packer.packString5(str5);
        packer.packString8(str8);
        packer.packString16(str16);
        // wrappers
        packer.pack(str0);
        packer.pack(str5);
        packer.pack(str8);
        packer.pack(str16);

        packer.packString5(MsgPack::str_t(str0));
        packer.packString5(MsgPack::str_t(str5));
        packer.packString8(MsgPack::str_t(str8));
        packer.packString16(MsgPack::str_t(str16));
        // packer.packString32(str32); // TODO:
        // wrappers
        packer.pack(MsgPack::str_t(str0));
        packer.pack(MsgPack::str_t(str5));
        packer.pack(MsgPack::str_t(str8));
        packer.pack(MsgPack::str_t(str16));
        // packer.pack(str32); // TODO:

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        assert(unpacker.unpackString5() == MsgPack::str_t(str0));
        assert(unpacker.unpackString5() == MsgPack::str_t(str5));
        assert(unpacker.unpackString8() == MsgPack::str_t(str8));
        assert(unpacker.unpackString16() == MsgPack::str_t(str16));
        assert(unpacker.unpackString5() == MsgPack::str_t(str0));
        assert(unpacker.unpackString5() == MsgPack::str_t(str5));
        assert(unpacker.unpackString8() == MsgPack::str_t(str8));
        assert(unpacker.unpackString16() == MsgPack::str_t(str16));
        assert(unpacker.unpackString5() == MsgPack::str_t(str0));
        assert(unpacker.unpackString5() == MsgPack::str_t(str5));
        assert(unpacker.unpackString8() == MsgPack::str_t(str8));
        assert(unpacker.unpackString16() == MsgPack::str_t(str16));
        assert(unpacker.unpackString5() == MsgPack::str_t(str0));
        assert(unpacker.unpackString5() == MsgPack::str_t(str5));
        assert(unpacker.unpackString8() == MsgPack::str_t(str8));
        assert(unpacker.unpackString16() == MsgPack::str_t(str16));
        assert(unpacker.unpackString5() == MsgPack::str_t(str0));
        assert(unpacker.unpackString5() == MsgPack::str_t(str5));
        assert(unpacker.unpackString8() == MsgPack::str_t(str8));
        assert(unpacker.unpackString16() == MsgPack::str_t(str16));
        assert(unpacker.unpackString5() == MsgPack::str_t(str0));
        assert(unpacker.unpackString5() == MsgPack::str_t(str5));
        assert(unpacker.unpackString8() == MsgPack::str_t(str8));
        assert(unpacker.unpackString16() == MsgPack::str_t(str16));
    }
#else
    {
        MsgPack::Packer packer;

        // ---------- BIN format family ----------

        MsgPack::bin_t<uint8_t> bin8_v;
        // MsgPack::bin_t<uint8_t> bin16_v;
        // MsgPack::bin_t<uint8_t> bin32_v;

        bin8_v.resize(10);
        // bin16_v.resize(bin16_v.capacity());
        // bin32_v.resize(bin32_v.capacity());
        for (auto& b : bin8_v) b = 8;
        // for (auto& b : bin16_v) b = 16;
        // for (auto& b : bin32_v) b = 32;

        packer.packBinary8(bin8_v.data(), bin8_v.size());
        // packer.packBinary16(bin16_v.data(), bin16_v.size());
        // packer.packBinary32(bin32_v.data(), bin32_v.size());

        // // wrappers
        packer.pack(bin8_v.data(), bin8_v.size());
        // packer.pack(bin16_v.data(), bin16_v.size());
        // packer.pack(bin32_v.data(), bin32_v.size());

        packer.pack(bin8_v);
        // packer.pack(bin16_v);
        // packer.pack(bin32_v);

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        assert(unpacker.unpackBinary8() == bin8_v);
        // assert(unpacker.unpackBinary16() == bin16_v);
        // assert(unpacker.unpackBinary32() == bin32_v);
        assert(unpacker.unpackBinary8() == bin8_v);
        // assert(unpacker.unpackBinary16() == bin16_v);
        // assert(unpacker.unpackBinary32() == bin32_v);
        assert(unpacker.unpackBinary8() == bin8_v);
        // assert(unpacker.unpackBinary16() == bin16_v);
        // assert(unpacker.unpackBinary32() == bin32_v);
    }
#endif
    {
        MsgPack::Packer packer;

        // ---------- ARRAY format family ----------

        packer.packArraySize4(arr_size4);
        packer.packUInt7(intu7);
        packer.packInt5(int5);
        packer.packUInt8(intu8);
        packer.packInt8(int8);
        packer.packString5(str5);
        // TODO:
        // packer.packArraySize16();
        // packer.packArraySize32();

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        assert(unpacker.unpackArraySize() == arr_size4);
        assert(unpacker.unpackUInt7() == intu7);
        assert(unpacker.unpackInt5() == int5);
        assert(unpacker.unpackUInt8() == intu8);
        assert(unpacker.unpackInt8() == int8);
        assert(unpacker.unpackString5() == MsgPack::str_t(str5));
        // TODO:
        // assert(unpacker.unpackArraySize() == arr_size16);
        // assert(unpacker.unpackArraySize() == arr_size32);
    }
    {
        MsgPack::Packer packer;

        // ---------- MAP format family ----------

        packer.packMapSize4(map_size4);
        packer.packString5(str5);
        packer.packUInt7(intu7);
        packer.packString5(str5);
        packer.packInt5(int5);
        packer.packString5(str5);
        packer.packFloat32(f);
        // TODO:
        // packer.packMapSize16();
        // packer.packMapSize32();

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        assert(unpacker.unpackMapSize() == map_size4);
        assert(unpacker.unpackString5() == MsgPack::str_t(str5));
        assert(unpacker.unpackUInt7() == intu7);
        assert(unpacker.unpackString5() == MsgPack::str_t(str5));
        assert(unpacker.unpackInt5() == int5);
        assert(unpacker.unpackString5() == MsgPack::str_t(str5));
        assert(unpacker.unpackFloat32() == f);
        // assert(unpacker.unpackMapSize() == map_size16);
        // assert(unpacker.unpackMapSize() == map_size32);
    }
    // {
    //     MsgPack::Packer packer;

    //     // ---------- EXT format family ----------

    //     // TODO:
    //     // packer.packFixExt1(-1, 123);
    //     // packer.packFixExt2(-2, 12345);
    //     // packer.packFixExt4(-4, 1234567891);
    //     // packer.packFixExt8(-8, 123456789123456789);
    //     // packer.packFixExt16(-16, 123456789123456789, 123456789123456789);
    //     // packer.packExtSize8(-18, 1);
    //     //     packer.packBool(true);
    //     // packer.packExtSize16();
    //     // packer.packExtSize32();

    //     // ---------- TIMESTAMP format family ----------

    //     // TODO:
    //     // packer.packTimestamp32();
    //     // packer.packTimestamp64();
    //     // packer.packTimestamp96();

    //     MsgPack::Unpacker unpacker;
    //     unpacker.feed(packer.data(), packer.size());

    // }

    // timestamp
    {
    }

    Serial.println("test success");
}

void loop() {
}
