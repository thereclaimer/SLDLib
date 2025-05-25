#ifndef SLD_BIT_LOGIC_HPP
#define SLD_BIT_LOGIC_HPP

#include "sld.hpp"

namespace sld {
    
    utility bool
    bit_test(
        const u32 value,
        const u32 bit) {

        return((value >> bit) & 1);
    }

    utility void 
    bit_set(
        u32&       value,
        const u32  bit,
        const bool state) {

        if (state) {
            value |=  (1 << bit);
        }
        else {
            value &= ~(1 << bit);
        }
    }

    utility void
    bit_set_high(
        u32&      value,
        const u32 bit) {
        
        value |= (1 << bit);
    }

    utility void
    bit_set_low(
        u32&      value,
        const u32 bit) {

        value &= ~(1 << bit);
    }

    utility void 
    bit_toggle(
        u32&      value,
        const u32 bit) {

        value ^=  (1 << bit);
    }
};

#endif //SLD_BIT_LOGIC_HPP