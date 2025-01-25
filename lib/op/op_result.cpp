#include "op_result.h"

OpResult::OpResult(): result(0), flags(0)
{
    
}

OpResult::~OpResult()
{
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
