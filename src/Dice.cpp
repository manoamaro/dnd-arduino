//
// Created by Manoel Amaro on 03.02.24.
//

#include "Dice.h"
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