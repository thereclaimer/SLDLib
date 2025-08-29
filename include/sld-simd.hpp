#ifndef SLD_SIMD_HPP
#define SLD_SIMD_HPP

#include <xmmintrin.h>

#include "sld.hpp"

#define SLD_SIMD_ALIGN_16 alignas(16)

namespace sld {

    struct SLD_SIMD_ALIGN_16 f32x4_t {
        f32 array[4];
    };
};

#endif //SLD_SIMD_HPP