#ifndef ROLL_OP_H
#define ROLL_OP_H

#include "op.h"

class RollOp : public Op {
private:
    int seed;
protected:
    static uint8_t roll(uint8_t min, uint8_t max, int _seed);

public:
    uint8_t sidesIdx = 0;
    uint8_t amount = 1;
    uint8_t minimum = 1;
    uint8_t keepHighest = 0;
    uint8_t reRollLowerThan = 0;

    explicit RollOp(int seed);

    void apply(OpResult* result) override;

    uint8_t sides() const;

    void incSides();

    void decSides();

    ~RollOp() override;
};

#endif //ROLL_OP_H