#include <unity.h>
#include <stdio.h>
#include "expression_tree.h"

#include "const_op.h"
#include "math_op.h"
#include "roll_op.h"

void test_detail(OpResultDetail *detail, uint8_t opType, uint8_t value)
{
    TEST_ASSERT_EQUAL(opType, detail->getOpType());
    TEST_ASSERT_EQUAL(value, detail->getValue());
}
template <size_t n>
void test_details_types_values(OpResult *result, uint8_t types_values[n][2])
{
    auto detail = result->details;
    for (int i = 0; i < n; i++)
    {
        test_detail(detail, types_values[i][0], types_values[i][1]);
        detail = detail->next;
    }
    TEST_ASSERT_EQUAL_PTR(NULL, detail);
}

void test_expression_tree(void)
{
    auto *tree = new ExpressionTree();
    tree->addNode(new ConstOp(5));
    tree->addNode(new MathOp(MATH_EXP_PLUS));
    tree->addNode(new ConstOp(3));
    tree->addNode(new MathOp(MATH_EXP_MINUS));
    tree->addNode(new ConstOp(2));
    tree->addNode(new MathOp(MATH_EXP_PLUS));
    tree->addNode(new ConstOp(2));
    tree->addNode(new MathOp(MATH_EXP_MINUS));
    tree->addNode(new ConstOp(2));
    tree->addNode(new MathOp(MATH_EXP_PLUS));
    tree->addNode(new ConstOp(7));
    auto result = new OpResult();

    tree->evaluateExpression(result);
    TEST_ASSERT_EQUAL(13, result->getResult());

    uint8_t types_values[11][2] = {
        {OP_RESULT_DETAIL_TYPE_CONST, 5},
        {OP_RESULT_DETAIL_TYPE_MATH_PLUS, 0},
        {OP_RESULT_DETAIL_TYPE_CONST, 3},
        {OP_RESULT_DETAIL_TYPE_MATH_MINUS, 0},
        {OP_RESULT_DETAIL_TYPE_CONST, 2},
        {OP_RESULT_DETAIL_TYPE_MATH_PLUS, 0},
        {OP_RESULT_DETAIL_TYPE_CONST, 2},
        {OP_RESULT_DETAIL_TYPE_MATH_MINUS, 0},
        {OP_RESULT_DETAIL_TYPE_CONST, 2},
        {OP_RESULT_DETAIL_TYPE_MATH_PLUS, 0},
        {OP_RESULT_DETAIL_TYPE_CONST, 7}
    };
    test_details_types_values<11>(result, types_values);

    delete result;
    delete tree;
}

void setUp(void)
{
    // Set up your testing objects here
}

void tearDown(void)
{
    // Clean up your testing objects here
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_expression_tree);
    //RUN_TEST(test_roll_op);
    //RUN_TEST(test_math_op);
    //RUN_TEST(test_math_roll_op);
    return UNITY_END();
}