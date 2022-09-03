#include "common/types.h"
#include "hardwarecommunication/pci.h"
#include "hardwarecommunication/interrupts.h"
#include "filesystem/msdocpart.h"
#include "drivers/driver.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "drivers/ata.h"
#include "drivers/vga.h"
#include "gui/desktop.h"
#include "gui/window.h"
#include "multitasking.h"
#include "memorymanagement.h"
#include "systemcalls.h"
#include "gdt.h"

using namespace cpos;
using namespace cpos::common;
using namespace cpos::drivers;
using namespace cpos::hardwarecommunication;
using namespace cpos::gui;
using namespace cpos::filesystem;

void printf(const char* str) {
    //获取显示器地址
    //实模式下文本模式
    //一页最大是 80 * 25 个字符
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i]; i++) {
        switch(str[i]) {
            case '\n':
                y++;
                x = 0;
                break;
            default:
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
                x++;
                break;
        }

        //换行
        if(x >= 80){
            x = 0;
            y++;
        }

        //换页
        if(y >= 25){
            for (y = 0; y < 25; y++) {
                for (x = 0; x < 80; x++) {
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
                }
            }
            x = 0, y = 0;
        }
    }
}

//打印端口号
void printfHex(uint8_t key) {
    char* foo = (char*)"00";
    const char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0x0f];
    foo[1] = hex[key & 0x0f];
    printf((const char*)foo);
}

void printfHex16(uint16_t key)
{
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}

void printfHex32(uint32_t key)
{
    printfHex((key >> 24) & 0xFF);
    printfHex((key >> 16) & 0xFF);
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}

void sysPrintf(char* str){
    asm volatile("int $0x80" : :"a" (4), "b" (str));
}

//键盘中断处理实现类
class PrintKeyboardEventHandler : public KeyboardEventHandler {
public:
    void OnKeyDown(char c) {
        char* foo = (char*)" ";
        foo[0] = c;
        printf(foo);
    }
};

//鼠标中断处理实现类
class MouseToConsole : public MouseEventHandler {
public:
    MouseToConsole()
        : x(40),
          y(12) {
    }

    void OnActivate() {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[y * 80 + x] = ((VideoMemory[y * 80 + x] & 0xf000) >> 4) |
                                ((VideoMemory[y * 80 + x] & 0x0f00) << 4) |
                                (VideoMemory[y * 80 + x] & 0x00ff);
    }

    void OnMouseMove(int8_t nx, int8_t ny) override {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[y * 80 + x] = ((VideoMemory[y * 80 + x] & 0xf000) >> 4) |
                                ((VideoMemory[y * 80 + x] & 0x0f00) << 4) |
                                (VideoMemory[y * 80 + x] & 0x00ff);

        x += nx;
        if (x < 0) x = 0;
        else if (x >= 80) x = 79;

        y += ny;
        if (y < 0) y = 0;
        else if (y >= 25) y = 24;

        VideoMemory[y  * 80 + x] = ((VideoMemory[y * 80 + x] & 0xf000) >> 4) |
                                ((VideoMemory[y * 80 + x] & 0x0f00) << 4) |
                                (VideoMemory[y * 80 + x] & 0x00ff);
    }

private:
    int8_t x, y;
};


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors(){
    for(constructor* i = &start_ctors; i != &end_ctors; i++){
        (*i)();
    }
}

void taskA(){
    while(true){
        sysPrintf("A");
    }
}

void taskB(){
    while(true){
        sysPrintf("B");
    }
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber) {

    GlobalDescriptorTable gdt;

    // 10M
    size_t heap = 10 * 1024 * 1024;
    // 通过grub获取可用内存, 64M
    uint32_t* memupper = (uint32_t*)((size_t)multiboot_structure + 8);
    MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

    printf("heap: 0x");
    printfHex((heap >> 24) & 0xff);
    printfHex((heap >> 16) & 0xff);
    printfHex((heap >> 8) & 0xff);
    printfHex((heap >> 0) & 0xff);

    void* allocated = memoryManager.malloc(1024);
    printf("\nallocated: 0x");
    printfHex(((size_t)allocated >> 24) & 0xff);
    printfHex(((size_t)allocated >> 16) & 0xff);
    printfHex(((size_t)allocated >> 8) & 0xff);
    printfHex(((size_t)allocated >> 0) & 0xff);

    TaskManger taskManger;
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManger.AddTask(&task1);
    taskManger.AddTask(&task2);

    InterruptManager interrupts(0x20, &gdt, &taskManger);
    SystemHandler syscalls(&interrupts, 0x80);

// #define GRAPHICMODE

#ifdef GRAPHICMODE
    Desktop desktop(320, 200, 0x00, 0x00, 0xa8);
#endif

#ifdef GRAPHICMODE
    Desktop desktop(320, 200, 0x00, 0x00, 0xa8);

    MouseDriver mouse(&interrupts, &desktop);
    KeyBoardDriver keyboard(&interrupts, &desktop);
#else
    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);

    PrintKeyboardEventHandler kbhandler;
    KeyBoardDriver keyboard(&interrupts, &kbhandler);
#endif

    DriverManager drvManager;
    drvManager.AddDriver(&keyboard);
    drvManager.AddDriver(&mouse);

    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);

    VideoGraphicsArray vga;
    drvManager.ActivateAll();

#ifdef GRAPHICMODE
    vga.SetMode(320, 200, 8);
    Window w1(&desktop, 10, 10, 20, 20, 0xa8, 0x00, 0x00);
    desktop.AddChild(&w1);

    Window w2(&desktop, 40, 15, 30, 30, 0x00, 0xa8, 0x00);
    desktop.AddChild(&w2);
#endif

    /**
     * Current disk controller chips almost always support two ATA buses per chip. 
     * There is a standardized set of IO ports to control the disks on the buses. 
     * The first two buses are called the Primary and Secondary ATA bus, 
     * and are almost always controlled by IO ports 0x1F0 through 0x1F7, 
     * and 0x170 through 0x177, respectively (unless you change it).
     * 
     * The standard IRQ for the Primary bus is IRQ14, and IRQ15 for the Secondary bus.
     */

    // AdvancedTechnologyAttachment ata0m(0x1F0, true);
    // printf("ATA Primary Master: ");
    // ata0m.Identify();
    // AdvancedTechnologyAttachment ata0s(0x1F0, false);
    // printf("\nATA Primary Slave: ");
    // ata0m.Identify();

    // char* buffer = "http://Alograaaaa.com";
    // ata0s.Write28(0, (uint8_t*)buffer, 21);
    // ata0s.Flush();

    // ata0s.Read28(0, (uint8_t*)buffer, 21);

    // MSDOSPartitionTable::ReadPartitions(&ata0m);

    // AdvancedTechnologyAttachment ata1m(0x170, true);
    // AdvancedTechnologyAttachment ata1s(0x170, false);

    interrupts.Activate();

    while(1){
        #ifdef GRAPHICMODE
            desktop.Draw(&vga);
        #endif
    }
}