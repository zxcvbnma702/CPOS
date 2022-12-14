#include "multitasking.h"

using namespace cpos;
using namespace cpos::common;

Task::Task(GlobalDescriptorTable* gdt, void entrypoint()) {
    //获取状态头
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));

    //初始化寄存器
    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;

    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    //EIP寄存器，用来存储CPU要读取指令的地址，CPU通过EIP寄存器读取即将要执行的指令
    cpustate->eip = (uint32_t)entrypoint;
    cpustate->cs = gdt->CodeSegmentSelector() << 3;
    cpustate->eflags = 0x202; // 0010 0000 0010
}

TaskManger::TaskManger() 
    : numTasks(0),
      currentTask(-1) {
}

bool TaskManger::AddTask(Task* task) {
    if (numTasks >= 256) {
        return false;
    }

    tasks[numTasks++] = task;
    return true;
}

CPUState* TaskManger::Schedule(CPUState* cpustate) {
    if (numTasks <= 0) return cpustate;

    if (currentTask >= 0) {
        tasks[currentTask]->cpustate = cpustate;
    }

    if (++currentTask >= numTasks) {
        currentTask %= numTasks;
    }
    return tasks[currentTask]->cpustate;
}