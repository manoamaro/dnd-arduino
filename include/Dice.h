//
// Created by Manoel Amaro on 03.02.24.
//

#ifndef DND_DICE_H
#define DND_DICE_H

#define DICE_COUNT 6

#include <inttypes.h>
#include <WString.h>
#include "lcdgfx.h"


#define _min(a, b) ((a)<(b)?(a):(b))
#define _max(a, b) ((a)>(b)?(a):(b))
#define _constrain(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

namespace utils
{
    size_t print(DisplaySSD1306_128x64_I2C *display, const __FlashStringHelper *ifsh);
} // namespace utils


namespace Dice {
    uint8_t sideOf(uint8_t index);
    uint8_t roll(uint8_t minimum, uint8_t sides, uint32_t seed);
}

#endif //DND_DICE_H
