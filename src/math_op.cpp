#include "math_op.h"

MathOp::MathOp(uint8_t exp) : Op(OP_MATH), exp(exp) {}

MathOp::~MathOp() {
    delete this->left;
    delete this->right;
}

void MathOp::apply(OpResult* result) {
    OpResult lr;
    OpResult rr;
    if (this->left) {
        this->left->apply(&lr);
    }
    if (this->right) {
        this->right->apply(&rr);
    }
    // prepend the details of the left result into current result
    for (int i = lr.getDetailsIndex() - 1; i >= 0; i--) {
        auto detail = lr.getDetails()[i];
        result->prependDetails(detail.getOpType(), detail.getValue());
    }

    // append the new detail to the current result
    result->appendDetail(this->exp == MATH_EXP_PLUS ? OP_RESULT_DETAIL_TYPE_MATH_PLUS : OP_RESULT_DETAIL_TYPE_MATH_MINUS, 0);

    // append the details of the right result into current result
    for (int i = 0; i < rr.getDetailsIndex(); i++) {
        auto detail = rr.getDetails()[i];
        result->appendDetail(detail.getOpType(), detail.getValue());
    }

    switch (this->exp) {
        case MATH_EXP_PLUS:
            result->setResult(lr.getResult() + rr.getResult());
            break;
        case MATH_EXP_MINUS:
            result->setResult(lr.getResult() - rr.getResult());
            break;
    }
}
