// #define MSGPACK_DEBUGLOG_ENABLE
#include <MsgPack.h>

struct CustomClassBase {
    int i;
    float f;
    MsgPack::str_t s;

    bool operator!=(const CustomClassBase& x) const {
        return (x.i != i) || (x.f != f) || (x.s != s);
    }

    MSGPACK_DEFINE(i, f, s);
};

struct CustomClassDerived : public CustomClassBase {
    int ii;
    float ff;
    MsgPack::str_t ss;

    bool operator!=(const CustomClassDerived& x) {
        return CustomClassBase::operator!=(x) || (x.ii != ii) || (x.ff != ff) || (x.ss != ss);
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

    bool operator!=(const CustomClassBaseMap& x) const {
        return (x.i != i) || (x.f != f) || (x.s != s) || (x.ki != ki) || (x.kf != kf) || (x.ks != ks);
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

    bool operator!=(const CustomClassDerivedMap& x) {
        return CustomClassBaseMap::operator!=(x) || (x.ii != ii) || (x.ff != ff) || (x.ss != ss) || (x.kii != kii) || (x.kff != kff) || (x.kss != kss);
    }

    MSGPACK_DEFINE_MAP(kii, ii, kff, ff, kss, ss, kb, MSGPACK_BASE(CustomClassBaseMap));
};

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("msgpack test start");

    // custom class
    {
        CustomClassBase c {1, 2.2, "3.3"};
        CustomClassBase cc;

        MsgPack::Packer packer;
        packer.serialize(c);

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.deserialize(cc);

        if (c != cc) Serial.println("failed: custom base class");
    }
    {
        CustomClassDerived d;  // {1, 2.2, "3.3"};
        CustomClassDerived dd;
        d.ii = 1;
        d.ff = 2.2;
        d.ss = "3.3";  // derived
        d.i = 4;
        d.f = 5.5;
        d.s = "6.6";  // base

        MsgPack::Packer packer;
        packer.serialize(d);

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.deserialize(dd);

        if (d != dd) Serial.println("failed: custom derived class");
    }
    {
        CustomClassBaseMap c {"i", 1, "f", 2.2, "s", "3.3"};
        CustomClassBaseMap cc;

        MsgPack::Packer packer;
        packer.serialize(c);

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.deserialize(cc);

        if (c != cc) Serial.println("failed: custom base class for map");
    }
    {
        CustomClassDerivedMap d;  // {1, 2.2, "3.3"};
        CustomClassDerivedMap dd;
        // derived
        d.kii = "ii";
        d.kff = "ff";
        d.kss = "ss";
        d.kb = "base";  // key for base
        d.ii = 1;
        d.ff = 2.2;
        d.ss = "3.3";
        // base
        d.ki = "i";
        d.kf = "f";
        d.ks = "s";
        d.i = 4;
        d.f = 5.5;
        d.s = "6.6";

        MsgPack::Packer packer;
        packer.serialize(d);

        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.deserialize(dd);

        if (d != dd) Serial.println("failed: custom derived class for map");
    }

    Serial.println("msgpack test success");
}

void loop() {
}
