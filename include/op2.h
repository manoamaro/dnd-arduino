//
// Created by Manoel Amaro on 21.02.24.
//

#ifndef DND_OP2_H
#define DND_OP2_H

#include <inttypes.h>
#include "op_result.h"

#define OP_CONST 0
#define OP_MATH 1
#define OP_DICE 2

#define MATH_EXP_PLUS 0
#define MATH_EXP_MINUS 1

namespace op {
    struct Op2 {
        uint8_t type = 0;
        union {
            struct {
                uint8_t exp;
                Op2 *left;
                Op2 *right;
            } math;
            struct {
                uint8_t value = 0;
            } constant;
            struct {
                int seed;
                uint8_t sides = 4;
                uint8_t amount = 1;
                uint8_t minimum = 1;
                uint8_t keepHighest = 0;
                uint8_t reRollLowerThan = 0;
            } roll;
        };
    };

    Op2 *Op2_new(uint8_t type);

    Op2 *Op2_new_math(uint8_t exp);

    Op2 *Op2_new_roll(int seed);

    Op2 *Op2_new_const(uint8_t value);

    void Op2_free(Op2 *op);

    void Op2_apply(Op2 *op, OpResult *result);

}
#endif //DND_OP2_H
