//
// Created by Manoel Amaro on 03.02.24.
//

#include "expression_tree.h"

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


void ExpressionTree::evaluateExpression(OpResult *result) {
    result->clearDetails();
    result->setResult(0);
    result->clearFlag(OP_RESULT_FLAG_CRITICAL_SUCCESS);
    result->clearFlag(OP_RESULT_FLAG_CRITICAL_FAILURE);    
    root->apply(result);
    delete root;
}

void ExpressionTree::reset() {
    delete root;
    root = nullptr;
}
