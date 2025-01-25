#include <unity.h>
#include <stdio.h>
#include "dice.cpp"
#include "op_result.cpp"
#include "op.cpp"
#include "const_op.cpp"

void test_const_op(void)
{
    auto constOp = new ConstOp(1);

    auto result = new OpResult();

    constOp->apply(result);

    TEST_ASSERT_EQUAL(1, result->getResult());

    auto detail = result->getDetails();
    TEST_ASSERT_EQUAL(OP_RESULT_DETAIL_TYPE_CONST, detail->getOpType());
    TEST_ASSERT_EQUAL(1, detail->getValue());
    TEST_ASSERT_EQUAL(1, result->getDetailsIndex());
    delete constOp;
    delete result;
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
    //RUN_TEST(test_roll_op);
    //RUN_TEST(test_math_op);
    //RUN_TEST(test_math_roll_op);
    return UNITY_END();
}