#ifndef CONST_OP_H
#define CONST_OP_H

#include "op.h"

class ConstOp : public Op
{
private:

public:
    uint8_t value;
    explicit ConstOp(uint8_t value);
    ~ConstOp() override;
    void apply(OpResult *result) override;
};

#endif // CONST_OP_H