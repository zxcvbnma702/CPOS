void printf(const char* str) {
    //获取显示器地址
    //实模式下文本模式
    unsigned short* VideoMemory = (unsigned short*)0xb8000;
    for (int i = 0; str[i]; i++) {
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
    }
}

extern "C" void kernelMain(void* multiboot_structure, unsigned int magicnumber) {
    printf((char*)"hello world!");
    while(1);
}