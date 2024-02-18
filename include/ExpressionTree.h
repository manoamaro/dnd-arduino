//
// Created by Manoel Amaro on 03.02.24.
//

#ifndef DND_EXPRESSIONTREE_H
#define DND_EXPRESSIONTREE_H

#include "Op.h"

class ExpressionTree {
public:
    Op* root;
    ExpressionTree();
    ~ExpressionTree();
    void addNode(Op *newNode);
    void addNode(MathOp *newNode);
    int evaluateExpression();
};

#endif //DND_EXPRESSIONTREE_H
