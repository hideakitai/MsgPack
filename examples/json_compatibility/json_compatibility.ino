// #define MSGPACK_DEBUGLOG_ENABLE
#include <MsgPack.h>

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("msgpack test start");

    // json is always packed into json array or json object (Map)
    // so we always should wrap values into array or map format

    // as json array : [123, 45.678, "9,9"]
    {
        // input to msgpack
        int i = 123;
        float f = 45.678;
        MsgPack::str_t s = "9.9";
        // output from msgpack
        int ii {0};
        float ff {0.f};
        MsgPack::str_t ss;

        // pack to msgpack
        MsgPack::Packer packer;
        packer.to_array(i, f, s);
        // or you can use "serialize"
        // packer.serialize(MsgPack::arr_size_t(3), i, f, s);

        // in json form, this becomes :
        // -> [123, 45.678, "9,9"]

        // unpack from msgpack
        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.from_array(ii, ff, ss);
        // or you can use "deserialize"
        // unpacker.deserialize(MsgPack::arr_size_t(3), ii, ff, ss);

        if (i != ii) Serial.println("failed: int");
        if (f != ff) Serial.println("failed: float");
        if (s != ss) Serial.println("failed: string");
    }

    // as json object : {"i":123, "f":45.678, "s":"9,9"}
    {
        // input to msgpack
        MsgPack::str_t ki = "i";
        int i = 123;
        MsgPack::str_t kf = "f";
        float f = 45.678;
        MsgPack::str_t ks = "s";
        MsgPack::str_t s = "9.9";
        // output from msgpack
        MsgPack::str_t kii;
        int ii {0};
        MsgPack::str_t kff;
        float ff {0.f};
        MsgPack::str_t kss;
        MsgPack::str_t ss;

        // pack to msgpack
        MsgPack::Packer packer;
        packer.to_map(ki, i, kf, f, ks, s);
        // or you can use "serialize"
        // packer.serialize(MsgPack::map_size_t(3), ki, i, kf, f, ks, s);

        // in json form, this becomes :
        // -> {"i":123, "f":45.678, "s":"9,9"}

        // unpack from msgpack
        MsgPack::Unpacker unpacker;
        unpacker.feed(packer.data(), packer.size());
        unpacker.from_map(kii, ii, kff, ff, kss, ss);
        // or you can use "deserialize"
        // unpacker.deserialize(MsgPack::map_size_t(3), kii, ii, kff, ff, kss, ss);

        if (ki != kii) Serial.println("failed: key of int");
        if (i != ii) Serial.println("failed: int");
        if (kf != kff) Serial.println("failed: key of float");
        if (f != ff) Serial.println("failed: float");
        if (ks != kss) Serial.println("failed: key of string");
        if (s != ss) Serial.println("failed: string");
    }

    Serial.println("msgpack test success");
}

void loop() {
}
