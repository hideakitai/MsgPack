#include <MsgPack.h>

#include <array>
#include <cstdint>

static bool decode(const uint8_t* wire, size_t size, bool expected) {
    MsgPack::Unpacker unpacker;
    if (!unpacker.feed(wire, size)) return false;

    std::array<uint8_t, 4> value{};
    if (unpacker.deserialize(value) != expected) return false;
    return !expected || (value[0] == 1 && value[3] == 4);
}

int main() {
    const uint8_t bin8_ok[] = {0xc4, 4, 1, 2, 3, 4};
    const uint8_t bin8_large[] = {0xc4, 5, 1, 2, 3, 4, 5};
    const uint8_t bin16_ok[] = {0xc5, 0, 4, 1, 2, 3, 4};
    const uint8_t bin16_large[] = {0xc5, 0, 5, 1, 2, 3, 4, 5};
    const uint8_t bin32_ok[] = {0xc6, 0, 0, 0, 4, 1, 2, 3, 4};
    const uint8_t bin32_large[] = {0xc6, 0, 0, 0, 5, 1, 2, 3, 4, 5};

    MsgPack::Unpacker unpacker;
    const uint8_t nil[] = {0xc0};
    const uint8_t truncated_bin32[] = {0xc6};
    const uint8_t oversized_bin32[] = {0xc6, 0xff, 0xff, 0xff, 0xff};
    const uint8_t truncated_float[] = {0xcb, 0};
    if (unpacker.feed(nullptr, 0) || unpacker.feed(nil, 0)) return 2;
    if (unpacker.feed(truncated_bin32, sizeof(truncated_bin32))) return 3;
    if (unpacker.feed(oversized_bin32, sizeof(oversized_bin32))) return 4;
    if (unpacker.feed(truncated_float, sizeof(truncated_float))) return 5;
    // Reusing the decoder must reset the previous feed's index table/cursor.
    if (!unpacker.feed(bin8_ok, sizeof(bin8_ok))) return 6;
    std::array<uint8_t, 4> value{};
    if (!unpacker.deserialize(value) || value[3] != 4) return 7;
    if (!unpacker.feed(bin16_ok, sizeof(bin16_ok))) return 8;
    if (!unpacker.deserialize(value) || value[3] != 4) return 9;
    return decode(bin8_ok, sizeof(bin8_ok), true)
        && decode(bin8_large, sizeof(bin8_large), false)
        && decode(bin16_ok, sizeof(bin16_ok), true)
        && decode(bin16_large, sizeof(bin16_large), false)
        && decode(bin32_ok, sizeof(bin32_ok), true)
        && decode(bin32_large, sizeof(bin32_large), false)
        ? 0 : 1;
}
