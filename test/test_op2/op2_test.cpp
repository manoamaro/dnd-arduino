#include <unity.h>
#include <stdio.h>
#include "dice.cpp"
#include "op_result.cpp"
#include "op2.cpp"

void printDetails2(OpResultDetail *details)
{
    for (int i = 0; i < 10; i++)
    {
        auto detail = details[i];
        switch (detail.getOpType())
        {
        case OP_RESULT_DETAIL_TYPE_CONST:
            //std::printf("%d", detail.getValue());
            break;
        case OP_RESULT_DETAIL_TYPE_MATH_PLUS:
            //std::printf("+");
            break;
        case OP_RESULT_DETAIL_TYPE_MATH_MINUS:
            //std::printf("-");
            break;
        case OP_RESULT_DETAIL_TYPE_ROLL:
            //std::printf("(%d)", detail.getValue());
            break;
        case OP_RESULT_DETAIL_TYPE_ROLL_CRITICAL_SUCCESS:
            //std::printf("**%d**", detail.getValue());
            break;
        case OP_RESULT_DETAIL_TYPE_ROLL_CRITICAL_FAILURE:
            //std::printf("--%d--", detail.getValue());
            break;
        case OP_RESULT_DETAIL_TYPE_ROLL_DROP:
            //std::printf("~%d~", detail.getValue());
            break;
        default:
            break;
        }
    }
    //std::printf("\n");
}

void test_const_op2(void)
{
    op::Op2* constOp = op::Op2_new_const(1);

    auto result = new OpResult();

    op::Op2_apply(constOp, result);

    TEST_ASSERT_EQUAL(1, result->getResult());

    auto detail = result->getDetails();
    TEST_ASSERT_EQUAL(OP_RESULT_DETAIL_TYPE_CONST, detail->getOpType());
    TEST_ASSERT_EQUAL(1, detail->getValue());
    TEST_ASSERT_EQUAL(1, result->getDetailsIndex());
    delete constOp;
    delete result;
}

void test_roll_op2(void)
{
    auto rollOp = op::Op2_new_roll(1);
    rollOp->roll.amount = 5;
    rollOp->roll.reRollLowerThan = 5;
    rollOp->roll.sides = 20;

    auto result = new OpResult();

    op::Op2_apply(rollOp, result);

    printDetails2(result->getDetails());

    TEST_ASSERT_EQUAL(50, result->getResult());

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

    delete rollOp;
    delete result;
}

void test_math_op2(void)
{
    // 1 + 2 + 3 - 4 = 2
    auto leftMath = op::Op2_new_math(MATH_EXP_PLUS);
    auto leftConst = op::Op2_new_const(1);
    auto rightConst = op::Op2_new_const(2);
    leftMath->math.left = leftConst;
    leftMath->math.right = rightConst;
    auto rightMath = op::Op2_new_math(MATH_EXP_MINUS);
    leftConst = op::Op2_new_const(3);
    rightConst = op::Op2_new_const(4);
    rightMath->math.left = leftConst;
    rightMath->math.right = rightConst;
    auto mathOp = op::Op2_new_math(MATH_EXP_PLUS);
    mathOp->math.left = leftMath;
    mathOp->math.right = rightMath;
    auto result = new OpResult();

    op::Op2_apply(mathOp, result);

    printDetails2(result->getDetails());

    TEST_ASSERT_EQUAL(2, result->getResult());
}

void test_math_roll_op2(void)
{
    // 2d20min2ro5 + 2 + 3 + 1d4

    auto leftMath = op::Op2_new_math(MATH_EXP_PLUS);
    auto leftRoll = op::Op2_new_roll(1);
    leftRoll->roll.amount = 2;
    leftRoll->roll.minimum = 2;
    leftRoll->roll.reRollLowerThan = 5;
    leftRoll->roll.sides = 20;
    auto rightConst = op::Op2_new_const(2);
    leftMath->math.left = leftRoll;
    leftMath->math.right = rightConst;
    auto rightMath = op::Op2_new_math(MATH_EXP_PLUS);
    rightConst = op::Op2_new_const(3);
    rightMath->math.left = rightConst;
    auto roll = op::Op2_new_roll(1);
    roll->roll.sides = 4;
    rightMath->math.right = roll;
    auto mathOp = op::Op2_new_math(MATH_EXP_PLUS);
    mathOp->math.left = leftMath;
    mathOp->math.right = rightMath;
    auto result = new OpResult();

    op::Op2_apply(mathOp, result);

    printDetails2(result->getDetails());

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
    RUN_TEST(test_const_op2);
    RUN_TEST(test_roll_op2);
    RUN_TEST(test_math_op2);
    RUN_TEST(test_math_roll_op2);
    return UNITY_END();
}