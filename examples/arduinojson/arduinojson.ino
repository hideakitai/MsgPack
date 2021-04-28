// #define USE_DYNAMIC_JSON_DOCUMENT_INPUT   // remove comment to test DynamicJsonDocument
// #define USE_DYNAMIC_JSON_DOCUMENT_OUTPUT  // remove comment to test DynamicJsonDocument

#include <ArduinoJson.h>  // include before MsgPack.h

// #define MSGPACK_DEBUGLOG_ENABLE
#include <MsgPack.h>

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("msgpack test start");

#ifdef USE_DYNAMIC_JSON_DOCUMENT_INPUT
    DynamicJsonDocument doc_in(200);
#else
    StaticJsonDocument<200> doc_in;
#endif
    doc_in["sensor"] = "gps";
    doc_in["time"] = 1351824120;
    JsonArray data = doc_in.createNestedArray("data");
    data.add(48.756080);
    data.add(2.302038);

    MsgPack::Packer packer;
    packer.serialize(doc_in);

#ifdef USE_DYNAMIC_JSON_DOCUMENT_OUTPUT
    DynamicJsonDocument doc(200);
#else
    StaticJsonDocument<200> doc;
#endif
    MsgPack::Unpacker unpacker;
    unpacker.feed(packer.data(), packer.size());
    unpacker.deserialize(doc);

    const char* sensor = doc["sensor"];
    long time = doc["time"];
    double latitude = doc["data"][0];
    double longitude = doc["data"][1];
    Serial.println(sensor);
    Serial.println(time);
    Serial.println(latitude, 6);
    Serial.println(longitude, 6);

    Serial.println("msgpack test finish");
}

void loop() {
}
