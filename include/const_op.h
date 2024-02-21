#ifndef CONST_OP_H
#define CONST_OP_H

#include "op.h"

class ConstOp : public Op {
public:
    uint8_t value;

    explicit ConstOp(uint8_t value);

    void apply(OpResult* result) override;

    ~ConstOp() override;
};

#endif //CONST_OP_H