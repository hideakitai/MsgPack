#pragma once
#ifndef HT_SERIAL_MSGPACK_H
#define HT_SERIAL_MSGPACK_H

#if defined(ARDUINO_ARCH_AVR)\
 || defined(ARDUINO_ARCH_MEGAAVR)\
 || defined(ARDUINO_ARCH_SAMD)\
 || defined(ARDUINO_spresense_ast)
#define HT_SERIAL_MSGPACK_DISABLE_STL
#endif

#ifndef MAGPACK_ENABLE_DEBUG_LOG
#define NDEBUG // disable conversion warning
#endif

#include "MsgPack/Types.h"
#include "MsgPack/Packer.h"
#include "MsgPack/Unpacker.h"

namespace MsgPack = ht::serial::msgpack;

#endif // ARDUINOMSGPACK_H
