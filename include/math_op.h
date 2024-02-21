#ifndef MATH_OP_H
#define MATH_OP_H

#include "op.h"

#define MATH_EXP_PLUS 0
#define MATH_EXP_MINUS 1

class MathOp : public Op {
public:
    uint8_t exp;
    Op *left{}, *right = nullptr;

    explicit MathOp(uint8_t exp);

    void apply(OpResult* result) override;

    ~MathOp() override;

};

#endif //MATH_OP_H