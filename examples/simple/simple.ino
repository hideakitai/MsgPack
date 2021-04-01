// #define MSGPACK_DEBUGLOG_ENABLE
#include <MsgPack.h>

// input to msgpack
int i = 123;
float f = 1.23;
String s = "str";
MsgPack::arr_t<int> v {1, 2, 3};
MsgPack::map_t<String, float> m {{"one", 1.1}, {"two", 2.2}, {"three", 3.3}};

// output from msgpack
int ri;
float rf;
String rs;
MsgPack::arr_t<int> rv;
MsgPack::map_t<String, float> rm;

// for STL enabled boards:
// MsgPack::arr_t -> std::vector
// MsgPack::map_t -> std::map
// MsgPack::bin_t -> std::vector<uint8_t> or <char>
// for NO-STL boards:
// MsgPack::arr_t -> arx::vector
// MsgPack::map_t -> arx::map
// MsgPack::bin_t -> arx::vector<uint8_t> or <char>

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("msgpack test start");

    MsgPack::Packer packer;
    packer.serialize(i, f, s, v, m);

    // you can also get serialized binary buffer
    // const MsgPack::bin_t<uint8_t>& packet = packer.packet();

    MsgPack::Unpacker unpacker;
    unpacker.feed(packer.data(), packer.size());
    unpacker.deserialize(ri, rf, rs, rv, rm);

    if (i != ri) Serial.println("failed: int");
    if (f != rf) Serial.println("failed: float");
    if (s != rs) Serial.println("failed: string");
    if (v != rv) Serial.println("failed: vector<int>");
    if (m != rm) Serial.println("failed: map<string, int>");

    Serial.println("msgpack test success");
}

void loop() {
}
