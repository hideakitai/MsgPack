#pragma once
#ifndef HT_SERIAL_MSGPACK_PACKER_H
#define HT_SERIAL_MSGPACK_PACKER_H

#include "util/ArxTypeTraits/ArxTypeTraits.h"
#ifdef HT_SERIAL_MSGPACK_DISABLE_STL
    #include "util/ArxContainer/ArxContainer.h"
#else
    #include <vector>
    #include <array>
    #include <deque>
    #include <tuple>
    #include <list>
    #include <forward_list>
    #include <set>
    #include <unordered_set>
    #include <map>
    #include <unordered_map>
    #include <limits>
#endif // HT_SERIAL_MSGPACK_DISABLE_STL
#ifdef TEENSYDUINO
    #include "util/TeensyDirtySTLErrorSolution/TeensyDirtySTLErrorSolution.h"
#endif // TEENSYDUINO

#include "Types.h"

namespace ht {
namespace serial {
namespace msgpack {

    class Packer
    {
        bin_t<uint8_t> buffer;

    public:

        template <typename First, typename ...Rest>
        auto encode(const First& first, Rest&&... rest)
        -> typename std::enable_if<!std::is_pointer<First>::value>::type
        {
            pack(first);
            encode(std::forward<Rest>(rest)...);
        }

        template <typename T>
        void encode(const T* data, const size_t size)
        {
            pack(data, size);
        }

        void encode()
        {
        }

        const bin_t<uint8_t>& packet() const { return buffer; }
        const uint8_t* data() const { return buffer.data(); }
        size_t size() const { return buffer.size(); }
        void clear() { buffer.clear(); }


        /////////////////////////////////////////////////
        // ---------- msgpack type adaptors ---------- //
        /////////////////////////////////////////////////

        // adaptation of types to msgpack
        // https://github.com/msgpack/msgpack-c/wiki/v2_0_cpp_adaptor


        // ---------- NIL format family ----------
        // - N/A

        template <typename T>
        auto pack(const T& n)
        -> typename std::enable_if<
            std::is_same<T, object::nil>::value ||
            std::is_same<T, std::nullptr_t>::value
        >::type
        {
            (void)n;
            packNil();
        }


        // ---------- BOOL format family ----------
        // - bool

        template <typename T>
        auto pack(const T& b)
        -> typename std::enable_if<std::is_same<T, bool>::value>::type
        {
            packBool(b);
        }


        // ---------- INT format family ----------
        // - char (signed/unsigned)
        // - ints (signed/unsigned)

        template <typename T>
        auto pack(const T& value)
        -> typename std::enable_if <
            std::is_arithmetic<T>::value &&
            !std::is_floating_point<T>::value &&
            !std::is_same<T, bool>::value &&
            !std::is_same<typename std::remove_cv<T>::type, char*>::value &&
            std::is_unsigned<T>::value
        >::type
        {
            size_t size = sizeof(T);
            if (size == sizeof(uint8_t))
                if (value <= (T)BitMask::UINT7) packIntU7(value);
                else                            packIntU8(value);
            else if (size == sizeof(uint16_t))  packIntU16(value);
            else if (size == sizeof(uint32_t))  packIntU32(value);
            else if (size == sizeof(uint64_t))  packIntU64(value);
        }

        template <typename T>
        auto pack(const T& value)
        -> typename std::enable_if <
            std::is_arithmetic<T>::value &&
            !std::is_floating_point<T>::value &&
            !std::is_same<T, bool>::value &&
            !std::is_same<typename std::remove_cv<T>::type, char*>::value &&
            std::is_signed<T>::value
        >::type
        {
            size_t size = sizeof(T);
            if (size == sizeof(int8_t))
                if ((value <  0) && value >= ((T)Type::INT5 | ((T)BitMask::INT5 & value)))
                    packInt5(value);
                else if ((value >= 0) && (value <= (T)BitMask::UINT7))
                    packIntU7(value);
                else
                    packInt8(value);
            else if (size == sizeof(int16_t))  packInt16(value);
            else if (size == sizeof(int32_t))  packInt32(value);
            else if (size == sizeof(int64_t))  packInt64(value);
        }


        // ---------- FLOAT format family ----------
        // - float
        // - double

        template <typename T>
        auto pack(const T& value)
        -> typename std::enable_if <
            std::is_arithmetic<T>::value &&
            std::is_floating_point<T>::value
        >::type
        {
            size_t size = sizeof(T);
            if (size == sizeof(float)) packFloat32(value);
            else                       packFloat64(value);
        }


        // ---------- STRING format family ----------
        // - char*
        // - char[]
        // - std::string

        void pack(const char* str)
        {
            str_t s(str);
            pack(s);
        }

        void pack(const str_t& str)
        {
            if (str.length() <= (size_t)BitMask::STR5)
                packString5(str);
            else if (str.length() <= std::numeric_limits<uint8_t>::max())
                packString8(str);
            else if (str.length() <= std::numeric_limits<uint16_t>::max())
                packString16(str);
            else if (str.length() <= std::numeric_limits<uint32_t>::max())
                packString32(str);
        }


        // ---------- BIN format family ----------
        // - unsigned char*
        // - unsigned char[]
        // - std::vector<char>
        // - std::vector<uint8_t>
        // - std::array<char>
        // - std::array<uint8_t>

        void pack(const uint8_t* bin, const size_t size)
        {
            if (size <= std::numeric_limits<uint8_t>::max())
                packBinary8(bin, size);
            else if (size <= std::numeric_limits<uint16_t>::max())
                packBinary16(bin, size);
            else if (size <= std::numeric_limits<uint32_t>::max())
                packBinary32(bin, size);
        }

        void pack(const bin_t<char>& bin)
        {
            pack((const uint8_t*)bin.data(), bin.size());
        }

        void pack(const bin_t<uint8_t>& bin)
        {
            pack(bin.data(), bin.size());
        }

#ifndef HT_SERIAL_MSGPACK_DISABLE_STL

        template <size_t N>
        void pack(const std::array<char, N>& bin)
        {
            pack((const uint8_t*)bin.data(), bin.size());
        }

        template <size_t N>
        void pack(const std::array<uint8_t, N>& bin)
        {
            pack(bin.data(), bin.size());
        }

#endif // HT_SERIAL_MSGPACK_DISABLE_STL

        // ---------- ARRAY format family ----------
        // - T[]
        // - std::vector
        // - std::array
        // - std::deque
        // - std::pair
        // - std::tuple
        // - std::list
        // - std::forward_list
        // - std::set
        // - std::multiset
        // - std::unordered_set *
        // - std::unordered_multiset *
        // * : not supported in arduino

        template <typename T>
        auto pack(const T* arr, const size_t size)
        -> typename std::enable_if <
            !std::is_same<T, char>::value &&
            !std::is_same<T, unsigned char>::value
        >::type
        {
            packArraySize(size);
            for (size_t i = 0; i < size; ++i) pack(arr[i]);
        }

        template <typename T>
        auto pack(const arr_t<T>& arr)
        -> typename std::enable_if <
            !std::is_same<T, char>::value &&
            !std::is_same<T, unsigned char>::value
        >::type
        {
            packArrayContainer(arr);
        }

#ifndef HT_SERIAL_MSGPACK_DISABLE_STL

        template <typename T, size_t N>
        auto pack(const std::array<T, N>& arr)
        -> typename std::enable_if <
            !std::is_same<T, char>::value &&
            !std::is_same<T, unsigned char>::value
        >::type
        {
            packArrayContainer(arr);
        }

        template <typename T>
        void pack(const std::deque<T>& arr)
        {
            packArrayContainer(arr);
        }

        template <typename T, typename U>
        void pack(const std::pair<T, U>& arr)
        {
            packArraySize(1);
            pack(arr.first);
            pack(arr.second);
        }

#endif // HT_SERIAL_MSGPACK_DISABLE_STL

        template<size_t I = 0, typename... Ts>
        auto pack(const std::tuple<Ts...>& t)
        -> typename std::enable_if<I < sizeof...(Ts)>::type
        {
            if (I == 0) packArraySize(sizeof...(Ts));
            pack(std::get<I>(t));
            pack<I + 1, Ts...>(t);
        }

        template<size_t I = 0, typename... Ts>
        auto pack(const std::tuple<Ts...>&)
        -> typename std::enable_if<I == sizeof...(Ts)>::type
        {
        }

#ifndef HT_SERIAL_MSGPACK_DISABLE_STL

        template <typename T>
        void pack(const std::list<T>& arr)
        {
            packArrayContainer(arr);
        }

        template <typename T>
        void pack(const std::forward_list<T>& arr)
        {
            auto size = std::distance(arr.begin(), arr.end());
            packArraySize(size);
            for (const auto& a : arr) pack(a);
        }

        template <typename T>
        void pack(const std::set<T>& arr)
        {
            packArrayContainer(arr);
        }

        template <typename T>
        void pack(const std::multiset<T>& arr)
        {
            packArrayContainer(arr);
        }

        template <typename T>
        void pack(const std::unordered_set<T>& arr)
        {
            packArrayContainer(arr);
        }

        template <typename T>
        void pack(const std::unordered_multiset<T>& arr)
        {
            packArrayContainer(arr);
        }

#endif // HT_SERIAL_MSGPACK_DISABLE_STL

        // ---------- MAP format family ----------
        // - std::map
        // - std::multimap
        // - std::unordered_map *
        // - std::unordered_multimap *
        // * : not supported in arduino

        template <typename T, typename U>
        void pack(const map_t<T, U>& mp)
        {
            packMapContainer(mp);
        }

#ifndef HT_SERIAL_MSGPACK_DISABLE_STL

        template <typename T, typename U>
        void pack(const std::multimap<T, U>& mp)
        {
            packMapContainer(mp);
        }

        template <typename T, typename U>
        void pack(const std::unordered_map<T, U>& mp)
        {
            packMapContainer(mp);
        }

        template <typename T, typename U>
        void pack(const std::unordered_multimap<T, U>& mp)
        {
            packMapContainer(mp);
        }


#endif // HT_SERIAL_MSGPACK_DISABLE_STL

        // ---------- EXT format family ----------

        void pack(const object::ext& e)
        {
            packExt(e);
        }


        // ---------- TIMESTAMP format family ----------

        void pack(const object::timespec& t)
        {
            packTimestamp(t);
        }


        // ---------- CUSTOM format ----------

        template <typename C>
        auto pack(const C& c)
        -> typename std::enable_if<has_to_msgpack<C, Packer&>::value>::type
        {
            c.to_msgpack(*this);
        }


        /////////////////////////////////////////
        // ---------- msgpack types ---------- //
        /////////////////////////////////////////

        // MessagePack Specification
        // https://github.com/msgpack/msgpack/blob/master/spec.md

        // ---------- NIL format family ----------

        void packNil()
        {
            packRawByte(Type::NIL);
        }

        void packNil(const object::nil& n)
        {
            (void)n;
            packRawByte(Type::NIL);
        }


        // ---------- BOOL format family ----------

        void packBool(const bool b)
        {
            packRawByte((uint8_t)Type::BOOL | ((uint8_t)b & (uint8_t)BitMask::BOOL));
        }


        // ---------- INT format family ----------

        void packIntU7(const uint8_t value)
        {
            packRawByte((uint8_t)Type::UINT7 | (value & (uint8_t)BitMask::UINT7));
        }

        void packIntU8(const uint8_t value)
        {
            packRawByte(Type::UINT8);
            packRawByte(value);
        }

        void packIntU16(const uint16_t value)
        {
            packRawByte(Type::UINT16);
            packRawReversed(value);
        }

        void packIntU32(const uint32_t value)
        {
            packRawByte(Type::UINT32);
            packRawReversed(value);
        }

        void packIntU64(const uint64_t value)
        {
            packRawByte(Type::UINT64);
            packRawReversed(value);
        }

        void packInt5(const int8_t value)
        {
            packRawByte((uint8_t)Type::INT5 | ((uint8_t)value & (uint8_t)BitMask::INT5));
        }

        void packInt8(const int8_t value)
        {
            packRawByte(Type::INT8);
            packRawByte(value);
        }

        void packInt16(const int16_t value)
        {
            packRawByte(Type::INT16);
            packRawReversed(value);
        }

        void packInt32(const int32_t value)
        {
            packRawByte(Type::INT32);
            packRawReversed(value);
        }

        void packInt64(const int64_t value)
        {
            packRawByte(Type::INT64);
            packRawReversed(value);
        }


        // ---------- FLOAT format family ----------

        void packFloat32(const float value)
        {
            packRawByte(Type::FLOAT32);
            packRawReversed(value);
        }

        void packFloat64(const double value)
        {
#ifndef HT_SERIAL_MSGPACK_DISABLE_STL
            packRawByte(Type::FLOAT64);
            packRawReversed(value);
#else
            packFloat32(value); // Uno, etc. does not support double
#endif // HT_SERIAL_MSGPACK_DISABLE_STL
        }


        // ---------- STR format family ----------

        void packString5(const str_t& str)
        {
            packRawByte((uint8_t)Type::STR5 | (str.length() & (uint8_t)BitMask::STR5));
            packRawBytes(str.c_str(), str.length());
        }
        void packString5(const char* value)
        {
            str_t str(value);
            packString5(str);
        }

        void packString8(const str_t& str)
        {
            packRawByte(Type::STR8);
            packRawByte((uint8_t)str.length());
            packRawBytes(str.c_str(), str.length());
        }
        void packString8(const char* value)
        {
            str_t str(value);
            packString8(str);
        }

        void packString16(const str_t& str)
        {
            packRawByte(Type::STR16);
            packRawReversed((uint16_t)str.length());
            packRawBytes(str.c_str(), str.length());
        }
        void packString16(const char* value)
        {
            str_t str(value);
            packString16(str);
        }

        void packString32(const str_t& str)
        {
            packRawByte(Type::STR32);
            packRawReversed((uint32_t)str.length());
            packRawBytes(str.c_str(), str.length());
        }
        void packString32(const char* value)
        {
            str_t str(value);
            packString32(str);
        }

        // ---------- BIN format family ----------

        void packBinary8(const uint8_t* value, const uint8_t size)
        {
            packRawByte(Type::BIN8);
            packRawByte(size);
            packRawBytes(value, size);
        }

        void packBinary16(const uint8_t* value, const uint16_t size)
        {
            packRawByte(Type::BIN16);
            packRawReversed(size);
            packRawBytes(value, size);
        }

        void packBinary32(const uint8_t* value, const uint32_t size)
        {
            packRawByte(Type::BIN32);
            packRawReversed(size);
            packRawBytes(value, size);
        }

        // ---------- ARRAY format family ----------

        void packArraySize(const size_t size)
        {
            if (size < (1 << 4))
                packArraySize4(size);
            else if (size <= std::numeric_limits<uint16_t>::max())
                packArraySize16(size);
            else if (size <= std::numeric_limits<uint32_t>::max())
                packArraySize32(size);
        }

        void packArraySize4(const uint8_t value)
        {
            packRawByte((uint8_t)Type::ARRAY4 | (value & (uint8_t)BitMask::ARRAY4));
        }

        void packArraySize16(const uint16_t value)
        {
            packRawByte(Type::ARRAY16);
            packRawReversed(value);
        }

        void packArraySize32(const uint32_t value)
        {
            packRawByte(Type::ARRAY32);
            packRawReversed(value);
        }


        // ---------- MAP format family ----------

        void packMapSize(const size_t size)
        {
            if (size < (1 << 4))
                packMapSize4(size);
            else if (size <= std::numeric_limits<uint16_t>::max())
                packMapSize16(size);
            else if (size <= std::numeric_limits<uint32_t>::max())
                packMapSize32(size);
        }

        void packMapSize4(const uint8_t value)
        {
            packRawByte((uint8_t)Type::MAP4 | (value & (uint8_t)BitMask::MAP4));
        }

        void packMapSize16(const uint16_t value)
        {
            packRawByte(Type::MAP16);
            packRawReversed(value);
        }

        void packMapSize32(const uint32_t value)
        {
            packRawByte(Type::MAP32);
            packRawReversed(value);
        }


        // ---------- EXT format family ----------

        void packFixExt1(const int8_t type, const uint8_t value)
        {
            packRawByte(Type::FIXEXT1);
            packRawByte((uint8_t)type);
            packRawByte(value);
        }

        void packFixExt2(const int8_t type, const uint16_t value)
        {
            packRawByte(Type::FIXEXT2);
            packRawByte((uint8_t)type);
            packRawBytes((const uint8_t*)&value, sizeof(value));
        }

        void packFixExt2(const int8_t type, const uint8_t* ptr)
        {
            packRawByte(Type::FIXEXT2);
            packRawByte((uint8_t)type);
            packRawBytes((const uint8_t*)ptr, 2);
        }

        void packFixExt2(const int8_t type, const uint16_t* ptr)
        {
            packFixExt2(type, (const uint8_t*)ptr);
        }

        void packFixExt4(const int8_t type, const uint32_t value)
        {
            packRawByte(Type::FIXEXT4);
            packRawByte((uint8_t)type);
            packRawBytes((const uint8_t*)&value, sizeof(value));
        }

        void packFixExt4(const int8_t type, const uint8_t* ptr)
        {
            packRawByte(Type::FIXEXT4);
            packRawByte((uint8_t)type);
            packRawBytes((const uint8_t*)ptr, 4);
        }

        void packFixExt4(const int8_t type, const uint32_t* ptr)
        {
            packFixExt4(type, (const uint8_t*)ptr);
        }

        void packFixExt8(const int8_t type, const uint64_t value)
        {
            packRawByte(Type::FIXEXT8);
            packRawByte((uint8_t)type);
            packRawBytes((const uint8_t*)&value, sizeof(value));
        }

        void packFixExt8(const int8_t type, const uint8_t* ptr)
        {
            packRawByte(Type::FIXEXT8);
            packRawByte((uint8_t)type);
            packRawBytes((const uint8_t*)ptr, 8);
        }

        void packFixExt8(const int8_t type, const uint64_t* ptr)
        {
            packFixExt8(type, (const uint8_t*)ptr);
        }

        void packFixExt16(const int8_t type, const uint64_t value_h, const uint64_t value_l)
        {
            packRawByte(Type::FIXEXT16);
            packRawByte((uint8_t)type);
            packRawBytes((const uint8_t*)&value_h, sizeof(value_h));
            packRawBytes((const uint8_t*)&value_l, sizeof(value_l));
        }

        void packFixExt16(const int8_t type, const uint8_t* ptr)
        {
            packRawByte(Type::FIXEXT16);
            packRawByte((uint8_t)type);
            packRawBytes((const uint8_t*)ptr, 16);
        }

        void packFixExt16(const int8_t type, const uint64_t* ptr)
        {
            packFixExt16(type, (const uint8_t*)ptr);
        }

        template <typename T>
        auto packFixExt(const int8_t type, const T value)
        -> typename std::enable_if<std::is_integral<T>::value>::type
        {
            size_t size = sizeof(T);
            if      (size == sizeof(uint8_t))  packFixExt1(type, value);
            else if (size == sizeof(uint16_t)) packFixExt2(type, value);
            else if (size == sizeof(uint32_t)) packFixExt4(type, value);
            else if (size == sizeof(uint64_t)) packFixExt8(type, value);
        }

        void packFixExt(const int8_t type, const uint64_t value_h, const uint64_t value_l)
        {
            packFixExt16(type, value_h, value_l);
        }

        void packFixExt(const int8_t type, const uint8_t* ptr, const uint8_t size)
        {
            if      (size == 0) return;
            if      (size == sizeof(uint8_t))      packFixExt1(type, (uint8_t)*(ptr));
            else if (size == sizeof(uint16_t))     packFixExt2(type, ptr);
            else if (size <= sizeof(uint32_t))     packFixExt4(type, ptr);
            else if (size <= sizeof(uint64_t))     packFixExt8(type, ptr);
            else if (size <= sizeof(uint64_t) * 2) packFixExt16(type, ptr);
        }

        void packFixExt(const int8_t type, const uint16_t* ptr, const uint8_t size)
        {
            packFixExt(type, (const uint8_t*)ptr, size);
        }

        void packFixExt(const int8_t type, const uint32_t* ptr, const uint8_t size)
        {
            packFixExt(type, (const uint8_t*)ptr, size);
        }

        void packFixExt(const int8_t type, const uint64_t* ptr, const uint8_t size)
        {
            packFixExt(type, (const uint8_t*)ptr, size);
        }

        void packExtSize8(const int8_t type, const uint8_t size)
        {
            packRawByte(Type::EXT8);
            packRawByte(size);
            packRawByte((uint8_t)type);
        }

        void packExtSize16(const int8_t type, const uint16_t size)
        {
            packRawByte(Type::EXT16);
            packRawReversed(size);
            packRawByte((uint8_t)type);
        }

        void packExtSize32(const int8_t type, const uint32_t size)
        {
            packRawByte(Type::EXT32);
            packRawReversed(size);
            packRawByte((uint8_t)type);
        }

        template <typename T, typename U>
        auto packExt(const int8_t type, const T* ptr, const U size)
        -> typename std::enable_if<std::is_integral<T>::value && std::is_integral<U>::value>::type
        {
            if (size <= sizeof(uint64_t) * 2)
                packFixExt(type, ptr, size);
            else
            {
                if (size <= std::numeric_limits<uint8_t>::max())
                    packExtSize8(type, size);
                else if (size <= std::numeric_limits<uint16_t>::max())
                    packExtSize16(type, size);
                else if (size <= std::numeric_limits<uint32_t>::max())
                    packExtSize32(type, size);
                packRawBytes((const uint8_t*)ptr, size);
            }
        }

        void packExt(const object::ext& e)
        {
            packExt(e.type(), e.data(), e.size());
        }


        // ---------- TIMESTAMP format family ----------

        void packTimestamp32(const uint32_t unix_time_sec)
        {
            packRawByte(Type::FIXEXT4);
            packRawByte((uint8_t)-1);
            packRawReversed(unix_time_sec);
        }

        void packTimestamp64(const uint64_t unix_time)
        {
            packRawByte(Type::FIXEXT8);
            packRawByte((uint8_t)-1);
            packRawReversed(unix_time);
        }

        void packTimestamp64(const uint64_t unix_time_sec, const uint32_t unix_time_nsec)
        {
            uint64_t utime = ((unix_time_nsec & 0x00000003FFFFFFFF) << 34) | (uint64_t)(unix_time_sec & 0x3FFFFFFFF);
            packTimestamp64(utime);
        }

        void packTimestamp96(const int64_t unix_time_sec, const uint32_t unix_time_nsec)
        {
            packExtSize8(-1, 12);
            packRawReversed(unix_time_nsec);
            packRawReversed(unix_time_sec);
        }

        void packTimestamp(const object::timespec& time)
        {
            if ((time.tv_sec >> 34) == 0)
            {
                uint64_t data64 = ((uint64_t)time.tv_nsec << 34) | time.tv_sec;
                if ((data64 & 0xffffffff00000000L) == 0)
                    packTimestamp32((uint32_t)data64);
                else
                    packTimestamp64(data64);
            }
            else
            {
                packTimestamp96(time.tv_sec, time.tv_nsec);
            }
        }


    private:

        void packRawByte(const uint8_t value)
        {
            buffer.emplace_back(value);
        }

        void packRawByte(const Type& type)
        {
            buffer.emplace_back((uint8_t)type);
        }

        void packRawBytes(const uint8_t* value, const size_t size)
        {
            for (size_t i = 0; i < size; ++i) buffer.emplace_back(value[i]);
        }

        void packRawBytes(const char* value, const size_t size)
        {
            for (size_t i = 0; i < size; ++i) buffer.emplace_back(value[i]);
        }

        template<typename DataType>
        void packRawReversed(const DataType& value)
        {
            const auto size = sizeof(DataType);
            for(size_t i = 0; i < size; ++i)
            {
                buffer.emplace_back(((uint8_t*)&value)[size - 1 - i]);
            }
        }

        template <typename A>
        void packArrayContainer(const A& arr)
        {
            packArraySize(arr.size());
            for (const auto& a : arr) pack(a);
        }

        template <typename M>
        void packMapContainer(const M& mp)
        {
            packMapSize(mp.size());
            for (const auto& m : mp)
            {
                pack(m.first);
                pack(m.second);
            }
        }
    };

} // namespace msgpack
} // namespace serial
} // namespace ht

#endif // HT_SERIAL_MSGPACK_PACKER_H

