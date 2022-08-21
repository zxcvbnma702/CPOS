#ifndef __CPOS__DRIVERS__MOUSE_H
#define __CPOS__DRIVERS__MOUSE_H

#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"

namespace cpos 
{
    namespace drivers 
    {
        //接口回调, 此代码与java中的接口作用一致
        class MouseEventHandler {
        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(cpos::common::uint8_t button);
            virtual void OnMouseUp(cpos::common::uint8_t button);
            virtual void OnMouseMove(cpos::common::int8_t x, cpos::common::int8_t y);
        };

        class MouseDriver : public cpos::hardwarecommunication::InterruptHandler, public Driver {
        public:
            MouseDriver(cpos::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();
            virtual cpos::common::uint32_t HandleInterrupt(cpos::common::uint32_t esp);
            virtual void Activate();
        private:
            cpos::hardwarecommunication::Port8Bit dataport;
            cpos::hardwarecommunication::Port8Bit commandport;

            //0 contral bit, 1 x bit, 2 y bit
            cpos::common::uint8_t buffer[3];
            cpos::common::uint8_t offset;
            cpos::common::uint8_t buttons;

            cpos::common::int8_t x, y;
            MouseEventHandler* handler;
        };
    }
}

#endif 