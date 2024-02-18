//
// Created by Manoel Amaro on 03.02.24.
//

#include "ExpressionTree.h"

ExpressionTree::ExpressionTree() : root(nullptr) {}

ExpressionTree::~ExpressionTree() {
    delete root;
}

void ExpressionTree::addNode(Op *newNode)  {
    if (!root) {
        root = newNode;
    } else if (root->type == OP_MATH && !((MathOp *) root)->right) {
        ((MathOp *) root)->right = newNode;
    } else {
        Op* current = root;
        while (current->type == OP_MATH && ((MathOp *) current)->right != nullptr) {
            current = ((MathOp *) current)->right;
        }
        if (((MathOp *) current)->right == nullptr) {
            ((MathOp *) current)->right = newNode;
        }
    }
}

void ExpressionTree::addNode(MathOp *newNode)  {
    if (!root) {
        // If there's no root, the operator node cannot be added first.
        return;
    }
    newNode->left = root;
    root = newNode;
}


int ExpressionTree::evaluateExpression() {
    if (!root) {
        return 0;
    }
    return root->apply();
}
