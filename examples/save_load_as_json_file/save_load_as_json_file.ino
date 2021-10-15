#define MSGPACK_DEBUGLOG_ENABLE

// if you want to use standard SD library
#include <SD.h>
#define fs SD

// If you want to use SdFat
// #include <SdFat.h>
// SdFat fs;
// SdFatSdio fs;

// If you want use SD_MMC (ESP) or other FileSystems
// #include <SD_MMC.h>
// #define fs SD_MMC

// If you want use SPIFFS (ESP32) or other FileSystems
// #include <SPIFFS.h>
// #define fs SPIFFS

// #include <LittleFS.h>
// TODO:

// #include <FatFs.h>
// TODO:

#include <ArduinoJson.h>
#include <MsgPack.h>

static constexpr size_t ARDUINOJSON_DOC_SIZE = 1024;

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

    // prepare file system
#ifdef TEENSYDUINO
    if (fs.begin(BUILTIN_SDCARD)) {
#else
    if (fs.begin()) {
#endif
        Serial.println("FileSystem initialization success");
    } else {
        ASSERTM(false, "FileSystem initialization failed!");
    }

    String filename = "/config.txt";  // txt: to avoid 8.3 rule of legacy SD class...
    MyConfig config;

    // load current config with StaticJsonDocument<ARDUINOJSON_DOC_SIZE>
    if (!MsgPack::file::load_from_json_static<ARDUINOJSON_DOC_SIZE>(fs, filename, config)) {
        Serial.println("[ERROR] No configuration found on file system");
    } else {
        Serial.println("Found configuration on file system:");
        print_config(config);
    }

    Serial.println();
    Serial.println("If you want to save new configuration, press any key...");

    while (!Serial.available()) {
        ;
    }

    // change config values
    randomSeed(analogRead(A0));  // change random seed every time
    config.meta.version = random(0, 100);
    config.meta.description = String("my setting on ") + __TIME__;
    config.data.i = random(100, 200);
    config.data.f = random(100, 200);
    config.data.s = String("random ") + random(100, 200);

    // save with DynamicJsonDocument
    if (!MsgPack::file::save_as_json_dynamic(fs, filename, config)) {
        Serial.println("[ERROR] Saving configuration to file system failed");
    }

    // show json file
    Serial.print("contents of ");
    Serial.println(filename);
    File f = fs.open(filename.c_str());
    while (f.available()) {
        Serial.write(f.read());
    }
    Serial.println("\n");

    // load with manually allocated JsonDocument
    StaticJsonDocument<ARDUINOJSON_DOC_SIZE> doc;  // for buffer
    if (!MsgPack::file::load_from_json(fs, filename, config, doc)) {
        Serial.println("[ERROR] Loading configuration from file system failed");
    } else {
        Serial.println("Saving configuration to file system success");
        print_config(config);
    }

    Serial.println("===== FINISH =====");
}

void loop() {}
