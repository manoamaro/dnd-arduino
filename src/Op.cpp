//
// Created by Manoel Amaro on 28.01.24.
//

#include "Op.h"
#include "Dice.h"

Op::Op(const uint8_t type) : type(type) {}

Op::~Op() {}

ConstOp::ConstOp(uint8_t value) : Op(OP_CONST), value(value) {}

ConstOp::~ConstOp() = default;

int ConstOp::apply() {
    return this->value;
}


MathOp::MathOp(uint8_t exp) : Op(OP_MATH), exp(exp) {}

MathOp::~MathOp() {
    delete this->left;
    delete this->right;
}

int MathOp::apply() {
    int l = this->left ? this->left->apply() : 0;
    int r = this->right ? this->right->apply() : 0;
    switch (this->exp) {
        case MATH_EXP_PLUS:
            return l + r;
        case MATH_EXP_MINUS:
            return l - r;
    }
    return 0;
}


RollOp::RollOp(int seed) : Op(OP_DICE), seed(seed) {}

RollOp::~RollOp() = default;

int RollOp::apply() {
    auto *results = new uint8_t[this->amount];
    int result = 0;
    for (int i = 0; i < this->amount; i++) {
        results[i] = roll(minimum, sides(), seed + i);
        if (results[i] == 1) {
            this->critical = ROLL_OP_CRITICAL_FAILURE;
        } else if (results[i] == sides()) {
            this->critical = ROLL_OP_CRITICAL_SUCCESS;
        }
        result += results[i];
    }
    if (reRollLowerThan > 0) {
        result = 0;
        for (int i = 0; i < this->amount; i++) {
            if (results[i] < reRollLowerThan) {
                results[i] = roll(minimum, sides(), seed + i);
                if (results[i] == 1) {
                    this->critical = ROLL_OP_CRITICAL_FAILURE;
                } else if (results[i] == sides()) {
                    this->critical = ROLL_OP_CRITICAL_SUCCESS;
                }
            }
            result += results[i];
        }
    }
    if (keepHighest > 0) {
        result = 0;
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
            result += results[i];
        }
    }
    delete[] results;
    return result;
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


