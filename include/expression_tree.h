//
// Created by Manoel Amaro on 03.02.24.
//

#ifndef DND_EXPRESSIONTREE_H
#define DND_EXPRESSIONTREE_H

#include "op.h"
#include "math_op.h"


class ExpressionTree {
public:
    Op* root;
    ExpressionTree();
    ~ExpressionTree();
    void addNode(Op *newNode);
    void addNode(MathOp *newNode);
    void evaluateExpression(OpResult *result);
    void reset();
};

#endif //DND_EXPRESSIONTREE_H
