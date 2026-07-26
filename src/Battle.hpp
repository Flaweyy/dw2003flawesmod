#pragma once

#include "extern/dw2003.hpp"
#include "extern/libc.hpp"
#include "extern/stddef.hpp"

struct BattleMessageTask
{
    uint8_t padding[0xB];
    int32_t state; // 0x0C
};

extern "C"
{
    BattleMessageTask* taskCreate(void* updateFunc, uint32_t size, uint32_t priority);
    void battleResultsMasterSequenceController(uint32_t*, uint32_t*);
    void battleResultsLevelUpController(uint32_t*, uint32_t*);
}