// #define MSGPACK_DEBUGLOG_ENABLE
#include <MsgPack.h>

// serialize and deserialize nested structure
// {"i":i, "f":f, "a":["str", {"first":1, "second":"two"}]}

struct MyMap {
    MsgPack::str_t key_first;
    int i;
    MsgPack::str_t key_second;
    MsgPack::str_t s;
    MSGPACK_DEFINE_MAP(key_first, i, key_second, s);

    bool operator==(const MyMap& x) { return (key_first == x.key_first) && (i == x.i) && (key_second == x.key_second) && (s == x.s); }
    bool operator!=(const MyMap& x) { return !(*this == x); }
};

struct MyArr {
    MsgPack::str_t s;
    MyMap m;
    MSGPACK_DEFINE(s, m);

    bool operator==(const MyArr& x) { return (s == x.s) && (m == x.m); }
    bool operator!=(const MyArr& x) { return !(*this == x); }
};

struct MyNestedClass {
    MsgPack::str_t key_i;
    int i;
    MsgPack::str_t key_f;
    int f;
    MsgPack::str_t key_a;
    MyArr arr;
    MSGPACK_DEFINE_MAP(key_i, i, key_f, f, key_a, arr);

    bool operator==(const MyNestedClass& x) { return (key_i == x.key_i) && (i == x.i) && (key_f == x.key_f) && (f == x.f) && (key_a == x.key_a); }
    bool operator!=(const MyNestedClass& x) { return !(*this == x); }
};

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("msgpack test start");

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

    if (cc != c) Serial.println("failed: MyNestedClass");

    Serial.println("msgpack test success");
}

void loop() {
}
