//
// Created by Manoel Amaro on 21.02.24.
//
#include "op2.h"
#include "dice.h"
#include <stdlib.h>

namespace op {
    Op2 *Op2_new(const uint8_t type) {
        auto *op = (Op2 *) malloc(sizeof(Op2));
        op->type = type;
        return op;
    }

    Op2 *Op2_new_math(const uint8_t exp) {
        auto *op = Op2_new(OP_MATH);
        op->math.exp = exp;
        return op;
    }

    Op2 *Op2_new_roll(const int seed) {
        auto *op = Op2_new(OP_DICE);
        op->roll.seed = seed;
        op->roll.amount = 1;
        op->roll.sides = 4;
        op->roll.minimum = 1;
        op->roll.keepHighest = 0;
        op->roll.reRollLowerThan = 0;
        return op;
    }

    Op2 *Op2_new_const(const uint8_t value) {
        auto *op = Op2_new(OP_CONST);
        op->constant.value = value;
        return op;
    }

    void Op2_math_apply(Op2 *op, OpResult *result) {
        OpResult lr;
        OpResult rr;
        if (op->math.left) {
            Op2_apply(op->math.left, &lr);
        }
        if (op->math.right) {
            Op2_apply(op->math.right, &rr);
        }
        // prepend the details of the left result into current result
        for (int i = lr.getDetailsIndex() - 1; i >= 0; i--) {
            auto detail = lr.getDetails()[i];
            result->prependDetails(detail.getOpType(), detail.getValue());
        }

        // append the new detail to the current result
        result->appendDetail(
                op->math.exp == MATH_EXP_PLUS ? OP_RESULT_DETAIL_TYPE_MATH_PLUS : OP_RESULT_DETAIL_TYPE_MATH_MINUS, 0);

        // append the details of the right result into current result
        for (int i = 0; i < rr.getDetailsIndex(); i++) {
            auto detail = rr.getDetails()[i];
            result->appendDetail(detail.getOpType(), detail.getValue());
        }

        switch (op->math.exp) {
            case MATH_EXP_PLUS:
                result->setResult(lr.getResult() + rr.getResult());
                break;
            case MATH_EXP_MINUS:
                result->setResult(lr.getResult() - rr.getResult());
                break;
        }
    }

    void Op2_dice_apply(Op2 *op, OpResult *result) {
        auto *results = new uint8_t[op->roll.amount];
        int sum = 0;

        for (int i = 0; i < op->roll.amount; i++) {
            results[i] = Dice::roll(op->roll.minimum, op->roll.sides, op->roll.seed + i);
            result->appendDetail(OP_RESULT_DETAIL_TYPE_ROLL, results[i]);

            if (results[i] <= op->roll.reRollLowerThan) {
                result->setDetail(_max(0, result->getDetailsIndex() - 1), OP_RESULT_DETAIL_TYPE_ROLL_DROP, results[i]);
                // Roll again
                results[i] = Dice::roll(op->roll.minimum, op->roll.sides, op->roll.seed + i + 100);
                result->appendDetail(OP_RESULT_DETAIL_TYPE_ROLL, results[i]);
            }
            sum += results[i];
        }
        if (op->roll.keepHighest > 0) {
            sum = 0;
            // sort the results array descending and sum the biggest n values
            for (int i = 0; i < op->roll.amount; i++) {
                for (int j = i + 1; j < op->roll.amount; j++) {
                    if (results[i] < results[j]) {
                        uint8_t temp = results[i];
                        results[i] = results[j];
                        results[j] = temp;
                    }
                }
            }

            for (int i = 0; i < op->roll.keepHighest; i++) {
                sum += results[i];
            }
        }
        delete[] results;
        result->setResult(sum);
    }

    void Op2_apply(Op2 *op, OpResult *result) {
        switch (op->type) {
            case OP_CONST:
                result->setResult(op->constant.value);
                result->appendDetail(OP_RESULT_DETAIL_TYPE_CONST, op->constant.value);
                break;
            case OP_MATH:
                Op2_math_apply(op, result);
                break;
            case OP_DICE:
                Op2_dice_apply(op, result);
                break;
            default:
                break;
        }
    }

    void Op2_free(Op2 *op) {
        if (op->type == OP_MATH) {
            if (op->math.left) {
                Op2_free(op->math.left);
            }
            if (op->math.right) {
                Op2_free(op->math.right);
            }
        }
        free(op);
    }
} // namespace op