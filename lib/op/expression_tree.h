//
// Created by Manoel Amaro on 03.02.24.
//

#ifndef DND_EXPRESSIONTREE_H
#define DND_EXPRESSIONTREE_H

#include "op.h"
#include "math_op.h"
#include <memory>


class ExpressionTree {
public:
    std::shared_ptr<Op> root;
    ExpressionTree();
    ~ExpressionTree();
    void addNode(std::shared_ptr<MathOp> mathOp);
    void addNode(std::shared_ptr<Op> newNode);
    void evaluateExpression(OpResult *result);
};

#endif //DND_EXPRESSIONTREE_H
