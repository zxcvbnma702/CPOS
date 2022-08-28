#ifndef __CPOS__HARDWARECOMMUNICATION__PCI_H
#define __CPOS__HARDWARECOMMUNICATION__PCI_H

#include "hardwarecommunication/port.h"
#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "drivers/driver.h"

/*
    PCI -> 外围设备互联总线
    256 bus -> 32 device -> 8 function
*/

namespace cpos
{
    namespace hardwarecommunication
    {
        enum BaseAdressRegisterType{
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAdressRegister{
        public:
            //寄存器是否可以支持无副作用读取
            bool prefetchable;
            //寄存器地址
            cpos::common::uint8_t* address;
            //寄存器大小
            cpos::common::uint32_t size;
            BaseAdressRegisterType type;
        };

        //PCI Device Descriptor
        class PeripheralComponentInterconnectDeviceDescriptor {
        public:
            PeripheralComponentInterconnectDeviceDescriptor();
            ~PeripheralComponentInterconnectDeviceDescriptor();
        
            cpos::common::uint32_t portBase;
            cpos::common::uint32_t interrupt;

            cpos::common::uint8_t bus;
            cpos::common::uint8_t device;
            cpos::common::uint8_t function;

            cpos::common::uint16_t device_id;
            cpos::common::uint16_t vendor_id;

            cpos::common::uint8_t class_id;
            cpos::common::uint8_t subclass_id;
            cpos::common::uint8_t interface_id;
            cpos::common::uint8_t revision;
        };

        //PCI Controller
        class PeripheralComponentInterconnectController {
        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();

            cpos::common::uint32_t Read(cpos::common::uint8_t bus, 
                cpos::common::uint8_t device, 
                cpos::common::uint8_t function, 
                cpos::common::uint8_t registerOffset);

            void Write(cpos::common::uint8_t bus, 
                cpos::common::uint8_t device, 
                cpos::common::uint8_t function, 
                cpos::common::uint8_t registerOffset,
                cpos::common::uint32_t value);

            //判断设备是否有函数
            bool DeviceHasFunction(cpos::common::uint8_t bus, cpos::common::uint8_t device);

            void SelectDrivers(cpos::drivers::DriverManager* driverManager, cpos::hardwarecommunication::InterruptManager* interrupts);
            cpos::drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, cpos::hardwarecommunication::InterruptManager* interrupts);

            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(cpos::common::uint8_t bus, cpos::common::uint8_t device, cpos::common::uint8_t function);
            BaseAdressRegister GetBaseAddressRegister(cpos::common::uint8_t bus, cpos::common::uint8_t device, cpos::common::uint8_t function, cpos::common::uint8_t bar);
        private:
            Port32Bit dataPort;
            Port32Bit commandPort;
        };
    }
}
#endif