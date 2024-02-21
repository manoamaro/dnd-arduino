#include "op_result.h"

OpResultDetail::OpResultDetail()
{
}

OpResultDetail::~OpResultDetail()
{
}

uint8_t OpResultDetail::getOpType()
{
    return this->opType;
}

void OpResultDetail::setOpType(uint8_t opType)
{
    this->opType = opType;
}

uint8_t OpResultDetail::getValue()
{
    return this->value;
}

void OpResultDetail::setValue(uint8_t value)
{
    this->value = value;
}

OpResult::OpResult(): result(0), flags(0)
{
    
}

OpResult::~OpResult()
{
}

OpResultDetail* OpResult::getDetails()
{
    return this->details;
}

uint8_t OpResult::getDetailsIndex()
{
    return this->detailsIdx;
}

void OpResult::prependDetails(uint8_t opType, uint8_t value)
{
    for (int i = this->detailsIdx; i > 0; i--)
    {
        this->details[i] = this->details[i - 1];
    }
    this->details[0].setOpType(opType);
    this->details[0].setValue(value);
    this->detailsIdx++;
}

void OpResult::appendDetail(uint8_t opType, uint8_t value)
{
    this->setDetail(this->detailsIdx, opType, value);
    this->detailsIdx++;
}

void OpResult::clearDetails()
{
    this->detailsIdx = 0;
}

void OpResult::setDetail(uint8_t idx, uint8_t opType, uint8_t value)
{
    this->details[idx].setOpType(opType);
    this->details[idx].setValue(value);
}

void OpResult::setFlag(uint8_t flag)
{
    this->flags |= flag;
}

void OpResult::clearFlag(uint8_t flag)
{
    this->flags &= ~flag;
}

bool OpResult::hasFlag(uint8_t flag)
{
    return (this->flags & flag) == flag;
}

void OpResult::setResult(int result)
{
    this->result = result;
}

int OpResult::getResult()
{
    return this->result;
}
