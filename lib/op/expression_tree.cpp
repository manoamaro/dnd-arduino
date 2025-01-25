#include "expression_tree.h"

ExpressionTree::ExpressionTree() : root(nullptr) {}

ExpressionTree::~ExpressionTree()
{
}

void ExpressionTree::addNode(std::shared_ptr<MathOp> mathOp)
{
    if (!root)
    {
        // If there's no root, the operator node cannot be added first.
        return;
    }
    mathOp->left = root;
    root = mathOp;
}

void ExpressionTree::addNode(std::shared_ptr<Op> newNode)
{
    if (!root)
    {
        root = newNode;
    }
    else if (root->type == OP_MATH)
    {
        auto mathOp = ((MathOp*)root.get());
        if (mathOp->right == nullptr)
        {
            mathOp->right = newNode;
        }
        else
        {
            auto current = mathOp->right;
            while (current->type == OP_MATH && ((MathOp*)current.get())->right != nullptr)
            {
                current = ((MathOp*)current.get())->right;
            }
            if (((MathOp*)current.get())->right == nullptr)
            {
                ((MathOp*)current.get())->right = newNode;
            }
        }
    }
}

void ExpressionTree::evaluateExpression(OpResult *result)
{
    root->apply(result);
}
