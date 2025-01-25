#ifndef OP_RESULT_H
#define OP_RESULT_H

#include <inttypes.h>

#define OP_RESULT_FLAG_CRITICAL_SUCCESS 1
#define OP_RESULT_FLAG_CRITICAL_FAILURE 2

#define OP_RESULT_DETAIL_TYPE_CONST 1
#define OP_RESULT_DETAIL_TYPE_MATH_PLUS 2
#define OP_RESULT_DETAIL_TYPE_MATH_MINUS 3
#define OP_RESULT_DETAIL_TYPE_ROLL 4
#define OP_RESULT_DETAIL_TYPE_ROLL_CRITICAL_SUCCESS 5
#define OP_RESULT_DETAIL_TYPE_ROLL_CRITICAL_FAILURE 6
#define OP_RESULT_DETAIL_TYPE_ROLL_DROP 7

#define OP_RESULT_MAX_DETAILS 100
class OpResult
{
private:
public:
    int result = 0;
    uint8_t flags = 0;

    OpResult();
    ~OpResult();
    void setFlag(uint8_t flag);
    void clearFlag(uint8_t flag);
    bool hasFlag(uint8_t flag);
};

#endif //OP_RESULT_H