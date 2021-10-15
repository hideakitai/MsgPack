#define MSGPACK_DEBUGLOG_ENABLE
#include <EEPROM.h>  // you should include EEPROM.h first
#include <MsgPack.h>

struct Meta {
    int version;
    String description;

    MSGPACK_DEFINE(version, description);
};

struct Data {
    int i;
    float f;
    String s;

    MSGPACK_DEFINE(i, f, s);
};

struct MyConfig {
    Meta meta;
    Data data;

    MSGPACK_DEFINE(meta, data);
};

void print_config(const MyConfig& config) {
    Serial.print("version     : ");
    Serial.println(config.meta.version);
    Serial.print("description : ");
    Serial.println(config.meta.description);
    Serial.print("data (i)    : ");
    Serial.println(config.data.i);
    Serial.print("data (f)    : ");
    Serial.println(config.data.f);
    Serial.print("data (s)    : ");
    Serial.println(config.data.s);
}

void setup() {
    Serial.begin(115200);
    delay(2000);

#if defined(ESP_PLATFORM) || defined(ESP8266)
    EEPROM.begin(0xFF);  // should set the size of EEPROM
#endif

    MyConfig config;

    // load current config
    if (!MsgPack::eeprom::load(config)) {
        Serial.println("No configuration found on EEPROM");
    } else {
        Serial.println("Found configuration on EEPROM:");
        print_config(config);
    }

    Serial.println();
    Serial.println("If you want to save new configuration, press any key...");

    while (!Serial.available()) {
        ;
    }

    // clear
    MsgPack::eeprom::clear(config);
    // or clear manually with size
    // const size_t size = MsgPack::estimate_size(config);
    // MsgPack::eeprom::clear_size(size);

    // change config values
    randomSeed(analogRead(A0));  // change random seed every time
    config.meta.version = random(0, 100);
    config.meta.description = String("my setting on ") + __TIME__;
    config.data.i = random(100, 200);
    config.data.f = random(100, 200);
    config.data.s = String("random ") + random(100, 200);

    // save
    if (!MsgPack::eeprom::save(config)) {
        Serial.println("[ERROR] Saving configuration to EEPROM failed");
    }

    // load
    if (!MsgPack::eeprom::load(config)) {
        Serial.println("[ERROR] Loading configuration from EEPROM failed");
    } else {
        Serial.println("Saving configuration to EEPROM success");
        print_config(config);
    }

    EEPROM.end();
    Serial.println("===== FINISH =====");
}

void loop() {}
