#ifndef __GDT_H
#define __GDT_H

#include "common/types.h"
//全局描述符表
namespace cpos
{
    class GlobalDescriptorTable{
    public:
        //段描述符
        class SegmentDescriptor{
        public:
            //base -> 基址, limit -> 寻址的最大距离, type -> 段类型及访问权限
            SegmentDescriptor(cpos::common::uint32_t base, cpos::common::uint32_t limit, cpos::common::uint8_t type);
            //return the Base Address
            cpos::common::uint32_t Base();
            //return the Segement Limit
            cpos::common::uint32_t Limit();
            
        private:
            cpos::common::uint16_t limit_low;
            cpos::common::uint16_t base_low;
            cpos::common::uint8_t base_high;
            cpos::common::uint8_t type;
            cpos::common::uint8_t flags_limit_high;
            cpos::common::uint8_t base_vhigh;
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

        cpos::common::uint16_t CodeSegmentSelector();
        cpos::common::uint16_t DataSegmentSelector();
    };
}
#endif