#ifndef __CPOS__DRIVERS__ATA_H
#define __CPOS__DRIVERS__ATA_H

#include "common/types.h"
#include "hardwarecommunication/port.h"

namespace cpos{
    namespace drivers{
        class AdvancedTechnologyAttachment{
        public:
            AdvancedTechnologyAttachment(common::uint16_t portBase, bool master);
            ~AdvancedTechnologyAttachment();

            void Identify();
            void Read28(common::uint32_t sector, common::uint8_t* data, int count);
            void Write28(common::uint32_t sector, common::uint8_t* data, int count);
            void Flush();

        protected:
            hardwarecommunication::Port16Bit dataPort;
            hardwarecommunication::Port8Bit errorPort;
            hardwarecommunication::Port8Bit sectorCountPort;
            hardwarecommunication::Port8Bit lbaLowPort;
            hardwarecommunication::Port8Bit lbaMidPort;
            hardwarecommunication::Port8Bit lbaHiPort;
            hardwarecommunication::Port8Bit devicePort;
            hardwarecommunication::Port8Bit commandPort;
            hardwarecommunication::Port8Bit controlPort;

            bool master;
            common::uint16_t bytesPerSector;
        };                                                                                                                                                                                                                                                                                                                                                  
    }
}

#endif