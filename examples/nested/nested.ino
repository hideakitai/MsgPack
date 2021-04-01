// #define MSGPACK_DEBUGLOG_ENABLE
#include <MsgPack.h>

// serialize and deserialize nested structure
// {"i":i, "f":f, "a":["str", {"first":1, "second":"two"}]}

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("msgpack test start");

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
    packer.serialize(
        MsgPack::map_size_t(3),
        ki, i,
        kf, f,
        ka, MsgPack::arr_size_t(2),
        s,
        MsgPack::map_size_t(2),
        kmf, vmf,
        kms, vms);

    MsgPack::map_size_t m3(3);
    MsgPack::str_t kii;
    int ii {0};
    MsgPack::str_t kff;
    float ff {0.f};
    MsgPack::str_t kaa;
    MsgPack::arr_size_t va2(2);
    MsgPack::str_t ss;
    MsgPack::map_size_t m2(2);
    MsgPack::str_t kmff;
    int vmff {0};
    MsgPack::str_t kmss;
    MsgPack::str_t vmss;

    MsgPack::Unpacker unpacker;
    unpacker.feed(packer.data(), packer.size());
    unpacker.deserialize(
        m3,
        kii, ii,
        kff, ff,
        kaa, va2,
        ss,
        m2,
        kmff, vmff,
        kmss, vmss);

    if (kii != ki) Serial.println("failed: ki");
    if (ii != i) Serial.println("failed: i");
    if (kff != kf) Serial.println("failed: kf");
    if (ff != f) Serial.println("failed: f");
    if (kaa != ka) Serial.println("failed: ka");
    if (ss != s) Serial.println("failed: s");
    if (kmff != kmf) Serial.println("failed: kmf");
    if (vmff != vmf) Serial.println("failed: vmf");
    if (kmss != kms) Serial.println("failed: kms");
    if (vmss != vms) Serial.println("failed: vms");

    Serial.println("msgpack test success");
}

void loop() {
}
