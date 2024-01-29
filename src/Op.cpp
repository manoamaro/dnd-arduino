//
// Created by Manoel Amaro on 28.01.24.
//

#include "Op.h"

ConstOp::ConstOp(int value) : value(value) {}

int ConstOp::apply() {
    return this->value;
}

BinOp::BinOp(Op *left, Op *right) : left(left), right(right) {}

MathOp::MathOp(Op *left, Op *right, MathExp exp) : BinOp(left, right), exp(exp) {}

int MathOp::apply() {
    switch (this->exp) {
        case plus:
            return this->left->apply() + right->apply();
        case minus:
            return this->left->apply() - right->apply();
    }
}

