#include "gdt.h"

using namespace cpos;
using namespace cpos::common;

GlobalDescriptorTable::GlobalDescriptorTable()
    :nullSegmentDescriptor(0, 0, 0), 
    unUsedSegmentDescriptor(0, 0, 0), 
    codeSegmentDescriptor(0, 64 * 1024 * 1024, 0x9a), 
    dataSegmentDescriptor(0, 64 * 1024 * 1024, 0x92)
{
    //GDT Descriptor
    uint32_t i[2];
    //高位 offset
    i[1] = (uint32_t)this; 
    //低位 size
    i[0] = sizeof(GlobalDescriptorTable) << 16;
    asm volatile("lgdt (%0)": :"p" (((uint8_t *)i) + 2));
}

GlobalDescriptorTable::~GlobalDescriptorTable(){}

uint16_t GlobalDescriptorTable::DataSegmentSelector() {
    return ((uint8_t*)&dataSegmentDescriptor - (uint8_t*)this) >> 3;
}

uint16_t GlobalDescriptorTable::CodeSegmentSelector() {
    return ((uint8_t*)&codeSegmentDescriptor - (uint8_t*)this) >> 3;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type){
    uint8_t* target = (uint8_t*)this;

    if(limit < 1048576){
        //段描述符的第七字节 20 ~ 23 high
        target[6] = 0x40;
    }else{
        if((limit & 0xfff) != 0xfff){
            limit = (limit >> 12) - 1;
        }else{
            limit = limit >> 12;
        }
        target[6] = 0xc0;
    }

    //Segement Limit
    // 0 ~ 15 low
    target[0] = limit & 0xff;
    target[1] = (limit >> 8) & 0xff;
    // 16 ~ 19 high
    target[6] |= (limit >> 16) & 0xf;

    //Base Address
    // 16 ~ 31 low
    target[2] = base & 0xff;
    target[3] = (base >> 8) & 0xff;
    // 0 ~ 7 high
    target[4] = (base >> 16) & 0xff;
    // 24 ~ 31 high
    target[7] = (base >> 24) & 0xff;

    // Type & S & DPL & P
    // 8 ~ 15 high
    target[5] = type;
}

 uint32_t GlobalDescriptorTable::SegmentDescriptor::Base() {
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];
    return result;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit() {
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[6] & 0xf;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if ((target[6] & 0xC0) == 0xC0)
        result = (result << 12) | 0xfff;
    return result;
}