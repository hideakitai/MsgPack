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

    MsgPack::Packer packer;
    packer.encode(i, f, s, v, m);

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
