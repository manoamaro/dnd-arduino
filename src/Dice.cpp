//
// Created by Manoel Amaro on 03.02.24.
//

#include "dice.h"
#include <stdlib.h>

namespace Dice {
    // 0: 4, 1: 6, 2: 8, 3: 10, 4: 12, 5: 20, 6: 100
    uint8_t sideOf(uint8_t index) {
        static const uint8_t sides[] = {4, 6, 8, 10, 12, 20, 100};
        return sides[index];
    }

    uint8_t roll(uint8_t minimum, uint8_t sides, uint32_t seed) {
        srand(seed);
        auto r = static_cast<uint8_t>(rand() % (sides - minimum + 1) + minimum);
        return _constrain(r, minimum, sides);
    }
} // namespace Dice

size_t utils::print(DisplaySSD1306_128x64_I2C *display, const __FlashStringHelper *ifsh) {
    PGM_P p = reinterpret_cast<PGM_P>(ifsh);
    size_t n = 0;
    while (1) {
        unsigned char c = pgm_read_byte(p++);
        if (c == 0) break;
        if (display->write(c)) n++;
        else break;
    }
    return n;
}
