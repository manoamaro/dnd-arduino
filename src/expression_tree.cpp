//
// Created by Manoel Amaro on 03.02.24.
//

#include "expression_tree.h"

ExpressionTree::ExpressionTree() : root(nullptr) {}

ExpressionTree::~ExpressionTree()
{
    delete root;
}

void ExpressionTree::addNode(op::Op2 *newNode)
{
    if (newNode->type == OP_MATH)
    {
        if (!root)
        {
            // If there's no root, the operator node cannot be added first.
            return;
        }
        newNode->math.left = root;
        root = newNode;
    }
    else
    {
        if (!root)
        {
            root = newNode;
        }
        else if (root->type == OP_MATH && !root->math.right)
        {
            root->math.right = newNode;
        }
        else
        {
            op::Op2 *current = root;
            while (current->type == OP_MATH && current->math.right != nullptr)
            {
                current = current->math.right;
            }
            if (current->math.right == nullptr)
            {
                current->math.right = newNode;
            }
        }
    }
}

void ExpressionTree::evaluateExpression(OpResult *result)
{
    result->clearDetails();
    result->setResult(0);
    result->clearFlag(OP_RESULT_FLAG_CRITICAL_SUCCESS);
    result->clearFlag(OP_RESULT_FLAG_CRITICAL_FAILURE);
    op::Op2_apply(root, result);
    op::Op2_free(root);
}

void ExpressionTree::reset()
{
    op::Op2_free(root);
    root = nullptr;
}
