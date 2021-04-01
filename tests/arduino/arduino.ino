#define MSGPACK_DEBUGLOG_ENABLE
#include <Arduino.h>
#include <MsgPack.h>

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

void setup() {
    Serial.begin(115200);

    delay(2000);

    Serial.println("msgpack test start");

    // practical use
    {
        // Bool
        bool b1 {true};
        bool b2 {false};
        // Integer
        char i5 {(char)-12};
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
        String str {str8};
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
        std::pair<String, int> arr_p {"1", 1};
        std::tuple<int, float, String> arr_tp {1, 2.2, "three"};
        std::list<int> arr_l {1, 2, 3, 4, 5};
        std::forward_list<int> arr_fl {1, 2, 3, 4, 5};
        std::set<int> arr_s {1, 2, 3, 4, 5};
        // std::unordered_set<int> arr_us {1, 2, 3, 4, 5};
        std::multiset<int> arr_ms {1, 2, 3, 4, 5};
        // std::unordered_multiset<int> arr_ums {1, 2, 3, 4, 5};
        // Map
        std::map<String, int> m {{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}};
        // std::unordered_map<String, int> um {{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}};
        std::multimap<String, int> mm {{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}};
        // std::unordered_multimap<String, int> umm {{"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}};

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
            arr_l, arr_fl, arr_s,
            // arr_us,
            arr_ms,
            // arr_ums,
            m,
            // um,
            mm
            // umm
        );

        // Bool
        bool r_b1 {false};
        bool r_b2 {false};
        // Integer
        char r_i5 {0};
        unsigned char r_u7 {0};
        int8_t r_i8 {0};
        int16_t r_i16 {0};
        int32_t r_i32 {0};
        int64_t r_i64 {0};
        uint8_t r_u8 {0};
        uint16_t r_u16 {0};
        uint32_t r_u32 {0};
        uint64_t r_u64 {0};
        // Float
        float r_f;
        double r_d;
        // Str
        String r_str0;
        String r_str5;
        String r_str8;
        String r_str16;
        String r_str;
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
        std::pair<String, int> r_arr_p;
        std::tuple<int, float, String> r_arr_tp;
        std::list<int> r_arr_l;
        std::forward_list<int> r_arr_fl;
        std::set<int> r_arr_s;
        // std::unordered_set<int> r_arr_us;
        std::multiset<int> r_arr_ms;
        // std::unordered_multiset<int> r_arr_ums;
        // Map
        std::map<String, int> r_m;
        // std::unordered_map<String, int> r_um;
        std::multimap<String, int> r_mm;
        // std::unordered_multimap<String, int> r_umm;

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
            r_arr_l, r_arr_fl, r_arr_s,
            // r_arr_us,
            r_arr_ms,
            // r_arr_ums,
            r_m,
            // r_um,
            r_mm
            // r_umm
        );

        // Serial.println("assert 1");
        // assert(1 == 1);
        // Serial.println("assert 2");
        // assert(1 == 2);
        // Serial.println("done");

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
        // assert(String((const char*)r_bin8_p.data()) == str5);
        // assert(String((const char*)r_bin16_p.data()) == str8);
        // assert(String((const char*)r_bin32_p.data()) == str16);
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
        // assert(r_arr_us == arr_us);
        assert(r_arr_ms == arr_ms);
        // assert(r_arr_ums == arr_ums);
        // Map
        assert(r_m == m);
        // assert(r_um == um);
        assert(r_mm == mm);
        // assert(r_umm == umm);
    }

    // string literals
    {
        const char* c1 = "test3";
        char c2[] = "abc";
        MsgPack::Packer packer;
        packer.serialize("test test test");
        packer.serialize(c1);
        packer.serialize(c2);
        packer.serialize(F("flash"));

        // ptr, size
        packer.pack(c1, strlen(c1));
        packer.pack(c2, strlen(c2));

        MsgPack::str_t ss1;
        MsgPack::str_t ss2;
        MsgPack::str_t ss3;
        MsgPack::str_t ss4;
        MsgPack::str_t ss5;
        MsgPack::str_t ss6;
        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.deserialize(ss1);
        unpacker.deserialize(ss2);
        unpacker.deserialize(ss3);
        unpacker.deserialize(ss4);
        unpacker.deserialize(ss5);
        unpacker.deserialize(ss6);

        MsgPack::str_t s1("test test test");
        MsgPack::str_t s2("test3");
        MsgPack::str_t s3("abc");
        MsgPack::str_t s4("flash");
        MsgPack::str_t s5("test3");
        MsgPack::str_t s6("abc");
        assert(s1 == ss1);
        assert(s2 == ss2);
        assert(s3 == ss3);
        assert(s4 == ss4);
        assert(s5 == ss5);
        assert(s6 == ss6);
    }

    Serial.println("test success");
}

void loop() {
}
