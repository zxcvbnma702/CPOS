#ifndef __GDT_H
#define __GDT_H

#include "types.h"
//全局描述符表
class GlobalDescriptorTable{
public:
    //段描述符
    class SegmentDescriptor{
    public:
        //base -> 基址, limit -> 寻址的最大距离, type -> 段类型及访问权限
        SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
        //return the Base Address
        uint32_t Base();
        //return the Segement Limit
        uint32_t Limit();
        
    private:
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t base_high;
        uint8_t type;
        uint8_t flags_limit_high;
        uint8_t base_vhigh;
    }__attribute__((packed)); //禁用内存对齐

    SegmentDescriptor nullSegmentDescriptor;
    SegmentDescriptor unUsedSegmentDescriptor;
    //代码段
    SegmentDescriptor codeSegmentDescriptor;
    //数据段
    SegmentDescriptor dataSegmentDescriptor;

public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();

    uint16_t CodeSegmentSelector();
    uint16_t DataSegmentSelector();
};
#endif