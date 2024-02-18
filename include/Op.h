//
// Created by Manoel Amaro on 28.01.24.
//

#ifndef TEST_OP_H
#define TEST_OP_H

#include <inttypes.h>

#define OP_CONST 0
#define OP_MATH 1
#define OP_DICE 2

class Op {
public:
    uint8_t type;

    explicit Op(uint8_t type);

    virtual int apply() = 0;

    virtual ~Op();
};

class ConstOp : public Op {
public:
    uint8_t value;

    explicit ConstOp(uint8_t value);

    int apply() override;

    ~ConstOp() override;
};

#define MATH_EXP_PLUS 0
#define MATH_EXP_MINUS 1

class MathOp : public Op {
public:
    uint8_t exp;
    Op *left{}, *right = nullptr;

    explicit MathOp(uint8_t exp);

    int apply() override;

    ~MathOp() override;

};

#define ROLL_OP_CRITICAL_FAILURE 1
#define ROLL_OP_CRITICAL_SUCCESS 2

class RollOp : public Op {
private:
    uint8_t sidesIdx = 0;
    int seed;
protected:
    static uint8_t roll(uint8_t min, uint8_t max, int _seed);

public:
    uint8_t critical = 0;
    uint8_t amount = 1;
    uint8_t minimum = 1;
    uint8_t keepHighest = 0;
    uint8_t reRollLowerThan = 0;

    explicit RollOp(int seed);

    int apply() override;

    uint8_t sides() const;

    void incSides();

    void decSides();

    ~RollOp() override;
};


#endif //TEST_OP_H
