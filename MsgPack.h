#pragma once
#ifndef HT_SERIAL_MSGPACK_H
#define HT_SERIAL_MSGPACK_H

#if defined(ARDUINO_ARCH_AVR)\
 || defined(ARDUINO_ARCH_MEGAAVR)\
 || defined(ARDUINO_ARCH_SAMD)\
 || defined(ARDUINO_spresense_ast)
#define HT_SERIAL_MSGPACK_DISABLE_STL
#endif

#ifdef HT_SERIAL_MSGPACK_DISABLE_STL
    #ifndef MSGPACK_PACKER_MAX_BUFFER_BYTE_SIZE
        #define MSGPACK_PACKER_MAX_BUFFER_BYTE_SIZE 128
    #endif // MSGPACK_PACKER_MAX_BUFFER_BYTE_SIZE
    #ifndef MSGPACK_UNPACKER_MAX_INDICES_SIZE
        #define MSGPACK_UNPACKER_MAX_INDICES_SIZE 16
    #endif // MSGPACK_UNPACKER_MAX_INDICES_SIZE
#endif // HT_SERIAL_MSGPACK_DISABLE_STL


#include "MsgPack/Types.h"
#include "MsgPack/Packer.h"
#include "MsgPack/Unpacker.h"

namespace MsgPack = ht::serial::msgpack;

#endif // ARDUINOMSGPACK_H
