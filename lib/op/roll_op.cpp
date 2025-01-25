#include "roll_op.h"
#include "dice.h"

RollOp::RollOp(int seed) : Op(OP_DICE), seed(seed) {}

RollOp::~RollOp() = default;

void RollOp::apply(OpResult* result) {
    auto *results = new uint8_t[this->amount];
    int sum = 0;

    for (int i = 0; i < this->amount; i++) {
        results[i] = roll(minimum, sides(), seed + i);
        if (reRollLowerThan > 0) {
            if (results[i] <= reRollLowerThan) {
                // Roll again
                results[i] = roll(minimum, sides(), seed + i + 100);
            }
        }
        sum += results[i];
    }
    if (keepHighest > 0) {
        sum = 0;
        // sort the results array descending and sum the biggest n values
        for (int i = 0; i < this->amount; i++) {
            for (int j = i + 1; j < this->amount; j++) {
                if (results[i] < results[j]) {
                    uint8_t temp = results[i];
                    results[i] = results[j];
                    results[j] = temp;
                }
            }
        }

        for (int i = 0; i < keepHighest; i++) {
            sum += results[i];
        }
    }
    delete[] results;
    result->result = sum;
}

void RollOp::incSides() {
    this->sidesIdx = _constrain(this->sidesIdx + 1, 0, 5);
}

void RollOp::decSides() {
    this->sidesIdx = _constrain(this->sidesIdx - 1, 0, 5);
}

uint8_t RollOp::sides() const {
    return Dice::sideOf(this->sidesIdx);
}

uint8_t RollOp::roll(uint8_t min, uint8_t max, int _seed) {
    return Dice::roll(min, max, _seed);
}

