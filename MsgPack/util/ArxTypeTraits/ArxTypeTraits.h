#pragma once

#ifndef ARX_TYPE_TRAITS_H
#define ARX_TYPE_TRAITS_H

#if !defined(ARX_HAVE_LIBSTDCPLUSPLUS)
    #if !defined(__has_include)
        #error "Compiler does not support __has_include, please report a bug against the ArxTypeTraits library about this."
    #endif
    #if __has_include(<cstdlib>)
        #include <cstdlib>
        #if defined(__GLIBCXX__) || defined(_LIBCPP_VERSION)
            // For gcc's libstdc++ and clang's libc++, assume that
            // __cplusplus tells us what the standard includes support
            #define ARX_HAVE_LIBSTDCPLUSPLUS __cplusplus
        #elif defined(__UCLIBCXX_MAJOR__)
            // For uclibc++, assume C++98 support only.
            #define ARX_HAVE_LIBSTDCPLUSPLUS 199711L
        #else
            #error "Unknown C++ library found, please report a bug against the ArxTypeTraits library about this."
        #endif
    #else
        // Assume no standard library is available at all (e.g. on AVR)
        #define ARX_HAVE_LIBSTDCPLUSPLUS 0
    #endif
#endif

// Make sure std namespace exists
namespace std { }

// Import everything from the std namespace into arx::std. This includes
// everything yet to be defined, so we can do this early (and must do
// so, to allow e.g. the C++14 additions in the arx::std namespace to
// reference the C++11 stuff from the system headers.
namespace arx {
    namespace arx_std {
        using namespace ::std;
    }
}

// Import everything from arx::arx_std back into the normal std namespace.
// This ensures that you can just use `std::foo` everywhere and you get
// the standard library version if it is available, falling back to arx
// versions for things not supplied by the standard library. Only when
// you really need the arx version (e.g. for constexpr numeric_limits
// when also using ArduinoSTL), you need to qualify with arx::arx_std::
namespace std {
    using namespace ::arx::arx_std;
}

#include "ArxTypeTraits/replace_minmax_macros.h"
#include "ArxTypeTraits/type_traits.h"

#endif // ARX_TYPE_TRAITS_H
