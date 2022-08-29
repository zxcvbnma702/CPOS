#ifndef __CPOS__HARDWARECOMMUNICATION__INTERRUPTS_H
#define __CPOS__HARDWARECOMMUNICATION__INTERRUPTS_H

#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "gdt.h"
#include "multitasking.h"

namespace cpos
{
    namespace hardwarecommunication
    {
        class InterruptManager;

        class InterruptHandler {
        public:
            virtual cpos::common::uint32_t HandleInterrupt(cpos::common::uint32_t esp);
        protected:
            InterruptHandler(cpos::common::uint8_t interruptNumber, InterruptManager* interruptManager);
            ~InterruptHandler();

            cpos::common::uint8_t interruptNumber;
            InterruptManager* interruptManager;
        };


        class InterruptManager {
            friend class InterruptHandler;
        public:
            InterruptManager(cpos::common::uint16_t hardwareInterruptOffset, GlobalDescriptorTable* gdt, TaskManger* taskManger);
            ~InterruptManager();

            cpos::common::uint16_t HardwareInterruptOffset();
            void Activate();
            void Deactivate();

        protected:
            static InterruptManager* ActiveInterruptManager;
            InterruptHandler* handlers[256];
            TaskManger* taskManger;

            //门描述符
            struct GateDescriptor {
                cpos::common::uint16_t handlerAddressLowBits;
                cpos::common::uint16_t gdt_codeSegmentSelector;
                cpos::common::uint8_t reserved;
                cpos::common::uint8_t access;
                cpos::common::uint16_t handlerAddressHighBits;
            } __attribute__((packed));

            //中断描述符表
            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptDescriptorTablePointer {
                cpos::common::uint16_t size;
                cpos::common::uint32_t base;
            } __attribute__((packed));

            static void SetInterruptDescriptorTableEntry(
                cpos::common::uint8_t interruptNumber,
                cpos::common::uint16_t codeSegmentSelectorOffset,
                void (*handler)(),
                cpos::common::uint8_t DescriptorPrivilegelLevel,
                cpos::common::uint8_t DescriptorType
            );

            //硬件中断偏移
            cpos::common::uint16_t hardwareInterruptOffset;

            static void InterruptIgnore();

            static cpos::common::uint32_t HandleInterrupt(cpos::common::uint8_t interruptNumber, cpos::common::uint32_t esp);
            cpos::common::uint32_t DoHandleInterrupt(cpos::common::uint8_t interruptNumber, cpos::common::uint32_t esp);

            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x02();
            static void HandleInterruptRequest0x03();
            static void HandleInterruptRequest0x04();
            static void HandleInterruptRequest0x05();
            static void HandleInterruptRequest0x06();
            static void HandleInterruptRequest0x07();
            static void HandleInterruptRequest0x08();
            static void HandleInterruptRequest0x09();
            static void HandleInterruptRequest0x0A();
            static void HandleInterruptRequest0x0B();
            static void HandleInterruptRequest0x0C();
            static void HandleInterruptRequest0x0D();
            static void HandleInterruptRequest0x0E();
            static void HandleInterruptRequest0x0F();
            static void HandleInterruptRequest0x31();
            static void HandleInterruptRequest0x80();

            static void HandleException0x00();
            static void HandleException0x01();
            static void HandleException0x02();
            static void HandleException0x03();
            static void HandleException0x04();
            static void HandleException0x05();
            static void HandleException0x06();
            static void HandleException0x07();
            static void HandleException0x08();
            static void HandleException0x09();
            static void HandleException0x0A();
            static void HandleException0x0B();
            static void HandleException0x0C();
            static void HandleException0x0D();
            static void HandleException0x0E();
            static void HandleException0x0F();
            static void HandleException0x10();
            static void HandleException0x11();
            static void HandleException0x12();
            static void HandleException0x13();

            //8259芯片端口
            Port8BitSlow picMasterCommand;
            Port8BitSlow picMasterData;
            Port8BitSlow picSlaveCommand;
            Port8BitSlow picSlaveData;
        };
    }
}


#endif 