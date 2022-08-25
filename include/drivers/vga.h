#ifndef __CPOS__DRIVERS__VGA_H
#define __CPOS__DRIVERS__VGA_H

#include "hardwarecommunication/port.h"
#include "common/types.h"
#include "hardwarecommunication/interrupts.h"

namespace cpos{
    namespace drivers{
        class VideoGraphicsArray{
        public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();

            bool SupportsMode(cpos::common::uint32_t width, cpos::common::uint32_t heigh, cpos::common::uint32_t colorDepth);
            bool SetMode(cpos::common::uint32_t width, cpos::common::uint32_t heigh, cpos::common::uint32_t colorDepth);
            void PutPixel(cpos::common::uint32_t x, cpos::common::uint32_t y, cpos::common::uint8_t colorIndex);
            void PutPixel(cpos::common::uint32_t x, cpos::common::uint32_t y, cpos::common::uint8_t r, cpos::common::uint8_t g, cpos::common::uint8_t b);

        private:
            hardwarecommunication::Port8Bit miscPort;
            hardwarecommunication::Port8Bit crtcIndexPort;
            hardwarecommunication::Port8Bit crtcDataPort;
            hardwarecommunication::Port8Bit sequencerIndexPort;
            hardwarecommunication::Port8Bit sequencerDataPort;
            hardwarecommunication::Port8Bit graphicsControllerIndexPort;
            hardwarecommunication::Port8Bit graphicsControllerDataPort;
            hardwarecommunication::Port8Bit attributeControllerIndexPort;
            hardwarecommunication::Port8Bit attributeControllerReadPort;
            hardwarecommunication::Port8Bit attributeControllerWritePort;
            hardwarecommunication::Port8Bit attributeControllerResetPort;


            void WriteRegisters(common::uint8_t* registers);
            cpos::common::uint8_t GetColorIndex(cpos::common::uint8_t r, cpos::common::uint8_t g, cpos::common::uint8_t b);
            cpos::common::uint8_t* GetFrameBufferSegment();
        };
    }
}

#endif
