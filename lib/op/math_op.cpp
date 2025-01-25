#include "math_op.h"

MathOp::MathOp(uint8_t exp) : Op(OP_MATH), exp(exp) {}

MathOp::~MathOp() {
}

void MathOp::apply(OpResult* result) {
    auto lr = new OpResult();
    auto rr = new OpResult();
    if (this->left) {
        this->left->apply(lr);
    }
    if (this->right) {
        this->right->apply(rr);
    }
    switch (this->exp) {
        case MATH_EXP_PLUS:
            result->result = lr->result + rr->result;
            break;
        case MATH_EXP_MINUS:
            result->result = lr->result - rr->result;
            break;
    }
}
