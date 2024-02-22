//
// Created by Manoel Amaro on 03.02.24.
//

#ifndef DND_EXPRESSIONTREE_H
#define DND_EXPRESSIONTREE_H

#include "op2.h"


class ExpressionTree {
public:
    op::Op2* root;
    ExpressionTree();
    ~ExpressionTree();
    void addNode(op::Op2 *newNode);
    void evaluateExpression(OpResult *result);
    void reset();
};

#endif //DND_EXPRESSIONTREE_H
