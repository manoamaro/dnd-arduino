#include "const_op.h"

ConstOp::ConstOp(uint8_t value) : Op(OP_CONST), value(value) {}

ConstOp::~ConstOp() = default;

void ConstOp::apply(OpResult* result) {
    result->setResult(this->value);
    result->appendDetail(OP_RESULT_DETAIL_TYPE_CONST, this->value);
}