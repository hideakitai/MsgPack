#include <iostream>
#include <cassert>

#include <deque>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <array>
#include <forward_list>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#define MSGPACK_DEBUGLOG_ENABLE
#include "../../MsgPack.h"

const char str0[2] = "A";
const char str5[31] = "123456789012";
const char str8[255] = "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012";
const char str16[257] = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789001234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234";

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

float f = -12345.6789;
double d = -12345.6789;

uint8_t arr_size4 = 5;

uint8_t map_size4 = 3;

struct CustomClassBase {
    int i;
    float f;
    MsgPack::str_t s;

    bool operator==(const CustomClassBase& x) const {
        return (x.i == i) && (x.f == f) && (x.s == s);
    }

    MSGPACK_DEFINE(i, f, s);
};

struct CustomClassDerived : public CustomClassBase {
    int ii;
    float ff;
    MsgPack::str_t ss;

    bool operator==(const CustomClassDerived& x) {
        return CustomClassBase::operator==(x) && (x.ii == ii) && (x.ff == ff) && (x.ss == ss);
    }

    MSGPACK_DEFINE(ii, ff, ss, MSGPACK_BASE(CustomClassBase));
};

struct CustomClassBaseMap {
    MsgPack::str_t ki;
    int i;
    MsgPack::str_t kf;
    float f;
    MsgPack::str_t ks;
    MsgPack::str_t s;

    bool operator==(const CustomClassBaseMap& x) const {
        return (x.i == i) && (x.f == f) && (x.s == s) && (x.ki == ki) && (x.kf == kf) && (x.ks == ks);
    }

    MSGPACK_DEFINE_MAP(ki, i, kf, f, ks, s);
};

struct CustomClassDerivedMap : public CustomClassBaseMap {
    MsgPack::str_t kii;
    int ii;
    MsgPack::str_t kff;
    float ff;
    MsgPack::str_t kss;
    MsgPack::str_t ss;
    MsgPack::str_t kb;

    bool operator==(const CustomClassDerivedMap& x) {
        return CustomClassBaseMap::operator==(x) && (x.ii == ii) && (x.ff == ff) && (x.ss == ss) && (x.kii == kii) && (x.kff == kff) && (x.kss == kss);
    }

    MSGPACK_DEFINE_MAP(kii, ii, kff, ff, kss, ss, kb, MSGPACK_BASE(CustomClassBaseMap));
};

// serialize and deserialize nested structure
// {"i":i, "f":f, "a":["str", {"first":1, "second":"two"}]}

struct MyMap {
    MsgPack::str_t key_first;
    int i;
    MsgPack::str_t key_second;
    MsgPack::str_t s;

    bool operator==(const MyMap& x) { return (key_first == x.key_first) && (i == x.i) && (key_second == x.key_second) && (s == x.s); }
    bool operator!=(const MyMap& x) { return !(*this == x); }

    MSGPACK_DEFINE_MAP(key_first, i, key_second, s);
};

struct MyArr {
    MsgPack::str_t s;
    MyMap m;

    bool operator==(const MyArr& x) { return (s == x.s) && (m == x.m); }
    bool operator!=(const MyArr& x) { return !(*this == x); }

    MSGPACK_DEFINE(s, m);
};

struct MyNestedClass {
    MsgPack::str_t key_i;
    int i;
    MsgPack::str_t key_f;
    int f;
    MsgPack::str_t key_a;
    MyArr arr;

    bool operator==(const MyNestedClass& x) { return (key_i == x.key_i) && (i == x.i) && (key_f == x.key_f) && (f == x.f) && (key_a == x.key_a); }
    bool operator!=(const MyNestedClass& x) { return !(*this == x); }

    MSGPACK_DEFINE_MAP(key_i, i, key_f, f, key_a, arr);
};

int main() {
    std::cout << "msgpack test start" << std::endl;

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
        packer.packFloat64(-12345.6789);

        // wrappers
        packer.pack(-12345.6789f);
        packer.pack(-12345.6789);

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
        packer.packString8("01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012");
        packer.packString16("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789001234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234");
        // wrappers
        packer.pack("A");
        packer.pack("123456789012");
        packer.pack("01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012");
        packer.pack("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789001234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234");

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

        // ptr, size
        packer.pack(str0, strlen(str0));
        packer.pack(str5, strlen(str5));
        packer.pack(str8, strlen(str8));
        packer.pack(str16, strlen(str16));

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
        assert(unpacker.unpackString5() == MsgPack::str_t(str0));
        assert(unpacker.unpackString5() == MsgPack::str_t(str5));
        assert(unpacker.unpackString8() == MsgPack::str_t(str8));
        assert(unpacker.unpackString16() == MsgPack::str_t(str16));

        MsgPack::str_t r_str0;
        unpacker.index(0);
        unpacker.unpack(r_str0);
        assert(r_str0 == MsgPack::str_t(str0));
    }
    {
        MsgPack::Packer packer;

        // ---------- BIN format family ----------

        std::array<uint8_t, 255> bin8;
        std::array<uint8_t, 65535> bin16;
        std::array<uint8_t, 65536> bin32;
        std::vector<uint8_t> bin8_v;
        std::vector<uint8_t> bin16_v;
        std::vector<uint8_t> bin32_v;

        bin8.fill(8);
        bin16.fill(16);
        bin32.fill(32);
        bin8_v.resize(255);
        bin16_v.resize(65535);
        bin32_v.resize(65536);
        std::fill(bin8_v.begin(), bin8_v.end(), 8);
        std::fill(bin16_v.begin(), bin16_v.end(), 16);
        std::fill(bin32_v.begin(), bin32_v.end(), 32);

        packer.packBinary8(bin8.data(), bin8.size());
        packer.packBinary16(bin16.data(), bin16.size());
        packer.packBinary32(bin32.data(), bin32.size());
        packer.packBinary8(bin8_v.data(), bin8_v.size());
        packer.packBinary16(bin16_v.data(), bin16_v.size());
        packer.packBinary32(bin32_v.data(), bin32_v.size());

        // wrappers
        packer.pack(bin8.data(), bin8.size());
        packer.pack(bin16.data(), bin16.size());
        packer.pack(bin32.data(), bin32.size());
        packer.pack(bin8_v.data(), bin8_v.size());
        packer.pack(bin16_v.data(), bin16_v.size());
        packer.pack(bin32_v.data(), bin32_v.size());

        packer.pack(bin8);
        packer.pack(bin16);
        packer.pack(bin32);
        packer.pack(bin8_v);
        packer.pack(bin16_v);
        packer.pack(bin32_v);

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        assert((unpacker.unpackBinary<uint8_t, 255>() == bin8));
        assert((unpacker.unpackBinary16<uint8_t, 65535>() == bin16));
        assert((unpacker.unpackBinary32<uint8_t, 65536>() == bin32));
        assert(unpacker.unpackBinary8() == bin8_v);
        assert(unpacker.unpackBinary16() == bin16_v);
        assert(unpacker.unpackBinary32() == bin32_v);
        assert((unpacker.unpackBinary8<uint8_t, 255>() == bin8));
        assert((unpacker.unpackBinary16<uint8_t, 65535>() == bin16));
        assert((unpacker.unpackBinary32<uint8_t, 65536>() == bin32));
        assert(unpacker.unpackBinary8() == bin8_v);
        assert(unpacker.unpackBinary16() == bin16_v);
        assert(unpacker.unpackBinary32() == bin32_v);
        assert((unpacker.unpackBinary8<uint8_t, 255>() == bin8));
        assert((unpacker.unpackBinary16<uint8_t, 65535>() == bin16));
        assert((unpacker.unpackBinary32<uint8_t, 65536>() == bin32));
        assert(unpacker.unpackBinary8() == bin8_v);
        assert(unpacker.unpackBinary16() == bin16_v);
        assert(unpacker.unpackBinary32() == bin32_v);
    }
    {
        MsgPack::Packer packer;

        // ---------- BIN format family ----------

        std::array<uint8_t, 255> bin8;
        std::array<uint8_t, 65535> bin16;
        std::array<uint8_t, 65536> bin32;
        std::vector<uint8_t> bin8_v;
        std::vector<uint8_t> bin16_v;
        std::vector<uint8_t> bin32_v;

        bin8.fill(8);
        bin16.fill(16);
        bin32.fill(32);
        bin8_v.resize(255);
        bin16_v.resize(65535);
        bin32_v.resize(65536);
        std::fill(bin8_v.begin(), bin8_v.end(), 8);
        std::fill(bin16_v.begin(), bin16_v.end(), 16);
        std::fill(bin32_v.begin(), bin32_v.end(), 32);

        packer.packBinary8((uint8_t*)bin8.data(), bin8.size());
        packer.packBinary16((uint8_t*)bin16.data(), bin16.size());
        packer.packBinary32((uint8_t*)bin32.data(), bin32.size());
        packer.packBinary8((uint8_t*)bin8_v.data(), bin8_v.size());
        packer.packBinary16((uint8_t*)bin16_v.data(), bin16_v.size());
        packer.packBinary32((uint8_t*)bin32_v.data(), bin32_v.size());

        // wrappers
        // -> compile error! this should be string : pack(const char*)
        // packer.pack(bin8.data(), bin8.size());
        // packer.pack(bin16.data(), bin16.size());
        // packer.pack(bin32.data(), bin32.size());
        // packer.pack(bin8_v.data(), bin8_v.size());
        // packer.pack(bin16_v.data(), bin16_v.size());
        // packer.pack(bin32_v.data(), bin32_v.size());

        packer.pack(bin8);
        packer.pack(bin16);
        packer.pack(bin32);
        packer.pack(bin8_v);
        packer.pack(bin16_v);
        packer.pack(bin32_v);

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        // TODO: unpack to std::vector<char>
        std::vector<uint8_t> bin8_r;
        std::vector<uint8_t> bin16_r;
        std::vector<uint8_t> bin32_r;
        bin8_r.resize(255);
        bin16_r.resize(65535);
        bin32_r.resize(65536);
        std::fill(bin8_r.begin(), bin8_r.end(), 8);
        std::fill(bin16_r.begin(), bin16_r.end(), 16);
        std::fill(bin32_r.begin(), bin32_r.end(), 32);
        assert(unpacker.unpackBinary8() == bin8_r);
        assert(unpacker.unpackBinary16() == bin16_r);
        assert(unpacker.unpackBinary32() == bin32_r);
        assert(unpacker.unpackBinary8() == bin8_v);
        assert(unpacker.unpackBinary16() == bin16_v);
        assert(unpacker.unpackBinary32() == bin32_v);
        std::vector<uint8_t> bin8_v_r;
        unpacker.unpack(bin8_v_r);
        assert(bin8_v_r == bin8_r);
        std::vector<uint8_t> bin16_v_r;
        unpacker.unpack(bin16_v_r);
        assert(bin16_v_r == bin16_r);
        std::vector<uint8_t> bin32_v_r;
        unpacker.unpack(bin32_v_r);
        assert(bin32_v_r == bin32_r);
        unpacker.unpack(bin8_v_r);
        assert(bin8_v_r == bin8_r);
        unpacker.unpack(bin16_v_r);
        assert(bin16_v_r == bin16_r);
        unpacker.unpack(bin32_v_r);
        assert(bin32_v_r == bin32_r);
    }
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

        // wrappers
        uint16_t arr_uint16[5] {1, 2, 3, 4, 5};
        uint32_t arr_uint32[5] {1, 2, 3, 4, 5};
        uint64_t arr_uint64[5] {1, 2, 3, 4, 5};
        int16_t arr_int16[5] {1, 2, 3, 4, 5};
        int32_t arr_int32[5] {1, 2, 3, 4, 5};
        int64_t arr_int64[5] {1, 2, 3, 4, 5};
        float arr_f[5] {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
        double arr_d[5] {1.1, 2.2, 3.3, 4.4, 5.5};
        std::deque<int> dq;
        std::list<int> ls;
        std::map<std::string, int> m;
        std::pair<std::string, int> pr;
        std::set<int> s;
        std::vector<int> v;
        std::array<int, 5> arr;
        std::forward_list<int> fls;
        std::tuple<int, float, std::string> tp;
        std::unordered_map<std::string, int> um;
        std::unordered_set<int> us;
        std::multiset<int> ms;
        std::unordered_multiset<int> ums;
        std::multimap<std::string, int> mm;
        std::unordered_multimap<std::string, int> umm;

        packer.pack(arr_uint16, 5);
        packer.pack(arr_uint32, 5);
        packer.pack(arr_uint64, 5);
        packer.pack(arr_int16, 5);
        packer.pack(arr_int32, 5);
        packer.pack(arr_int64, 5);
        packer.pack(arr_f, 5);
        packer.pack(arr_d, 5);

        for (int i = 1; i <= 5; ++i) dq.push_back(i);
        packer.pack(dq);
        for (int i = 1; i <= 5; ++i) ls.push_back(i);
        packer.pack(ls);
        for (int i = 1; i <= 5; ++i) m.insert(std::make_pair(std::to_string(i), i));
        packer.pack(m);
        pr = std::make_pair(std::to_string(6), 6);
        packer.pack(pr);
        for (int i = 1; i <= 5; ++i) s.insert(i);
        packer.pack(s);
        for (int i = 1; i <= 5; ++i) v.push_back(i);
        packer.pack(v);
        for (int i = 0; i < 5; ++i) arr[i] = i + 1;
        packer.pack(arr);
        for (int i = 1; i <= 5; ++i) fls.push_front(i);
        packer.pack(fls);
        tp = std::make_tuple<int, float, std::string>(1, 2.2f, "3.3");
        packer.pack(tp);
        for (int i = 1; i <= 5; ++i) um.insert(std::make_pair(std::to_string(i), i));
        packer.pack(um);
        for (int i = 1; i <= 5; ++i) us.insert(i);
        packer.pack(us);
        for (int i = 1; i <= 5; ++i) ms.insert(i);
        packer.pack(ms);
        for (int i = 1; i <= 5; ++i) ums.insert(i);
        packer.pack(ums);
        for (int i = 1; i <= 5; ++i) mm.insert(std::make_pair(std::to_string(i), i));
        packer.pack(mm);
        for (int i = 1; i <= 5; ++i) umm.insert(std::make_pair(std::to_string(i), i));
        packer.pack(umm);

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

        std::vector<uint16_t> r_uint16;
        std::vector<uint16_t> i_uint16 {1, 2, 3, 4, 5};
        unpacker.unpack(r_uint16);
        assert(r_uint16 == i_uint16);
        std::vector<uint32_t> r_uint32;
        std::vector<uint32_t> i_uint32 {1, 2, 3, 4, 5};
        unpacker.unpack(r_uint32);
        assert(r_uint32 == i_uint32);
        std::vector<uint64_t> r_uint64;
        std::vector<uint64_t> i_uint64 {1, 2, 3, 4, 5};
        unpacker.unpack(r_uint64);
        assert(r_uint64 == i_uint64);
        std::vector<int16_t> r_int16;
        std::vector<int16_t> i_int16 {1, 2, 3, 4, 5};
        unpacker.unpack(r_int16);
        assert(r_int16 == i_int16);
        std::vector<int32_t> r_int32;
        std::vector<int32_t> i_int32 {1, 2, 3, 4, 5};
        unpacker.unpack(r_int32);
        assert(r_int32 == i_int32);
        std::vector<int64_t> r_int64;
        std::vector<int64_t> i_int64 {1, 2, 3, 4, 5};
        unpacker.unpack(r_int64);
        assert(r_int64 == i_int64);
        std::vector<float> r_f;
        std::vector<float> i_f {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
        unpacker.unpack(r_f);
        assert(r_f == i_f);
        std::vector<double> r_d;
        std::vector<double> i_d {1.1, 2.2, 3.3, 4.4, 5.5};
        unpacker.unpack(r_d);
        assert(r_d == i_d);

        std::deque<int> r_dq;
        unpacker.unpack(r_dq);
        assert(r_dq == dq);
        std::list<int> r_ls;
        unpacker.unpack(r_ls);
        assert(r_ls == ls);
        std::map<std::string, int> r_m;
        unpacker.unpack(r_m);
        assert(r_m == m);
        std::pair<std::string, int> r_pr;
        unpacker.unpack(r_pr);
        assert(r_pr == pr);
        std::set<int> r_s;
        unpacker.unpack(r_s);
        assert(r_s == s);
        std::vector<int> r_v;
        unpacker.unpack(r_v);
        assert(r_v == v);
        std::array<int, 5> r_arr;
        unpacker.unpack(r_arr);
        assert(r_arr == arr);
        std::forward_list<int> r_fls;
        unpacker.unpack(r_fls);
        assert(r_fls == fls);
        std::tuple<int, float, std::string> r_tp;
        unpacker.unpack(r_tp);
        assert(r_tp == tp);
        std::unordered_map<std::string, int> r_um;
        unpacker.unpack(r_um);
        assert(r_um == um);
        std::unordered_set<int> r_us;
        unpacker.unpack(r_us);
        assert(r_us == us);
        std::multiset<int> r_ms;
        unpacker.unpack(r_ms);
        assert(r_ms == ms);
        std::unordered_multiset<int> r_ums;
        unpacker.unpack(r_ums);
        assert(r_ums == ums);
        std::multimap<std::string, int> r_mm;
        unpacker.unpack(r_mm);
        assert(r_mm == mm);
        std::unordered_multimap<std::string, int> r_umm;
        unpacker.unpack(r_umm);
        assert(r_umm == umm);
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

#if 0
        assert(unpacker.unpackMapSize() == map_size4);
            assert(unpacker.unpackString5() == MsgPack::str_t(str5));
            assert(unpacker.unpackUInt7() == intu7);
            assert(unpacker.unpackString5() == MsgPack::str_t(str5));
            assert(unpacker.unpackInt5() == int5);
            assert(unpacker.unpackString5() == MsgPack::str_t(str5));
            assert(unpacker.unpackFloat32() == f);
        // assert(unpacker.unpackMapSize() == map_size16);
        // assert(unpacker.unpackMapSize() == map_size32);
#else
#endif
    }
    {
        MsgPack::Packer packer;

        // ---------- EXT format family ----------

        uint8_t e1 = 123;
        uint16_t e2 = 12345;
        uint32_t e4 = 1234567891;
        uint64_t e8 = 123456789123456789;
        union {
            struct {
                uint64_t l;
                uint64_t h;
            };
            uint8_t b[sizeof(uint64_t) * 2];
        } e16_ {e8, e8};
        MsgPack::object::ext e16(16, e16_.b, sizeof(e16_));
        union {
            struct {
                uint64_t ll;
                uint64_t lh;
                uint64_t hl;
                uint64_t hh;
            };
            uint8_t b[sizeof(uint64_t) * 4];
        } e32_ {e8, e8, e8, e8};
        MsgPack::object::ext e32(32, e32_.b, sizeof(e32_));
        uint8_t es8[123];
        for (auto& e : es8) e = 8;
        uint8_t es16[12345];
        for (auto& e : es16) e = 16;
        // uint8_t es32[1234567891];
        // for (auto& e : es32) e = 32;

        packer.packFixExt1(1, e1);
        packer.packFixExt2(2, e2);
        packer.packFixExt4(4, e4);
        packer.packFixExt8(8, e8);
        packer.packFixExt16(16, e8, e8);

        // packer.packFixExt1(1, &e1, sizeof(e1));
        packer.packFixExt2(2, &e2);
        packer.packFixExt4(4, &e4);
        packer.packFixExt8(8, &e8);
        packer.packFixExt16(16, e16.data());

        packer.packFixExt(1, e1);
        packer.packFixExt(2, e2);
        packer.packFixExt(4, e4);
        packer.packFixExt(8, e8);
        packer.packFixExt(16, e8, e8);

        packer.packFixExt(1, &e1, sizeof(e1));
        packer.packFixExt(2, &e2, sizeof(e2));
        packer.packFixExt(4, &e4, sizeof(e4));
        packer.packFixExt(8, &e8, sizeof(e8));
        packer.packFixExt(16, e16.data(), e16.size());

        packer.packExt(1, &e1, sizeof(e1));
        packer.packExt(2, &e2, sizeof(e2));
        packer.packExt(4, &e4, sizeof(e4));
        packer.packExt(8, &e8, sizeof(e8));
        packer.packExt(16, e16.data(), e16.size());
        packer.packExt(18, es8, sizeof(es8));
        packer.packExt(116, es16, sizeof(es16));
        // packer.packExt(32, es32, sizeof(es32));

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        MsgPack::object::ext r_e1, r_e2, r_e4, r_e8, r_e16, r_e32;
        MsgPack::object::ext r_es8, r_es16, r_es32;

        MsgPack::object::ext p_e1(1, (const uint8_t*)&e1, sizeof(e1));
        MsgPack::object::ext p_e2(2, (const uint8_t*)&e2, sizeof(e2));
        MsgPack::object::ext p_e4(4, (const uint8_t*)&e4, sizeof(e4));
        MsgPack::object::ext p_e8(8, (const uint8_t*)&e8, sizeof(e8));
        // MsgPack::object::ext p_e16;
        // MsgPack::object::ext p_e32;
        MsgPack::object::ext p_es8 {18, es8, sizeof(es8)};
        MsgPack::object::ext p_es16 {116, es16, sizeof(es16)};
        // MsgPack::object::ext p_es32 {32, es32, sizeof(es32)};

        unpacker.unpack(r_e1);
        unpacker.unpack(r_e2);
        unpacker.unpack(r_e4);
        unpacker.unpack(r_e8);
        unpacker.unpack(r_e16);
        assert(r_e1 == p_e1);
        assert(r_e2 == p_e2);
        assert(r_e4 == p_e4);
        assert(r_e8 == p_e8);
        assert(r_e16 == e16);

        // unpacker.unpack(r_e1);
        unpacker.unpack(r_e2);
        unpacker.unpack(r_e4);
        unpacker.unpack(r_e8);
        unpacker.unpack(r_e16);
        assert(r_e2 == p_e2);
        assert(r_e4 == p_e4);
        assert(r_e8 == p_e8);
        assert(r_e16 == e16);

        unpacker.unpack(r_e1);
        unpacker.unpack(r_e2);
        unpacker.unpack(r_e4);
        unpacker.unpack(r_e8);
        unpacker.unpack(r_e16);
        assert(r_e1 == p_e1);
        assert(r_e2 == p_e2);
        assert(r_e4 == p_e4);
        assert(r_e8 == p_e8);
        assert(r_e16 == e16);

        unpacker.unpack(r_e1);
        unpacker.unpack(r_e2);
        unpacker.unpack(r_e4);
        unpacker.unpack(r_e8);
        unpacker.unpack(r_e16);
        assert(r_e1 == p_e1);
        assert(r_e2 == p_e2);
        assert(r_e4 == p_e4);
        assert(r_e8 == p_e8);
        assert(r_e16 == e16);

        unpacker.unpack(r_e1);
        unpacker.unpack(r_e2);
        unpacker.unpack(r_e4);
        unpacker.unpack(r_e8);
        unpacker.unpack(r_e16);
        unpacker.unpack(r_es8);
        unpacker.unpack(r_es16);
        // unpacker.unpack(r_es32);
        assert(r_e1 == p_e1);
        assert(r_e2 == p_e2);
        assert(r_e4 == p_e4);
        assert(r_e8 == p_e8);
        assert(r_e16 == e16);
        assert(r_es8 == p_es8);
        assert(r_es16 == p_es16);
        // assert(r_es32 == p_es32);
    }

    // timestamp
    {
        // ---------- TIMESTAMP format family ----------

        MsgPack::object::timespec tv32 {12345, 0};
        MsgPack::object::timespec tv64 {17179869180, 1073741820};
        uint64_t utime64 = ((tv64.tv_nsec & 0x00000003FFFFFFFF) << 34) | (uint64_t)(tv64.tv_sec & 0x3FFFFFFFF);
        MsgPack::object::timespec tv96 {18000000000, 1100000000};

        MsgPack::Packer packer;

        packer.packTimestamp32(tv32.tv_sec);
        packer.packTimestamp64(utime64);
        packer.packTimestamp64(tv64.tv_sec, tv64.tv_nsec);
        packer.packTimestamp96(tv96.tv_sec, tv96.tv_nsec);

        packer.packTimestamp(tv32);
        packer.packTimestamp(tv64);
        packer.packTimestamp(tv96);

        MsgPack::object::timespec r_tv32, r_tv64, r_tv96;

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        unpacker.unpack(r_tv32);
        assert(r_tv32 == tv32);
        unpacker.unpack(r_tv64);
        assert(r_tv64 == tv64);
        unpacker.unpack(r_tv64);
        assert(r_tv64 == tv64);
        unpacker.unpack(r_tv96);
        assert(r_tv96 == tv96);

        unpacker.unpack(r_tv32);
        assert(r_tv32 == tv32);
        unpacker.unpack(r_tv64);
        assert(r_tv64 == tv64);
        unpacker.unpack(r_tv96);
        assert(r_tv96 == tv96);
    }

    // practical use
    {
        // Bool
        bool b1 {true};
        bool b2 {false};
        // Integer
        char i5 {-12};
        unsigned char u7 {12};
        int8_t i8 {-123};
        int16_t i16 {-12345};
        int32_t i32 {-1234567891};
        int64_t i64 {-1234567891234567891};
        uint8_t u8 {234};
        uint16_t u16 {12345};
        uint32_t u32 {1234567891};
        uint64_t u64 {1234567891234567891};
        // Float
        float f {-12345.6789};
        double d {-12345.6789};
        // Str
        // const char[] str0;
        // const char[] str5
        // const char[] str8
        // const char[] str16
        std::string str {str8};
        // Bin
        // unsigned char* bin8_p = (unsigned char*)str5;
        // unsigned char* bin16_p = (unsigned char*)str8;
        // unsigned char* bin32_p = (unsigned char*)str16;
        std::vector<char> bin8_vi {1, 2, 3, 4, 5};
        std::vector<unsigned char> bin8_vu {1, 2, 3, 4, 5};
        std::array<char, 5> bin8_ai {1, 2, 3, 4, 5};
        std::array<unsigned char, 5> bin8_au {1, 2, 3, 4, 5};
        // Array
        std::vector<int> arr_v {1, 2, 3, 4, 5};
        std::array<int, 5> arr_a {1, 2, 3, 4, 5};
        std::deque<int> arr_d {1, 2, 3, 4, 5};
        std::pair<std::string, int> arr_p {"1", 1};
        std::tuple<int, float, std::string> arr_tp {1, 2.2, "three"};
        std::list<int> arr_l {1, 2, 3, 4, 5};
        std::forward_list<int> arr_fl {1, 2, 3, 4, 5};
        std::set<int> arr_s {1, 2, 3, 4, 5};
        std::unordered_set<int> arr_us {1, 2, 3, 4, 5};
        std::multiset<int> arr_ms {1, 2, 3, 4, 5};
        std::unordered_multiset<int> arr_ums {1, 2, 3, 4, 5};
        // Map
        std::map<std::string, int> m {{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}};
        std::unordered_map<std::string, int> um {{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}};
        std::multimap<std::string, int> mm {{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}};
        std::unordered_multimap<std::string, int> umm {{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}};

        MsgPack::Packer packer;
        packer.serialize(
            b1, b2,
            i5, u7, i8, i16, i32, i64, u8, u16, u32, u64,
            f, d,
            str0, str5, str8, str16, str,
            // bin8_p, bin16_p, bin32_p,
            bin8_vi, bin8_vu, bin8_ai, bin8_au,
            arr_v, arr_a, arr_d, arr_p,
            arr_tp,
            arr_l, arr_fl, arr_s, arr_us, arr_ms, arr_ums,
            m, um, mm, umm);

        // Bool
        bool r_b1;
        bool r_b2;
        // Integer
        char r_i5;
        unsigned char r_u7;
        int8_t r_i8;
        int16_t r_i16;
        int32_t r_i32;
        int64_t r_i64;
        uint8_t r_u8;
        uint16_t r_u16;
        uint32_t r_u32;
        uint64_t r_u64;
        // Float
        float r_f;
        double r_d;
        // Str
        std::string r_str0;
        std::string r_str5;
        std::string r_str8;
        std::string r_str16;
        std::string r_str;
        // Bin
        // std::vector<uint8_t> r_bin8_p;
        // std::vector<uint8_t> r_bin16_p;
        // std::vector<uint8_t> r_bin32_p;
        std::vector<char> r_bin8_vi;
        std::vector<unsigned char> r_bin8_vu;
        std::array<char, 5> r_bin8_ai;
        std::array<unsigned char, 5> r_bin8_au;
        // Array
        std::vector<int> r_arr_v;
        std::array<int, 5> r_arr_a;
        std::deque<int> r_arr_d;
        std::pair<std::string, int> r_arr_p;
        std::tuple<int, float, std::string> r_arr_tp;
        std::list<int> r_arr_l;
        std::forward_list<int> r_arr_fl;
        std::set<int> r_arr_s;
        std::unordered_set<int> r_arr_us;
        std::multiset<int> r_arr_ms;
        std::unordered_multiset<int> r_arr_ums;
        // Map
        std::map<std::string, int> r_m;
        std::unordered_map<std::string, int> r_um;
        std::multimap<std::string, int> r_mm;
        std::unordered_multimap<std::string, int> r_umm;

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        unpacker.deserialize(
            r_b1, r_b2,
            r_i5, r_u7, r_i8, r_i16, r_i32, r_i64, r_u8, r_u16, r_u32, r_u64,
            r_f, r_d,
            r_str0, r_str5, r_str8, r_str16, r_str,
            // r_bin8_p, r_bin16_p, r_bin32_p,
            r_bin8_vi, r_bin8_vu, r_bin8_ai, r_bin8_au,
            r_arr_v, r_arr_a, r_arr_d, r_arr_p,
            r_arr_tp,
            r_arr_l, r_arr_fl, r_arr_s, r_arr_us, r_arr_ms, r_arr_ums,
            r_m, r_um, r_mm, r_umm);

        // Bool
        assert(r_b1 == b1);
        assert(r_b2 == b2);
        // Integer
        assert(r_i5 == i5);
        assert(r_u7 == u7);
        assert(r_i8 == i8);
        assert(r_i16 == i16);
        assert(r_i32 == i32);
        assert(r_i64 == i64);
        assert(r_u8 == u8);
        assert(r_u16 == u16);
        assert(r_u32 == u32);
        assert(r_u64 == u64);
        // Float
        assert(r_f == f);
        assert(r_d == d);
        // Str
        assert(r_str0 == str0);
        assert(r_str5 == str5);
        assert(r_str8 == str8);
        assert(r_str16 == str16);
        assert(r_str == str);
        // Bin
        // assert(std::string((const char*)r_bin8_p.data()) == str5);
        // assert(std::string((const char*)r_bin16_p.data()) == str8);
        // assert(std::string((const char*)r_bin32_p.data()) == str16);
        assert(r_bin8_vi == bin8_vi);
        assert(r_bin8_vu == bin8_vu);
        assert(r_bin8_ai == bin8_ai);
        assert(r_bin8_au == bin8_au);
        // Array
        assert(r_arr_v == arr_v);
        assert(r_arr_a == arr_a);
        assert(r_arr_d == arr_d);
        assert(r_arr_p == arr_p);
        assert(r_arr_tp == arr_tp);
        assert(r_arr_l == arr_l);
        assert(r_arr_fl == arr_fl);
        assert(r_arr_s == arr_s);
        assert(r_arr_us == arr_us);
        assert(r_arr_ms == arr_ms);
        assert(r_arr_ums == arr_ums);
        // Map
        assert(r_m == m);
        assert(r_um == um);
        assert(r_mm == mm);
        assert(r_umm == umm);
    }

    // serialize as array
    {
        int i = 123;
        float f = 45.678;
        MsgPack::str_t s = "9.9";

        MsgPack::Packer packer;
        packer.serialize(MsgPack::arr_size_t(3), i, f, s);

        int ii;
        float ff;
        MsgPack::str_t ss;

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.from_array(ii, ff, ss);

        assert(i == ii);
        assert(f == ff);
        assert(s == ss);
    }
    {
        int i = 123;
        float f = 45.678;
        MsgPack::str_t s = "9.9";

        MsgPack::Packer packer;
        packer.to_array(i, f, s);

        int ii;
        float ff;
        MsgPack::str_t ss;

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.from_array(ii, ff, ss);

        assert(i == ii);
        assert(f == ff);
        assert(s == ss);
    }

    // serialize as map
    {
        MsgPack::str_t ki = "i";
        int i = 123;
        MsgPack::str_t kf = "f";
        float f = 45.678;
        MsgPack::str_t ks = "s";
        MsgPack::str_t s = "9.9";

        MsgPack::Packer packer;
        packer.serialize(MsgPack::map_size_t(3), ki, i, kf, f, ks, s);

        MsgPack::str_t kii;
        int ii;
        MsgPack::str_t kff;
        float ff;
        MsgPack::str_t kss;
        MsgPack::str_t ss;

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.from_map(kii, ii, kff, ff, kss, ss);

        assert(ki == kii);
        assert(i == ii);
        assert(kf == kff);
        assert(f == ff);
        assert(ks == kss);
        assert(s == ss);
    }
    {
        MsgPack::str_t ki = "i";
        int i = 123;
        MsgPack::str_t kf = "f";
        float f = 45.678;
        MsgPack::str_t ks = "s";
        MsgPack::str_t s = "9.9";

        MsgPack::Packer packer;
        packer.to_map(ki, i, kf, f, ks, s);

        MsgPack::str_t kii;
        int ii;
        MsgPack::str_t kff;
        float ff;
        MsgPack::str_t kss;
        MsgPack::str_t ss;

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.from_map(kii, ii, kff, ff, kss, ss);

        assert(ki == kii);
        assert(i == ii);
        assert(kf == kff);
        assert(f == ff);
        assert(ks == kss);
        assert(s == ss);
    }

    // custom class
    {
        CustomClassBase a {1, 2.2, "3.3"};
        CustomClassBase b {4, 5.5, "6.6"};
        CustomClassBase c {7, 8.8, "9.9"};
        CustomClassBase r_a, r_b, r_c;

        CustomClassDerived d;  // {1, 2.2, "3.3"};
        CustomClassDerived e;  // {4, 5.5, "6.6"};
        CustomClassDerived f;  // {7, 8.8, "9.9"};
        CustomClassDerived r_d, r_e, r_f;
        d.ii = 1;
        d.ff = 2.2;
        d.ss = "3.3";
        d.i = 4;
        d.f = 5.5;
        d.s = "6.6";
        e.ii = 4;
        e.ff = 5.5;
        e.ss = "6.6";
        e.i = 7;
        e.f = 8.8;
        e.s = "9.9";
        f.ii = 7;
        f.ff = 8.8;
        f.ss = "9.9";
        f.i = 1;
        f.f = 2.2;
        f.s = "3.3";

        MsgPack::Packer packer;
        packer.pack(a);
        packer.pack(b);
        packer.pack(c);
        packer.pack(d);
        packer.pack(e);
        packer.pack(f);

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        unpacker.deserialize(r_a, r_b, r_c);

        assert(r_a == a);
        assert(r_b == b);
        assert(r_c == c);

        unpacker.deserialize(r_d, r_e, r_f);

        assert(r_d == d);
        assert(r_e == e);
        assert(r_f == f);
    }
    {
        CustomClassBaseMap a {"i", 1, "f", 2.2, "s", "3.3"};
        CustomClassBaseMap b {"i", 4, "f", 5.5, "s", "6.6"};
        CustomClassBaseMap c {"i", 7, "f", 8.8, "s", "9.9"};
        CustomClassBaseMap r_a, r_b, r_c;

        CustomClassDerivedMap d;  // {1, 2.2, "3.3"};
        CustomClassDerivedMap e;  // {4, 5.5, "6.6"};
        CustomClassDerivedMap f;  // {7, 8.8, "9.9"};
        CustomClassDerivedMap r_d, r_e, r_f;
        d.kii = "ii";
        d.kff = "ff";
        d.kss = "ss";
        d.kb = "base";
        d.ii = 1;
        d.ff = 2.2;
        d.ss = "3.3";
        d.ki = "i";
        d.kf = "f";
        d.ks = "s";
        d.i = 4;
        d.f = 5.5;
        d.s = "6.6";
        e.kii = "ii";
        e.kff = "ff";
        e.kss = "ss";
        e.kb = "base";
        e.ii = 4;
        e.ff = 5.5;
        e.ss = "6.6";
        e.ki = "i";
        e.kf = "f";
        e.ks = "s";
        e.i = 7;
        e.f = 8.8;
        e.s = "9.9";
        f.kii = "ii";
        f.kff = "ff";
        f.kss = "ss";
        f.kb = "base";
        f.ii = 7;
        f.ff = 8.8;
        f.ss = "9.9";
        f.ki = "i";
        f.kf = "f";
        f.ks = "s";
        f.i = 1;
        f.f = 2.2;
        f.s = "3.3";

        MsgPack::Packer packer;
        packer.pack(a);
        packer.pack(b);
        packer.pack(c);
        packer.pack(d);
        packer.pack(e);
        packer.pack(f);

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        unpacker.deserialize(r_a, r_b, r_c);

        assert(r_a == a);
        assert(r_b == b);
        assert(r_c == c);

        unpacker.deserialize(r_d, r_e, r_f);

        assert(r_d == d);
        assert(r_e == e);
        assert(r_f == f);
    }

    // nested
    {
        MsgPack::str_t ki {"i"};
        int i {1};
        MsgPack::str_t kf {"f"};
        float f {2.2};
        MsgPack::str_t ka {"a"};
        MsgPack::str_t s {"str"};
        MsgPack::str_t kmf {"first"};
        int vmf {1};
        MsgPack::str_t kms {"second"};
        MsgPack::str_t vms {"two"};

        MsgPack::Packer packer;
        packer.serialize(MsgPack::map_size_t(3),
                         ki, i,
                         kf, f,
                         ka, MsgPack::arr_size_t(2),
                         s,
                         MsgPack::map_size_t(2),
                         kmf, vmf,
                         kms, vms);

        MsgPack::str_t kii;
        int ii;
        MsgPack::str_t kff;
        float ff;
        MsgPack::str_t kaa;
        MsgPack::str_t ss;
        MsgPack::str_t kmff;
        int vmff;
        MsgPack::str_t kmss;
        MsgPack::str_t vmss;

        MsgPack::Unpacker unpacker;
        MsgPack::map_size_t msz1;
        MsgPack::arr_size_t asz1;
        MsgPack::map_size_t msz2;
        unpacker.feed(packer.data(), packer.size());
        unpacker.deserialize(msz1,
                             kii, ii,
                             kff, ff,
                             kaa, asz1,
                             ss,
                             msz2,
                             kmff, vmff,
                             kmss, vmss);

        assert(msz1.size() == 3);
        assert(kii == ki);
        assert(ii == i);
        assert(kff == kf);
        assert(ff == f);
        assert(kaa == ka);
        assert(asz1.size() == 2);
        assert(ss == s);
        assert(msz2.size() == 2);
        assert(kmff == kmf);
        assert(vmff == vmf);
        assert(kmss == kms);
        assert(vmss == vms);
    }

    // nested class
    {
        MyNestedClass c;
        c.key_i = "i";
        c.i = 1;
        c.key_f = "f";
        c.f = 2.2f;
        c.key_a = "a";
        c.arr.s = "str";
        c.arr.m.key_first = "first";
        c.arr.m.i = 1;
        c.arr.m.key_second = "second";
        c.arr.m.s = "two";

        MsgPack::Packer packer;
        packer.serialize(c);

        MyNestedClass cc;
        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.deserialize(cc);

        assert(c == cc);
    }

    // string literal
    {
        const char* c1 = "test3";
        char c2[] = "abc";
        MsgPack::Packer packer;
        packer.serialize("test test test");
        packer.serialize(c1);
        packer.serialize(c2);

        MsgPack::str_t ss1;
        MsgPack::str_t ss2;
        MsgPack::str_t ss3;
        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.deserialize(ss1);
        unpacker.deserialize(ss2);
        unpacker.deserialize(ss3);

        MsgPack::str_t s1("test test test");
        MsgPack::str_t s2("test3");
        MsgPack::str_t s3("abc");
        assert(s1 == ss1);
        assert(s2 == ss2);
        assert(s3 == ss3);
    }

    std::cout << "test success" << std::endl;
}
