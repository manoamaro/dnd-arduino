#include <unity.h>
#include <stdio.h>
#include "Op.h"

void test_expression(void) {
    auto constOp = new ConstOp(1);
    TEST_ASSERT_EQUAL(1, constOp->apply());
    delete constOp;
}

void setUp(void) {
    // Set up your testing objects here
}

void tearDown(void) {
    // Clean up your testing objects here
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_expression);
    return UNITY_END();
}