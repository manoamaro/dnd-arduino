#include <unity.h>
#include <stdio.h>
#include <cstdio>
#include "dice.cpp"
#include "op_result.cpp"
#include "op.cpp"
#include "const_op.cpp"
#include "math_op.cpp"
#include "roll_op.cpp"

void printDetails(OpResultDetail *details)
{
    for (int i = 0; i < 10; i++)
    {
        auto detail = details[i];
        switch (detail.getOpType())
        {
        case OP_RESULT_DETAIL_TYPE_CONST:
            std::printf("%d", detail.getValue());
            break;
        case OP_RESULT_DETAIL_TYPE_MATH_PLUS:
            std::printf("+");
            break;
        case OP_RESULT_DETAIL_TYPE_MATH_MINUS:
            std::printf("-");
            break;
        case OP_RESULT_DETAIL_TYPE_ROLL:
            std::printf("(%d)", detail.getValue());
            break;
        case OP_RESULT_DETAIL_TYPE_ROLL_CRITICAL_SUCCESS:
            std::printf("**%d**", detail.getValue());
            break;
        case OP_RESULT_DETAIL_TYPE_ROLL_CRITICAL_FAILURE:
            std::printf("--%d--", detail.getValue());
            break;
        case OP_RESULT_DETAIL_TYPE_ROLL_DROP:
            std::printf("~%d~", detail.getValue());
            break;
        default:
            break;
        }
    }
    std::printf("\n");
}

void test_const_op(void)
{
    auto constOp = new ConstOp(1);
    auto result = new OpResult();

    constOp->apply(result);

    TEST_ASSERT_EQUAL(1, result->getResult());

    auto detail = result->getDetails();
    TEST_ASSERT_EQUAL(OP_RESULT_DETAIL_TYPE_CONST, detail->getOpType());
    TEST_ASSERT_EQUAL(1, detail->getValue());
    delete constOp;
    delete result;
}

void test_roll_op(void)
{
    auto rollOp = new RollOp(1);
    rollOp->amount = 5;
    rollOp->minimum = 1;
    rollOp->keepHighest = 0;
    rollOp->reRollLowerThan = 5;
    rollOp->sidesIdx = 5;

    auto result = new OpResult();

    rollOp->apply(result);

    TEST_ASSERT_EQUAL(50, result->getResult());

    printDetails(result->getDetails());

    auto details = result->getDetails();
    auto detail = details[0];
    TEST_ASSERT_EQUAL(OP_RESULT_DETAIL_TYPE_ROLL, detail.getOpType());
    TEST_ASSERT_EQUAL(8, detail.getValue());
    detail = details[1];
    TEST_ASSERT_EQUAL(OP_RESULT_DETAIL_TYPE_ROLL, detail.getOpType());
    TEST_ASSERT_EQUAL(15, detail.getValue());
    detail = details[2];
    TEST_ASSERT_EQUAL(OP_RESULT_DETAIL_TYPE_ROLL_DROP, detail.getOpType());
    TEST_ASSERT_EQUAL(2, detail.getValue());
    detail = details[3];
    TEST_ASSERT_EQUAL(OP_RESULT_DETAIL_TYPE_ROLL, detail.getOpType());
    TEST_ASSERT_EQUAL(2, detail.getValue());
    detail = details[4];
    TEST_ASSERT_EQUAL(OP_RESULT_DETAIL_TYPE_ROLL, detail.getOpType());
    TEST_ASSERT_EQUAL(9, detail.getValue());
    detail = details[5];
    TEST_ASSERT_EQUAL(OP_RESULT_DETAIL_TYPE_ROLL, detail.getOpType());
    TEST_ASSERT_EQUAL(16, detail.getValue());
    detail = details[6];

    delete rollOp;
    delete result;
}

void test_math_op(void)
{
    // 1 + 2 + 3 - 4 = 2
    auto leftMath = new MathOp(MATH_EXP_PLUS);
    auto leftConst = new ConstOp(1);
    auto rightConst = new ConstOp(2);
    leftMath->left = leftConst;
    leftMath->right = rightConst;
    auto rightMath = new MathOp(MATH_EXP_MINUS);
    leftConst = new ConstOp(3);
    rightConst = new ConstOp(4);
    rightMath->left = leftConst;
    rightMath->right = rightConst;
    auto mathOp = new MathOp(MATH_EXP_PLUS);
    mathOp->left = leftMath;
    mathOp->right = rightMath;
    auto result = new OpResult();

    mathOp->apply(result);

    printDetails(result->getDetails());

    TEST_ASSERT_EQUAL(2, result->getResult());
}

void test_math_roll_op(void)
{
    // 2d20min2ro5 + 2 + 3 + 1d4
    auto leftMath = new MathOp(MATH_EXP_PLUS);
    auto leftRoll = new RollOp(1);
    leftRoll->amount = 2;
    leftRoll->minimum = 2;
    leftRoll->reRollLowerThan = 5;
    leftRoll->sidesIdx = 5;
    auto rightConst = new ConstOp(2);
    leftMath->left = leftRoll;
    leftMath->right = rightConst;
    auto rightMath = new MathOp(MATH_EXP_PLUS);
    rightConst = new ConstOp(3);
    rightMath->left = rightConst;
    auto roll = new RollOp(1);
    roll->amount = 1;
    roll->sidesIdx = 3;
    rightMath->right = roll;
    auto mathOp = new MathOp(MATH_EXP_PLUS);
    mathOp->left = leftMath;
    mathOp->right = rightMath;

    auto result = new OpResult();
    mathOp->apply(result);

    printDetails(result->getDetails());

    TEST_ASSERT_EQUAL(29, result->getResult());
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
    RUN_TEST(test_const_op);
    RUN_TEST(test_roll_op);
    RUN_TEST(test_math_op);
    RUN_TEST(test_math_roll_op);
    return UNITY_END();
}