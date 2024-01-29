//
// Created by Manoel Amaro on 28.01.24.
//

#ifndef TEST_OP_H
#define TEST_OP_H


class Op {
public:
    virtual int apply() = 0;
};

class ConstOp: public Op {
public:
    explicit ConstOp(int value);

    int apply() override;

private:
    int value;

};

class BinOp: public Op {
public:
    BinOp(Op *left, Op *right);
protected:
    Op *left, *right;
};

enum MathExp {plus, minus};
class MathOp: public BinOp {
public:
    MathOp(Op *left, Op *right, MathExp exp);
    int apply() override;
private:
    MathExp exp;
};

#endif //TEST_OP_H
