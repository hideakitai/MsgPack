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
double d =  -12345.6789;

uint8_t arr_size4 = 5;

uint8_t map_size4 = 3;

int main ()
{
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

        packer.packIntU7(12);
        packer.packIntU8(234);
        packer.packIntU16(12345);
        packer.packIntU32(1234567891);
        packer.packIntU64(1234567891234567891);
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

        packer.packIntU7(intu7);
        packer.packIntU8(intu8);
        packer.packIntU16(intu16);
        packer.packIntU32(intu32);
        // TODO:
        packer.packIntU64(intu64);
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

        assert(unpacker.unpackIntU7() == intu7);
        assert(unpacker.unpackIntU8() == intu8);
        assert(unpacker.unpackIntU16() == intu16);
        assert(unpacker.unpackIntU32() == intu32);
        assert(unpacker.unpackIntU64() == intu64);
        assert(unpacker.unpackInt5() == int5);
        assert(unpacker.unpackInt8() == int8);
        assert(unpacker.unpackInt16() == int16);
        assert(unpacker.unpackInt32() == int32);
        assert(unpacker.unpackInt64() == int64);
        assert(unpacker.unpackIntU7() == intu7);
        assert(unpacker.unpackIntU8() == intu8);
        assert(unpacker.unpackIntU16() == intu16);
        assert(unpacker.unpackIntU32() == intu32);
        assert(unpacker.unpackIntU64() == intu64);
        assert(unpacker.unpackInt5() == int5);
        assert(unpacker.unpackInt8() == int8);
        assert(unpacker.unpackInt16() == int16);
        assert(unpacker.unpackInt32() == int32);
        assert(unpacker.unpackInt64() == int64);
        assert(unpacker.unpackFloat32() == f);
        assert(unpacker.unpackFloat64() == d);
        assert(unpacker.unpackFloat32() == f);
        assert(unpacker.unpackFloat64() == d);

        assert(unpacker.unpackIntU7() == intu7);
        assert(unpacker.unpackIntU8() == intu8);
        assert(unpacker.unpackIntU16() == intu16);
        assert(unpacker.unpackIntU32() == intu32);
        assert(unpacker.unpackIntU64() == intu64);
        assert(unpacker.unpackInt5() == int5);
        assert(unpacker.unpackInt8() == int8);
        assert(unpacker.unpackInt16() == int16);
        assert(unpacker.unpackInt32() == int32);
        assert(unpacker.unpackInt64() == int64);
        assert(unpacker.unpackIntU7() == intu7);
        assert(unpacker.unpackIntU8() == intu8);
        assert(unpacker.unpackIntU16() == intu16);
        assert(unpacker.unpackIntU32() == intu32);
        assert(unpacker.unpackIntU64() == intu64);
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

        packer.packString5(MsgPack::StringType(str0));
        packer.packString5(MsgPack::StringType(str5));
        packer.packString8(MsgPack::StringType(str8));
        packer.packString16(MsgPack::StringType(str16));
        // packer.packString32(str32); // TODO:
        // wrappers
        packer.pack(MsgPack::StringType(str0));
        packer.pack(MsgPack::StringType(str5));
        packer.pack(MsgPack::StringType(str8));
        packer.pack(MsgPack::StringType(str16));
        // packer.pack(str32); // TODO:


        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());

        assert(unpacker.unpackString5() == MsgPack::StringType(str0));
        assert(unpacker.unpackString5() == MsgPack::StringType(str5));
        assert(unpacker.unpackString8() == MsgPack::StringType(str8));
        assert(unpacker.unpackString16() == MsgPack::StringType(str16));
        assert(unpacker.unpackString5() == MsgPack::StringType(str0));
        assert(unpacker.unpackString5() == MsgPack::StringType(str5));
        assert(unpacker.unpackString8() == MsgPack::StringType(str8));
        assert(unpacker.unpackString16() == MsgPack::StringType(str16));
        assert(unpacker.unpackString5() == MsgPack::StringType(str0));
        assert(unpacker.unpackString5() == MsgPack::StringType(str5));
        assert(unpacker.unpackString8() == MsgPack::StringType(str8));
        assert(unpacker.unpackString16() == MsgPack::StringType(str16));
        assert(unpacker.unpackString5() == MsgPack::StringType(str0));
        assert(unpacker.unpackString5() == MsgPack::StringType(str5));
        assert(unpacker.unpackString8() == MsgPack::StringType(str8));
        assert(unpacker.unpackString16() == MsgPack::StringType(str16));
        assert(unpacker.unpackString5() == MsgPack::StringType(str0));
        assert(unpacker.unpackString5() == MsgPack::StringType(str5));
        assert(unpacker.unpackString8() == MsgPack::StringType(str8));
        assert(unpacker.unpackString16() == MsgPack::StringType(str16));
        assert(unpacker.unpackString5() == MsgPack::StringType(str0));
        assert(unpacker.unpackString5() == MsgPack::StringType(str5));
        assert(unpacker.unpackString8() == MsgPack::StringType(str8));
        assert(unpacker.unpackString16() == MsgPack::StringType(str16));

        MsgPack::StringType r_str0;
        unpacker.index(0);
        unpacker.unpack(r_str0);
        assert(r_str0 == MsgPack::StringType(str0));
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
            packer.packIntU7(intu7);
            packer.packInt5(int5);
            packer.packIntU8(intu8);
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
            assert(unpacker.unpackIntU7() == intu7);
            assert(unpacker.unpackInt5() == int5);
            assert(unpacker.unpackIntU8() == intu8);
            assert(unpacker.unpackInt8() == int8);
            assert(unpacker.unpackString5() == MsgPack::StringType(str5));
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
            packer.packIntU7(intu7);
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
            assert(unpacker.unpackString5() == MsgPack::StringType(str5));
            assert(unpacker.unpackIntU7() == intu7);
            assert(unpacker.unpackString5() == MsgPack::StringType(str5));
            assert(unpacker.unpackInt5() == int5);
            assert(unpacker.unpackString5() == MsgPack::StringType(str5));
            assert(unpacker.unpackFloat32() == f);
        // assert(unpacker.unpackMapSize() == map_size16);
        // assert(unpacker.unpackMapSize() == map_size32);
#else
#endif
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
        packer.encode(
            b1, b2,
            i5, u7, i8, i16, i32, i64, u8, u16, u32, u64,
            f, d,
            str0, str5, str8, str16, str,
            // bin8_p, bin16_p, bin32_p,
            bin8_vi, bin8_vu, bin8_ai, bin8_au,
            arr_v, arr_a, arr_d, arr_p,
            // arr_tp,
            arr_l, arr_fl, arr_s, arr_us, arr_ms, arr_ums,
            m, um, mm, umm
        );


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
        // std::tuple<int, float, std::string> r_arr_tp; // TODO:
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

        unpacker.decode(
            r_b1, r_b2,
            r_i5, r_u7, r_i8, r_i16, r_i32, r_i64, r_u8, r_u16, r_u32, r_u64,
            r_f, r_d,
            r_str0, r_str5, r_str8, r_str16, r_str,
            // r_bin8_p, r_bin16_p, r_bin32_p,
            r_bin8_vi, r_bin8_vu, r_bin8_ai, r_bin8_au,
            r_arr_v, r_arr_a, r_arr_d, r_arr_p,
            // r_arr_tp,
            r_arr_l, r_arr_fl, r_arr_s, r_arr_us, r_arr_ms, r_arr_ums,
            r_m, r_um, r_mm, r_umm
        );


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
        // assert(r_arr_tp == arr_tp);
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

    std::cout << "test success" << std::endl;
}
