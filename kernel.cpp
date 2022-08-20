#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"

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

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors(){
    for(constructor* i = &start_ctors; i != &end_ctors; i++){
        (*i)();
    }
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber) {
    printf("hello world!\n");
    printf("world!");

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);

    KeyBoardDriver keyboard(&interrupts);
    interrupts.Activate();
    
    while(1);
}