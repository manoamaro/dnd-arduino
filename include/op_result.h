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


class OpResultDetail
{
private:
    uint8_t opType = 0;
    uint8_t value = 0;
public:
    OpResultDetail();
    ~OpResultDetail();
    uint8_t getOpType();
    void setOpType(uint8_t opType);
    uint8_t getValue();
    void setValue(uint8_t value);
};


#define OP_RESULT_MAX_DETAILS 30
class OpResult
{
private:
    int result = 0;
    uint8_t flags = 0;
    uint8_t detailsIdx = 0;
    OpResultDetail details[OP_RESULT_MAX_DETAILS];
public:
    OpResult();
    ~OpResult();
    OpResultDetail* getDetails();
    uint8_t getDetailsIndex();
    void prependDetails(uint8_t opType, uint8_t value);
    void appendDetail(uint8_t opType, uint8_t value);
    void setDetail(uint8_t idx, uint8_t opType, uint8_t value);
    void clearDetails();
    void setFlag(uint8_t flag);
    void clearFlag(uint8_t flag);
    bool hasFlag(uint8_t flag);
    void setResult(int result);
    int getResult();
};

#endif //OP_RESULT_H