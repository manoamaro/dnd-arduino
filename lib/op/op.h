//
// Created by Manoel Amaro on 28.01.24.
//

#ifndef TEST_OP_H
#define TEST_OP_H

#include <inttypes.h>
#include "op_result.h"

#define OP_CONST 0
#define OP_MATH 1
#define OP_DICE 2

class Op
{
public:
    uint8_t type;
    explicit Op(uint8_t type);
    virtual ~Op();
    virtual void apply(OpResult *result) = 0;
};

#endif // TEST_OP_H
