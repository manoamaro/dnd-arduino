#ifndef MATH_OP_H
#define MATH_OP_H

#include "op.h"
#include <memory>

#define MATH_EXP_PLUS 0
#define MATH_EXP_MINUS 1

class MathOp : public Op
{
private:

public:
    std::shared_ptr<Op> left, right = nullptr;
    uint8_t exp;

    explicit MathOp(uint8_t exp);
    ~MathOp() override;

    void apply(OpResult *result) override;
};

#endif // MATH_OP_H