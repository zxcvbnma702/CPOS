#include "systemcalls.h"

using namespace cpos;
using namespace cpos::common;
using namespace cpos::hardwarecommunication;

void printf(const char* str);

SystemHandler::SystemHandler(InterruptManager* interruptManager, uint8_t interruptNumber)
    :InterruptHandler(interruptNumber + interruptManager -> HardwareInterruptOffset(), interruptManager )
{
}
    
SystemHandler::~SystemHandler()
{
}

uint32_t SystemHandler::HandleInterrupt(uint32_t esp){
    CPUState* cpu = (CPUState*)esp;

    switch (cpu -> eax)
    {
    case 4:
        printf((char*)cpu -> ebx);
        break;
    
    default:
        break;
    }

    return esp;
}