#ifndef __SYSTEMCALLS_H
#define __SYSTEMCALLS_H

#include "common/types.h"
#include "multitasking.h"
#include "hardwarecommunication/interrupts.h"

namespace cpos{
    class SystemHandler : public hardwarecommunication::InterruptHandler
    {
    public:
        SystemHandler(hardwarecommunication::InterruptManager* interruptManager, cpos::common::uint8_t interruptNumber);
        ~SystemHandler();

        virtual cpos::common::uint32_t HandleInterrupt(cpos::common::uint32_t esp);
    };
}

#endif