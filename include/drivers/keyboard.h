#ifndef __CPOS__DRIVERS__KEYBOARD_H
#define __CPOS__DRIVERS__KEYBOARD_H

#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"

namespace cpos 
{
    namespace drivers 
    {
        //接口回调, 此代码与java中的接口作用一致
        class KeyboardEventHandler {
        public:
            KeyboardEventHandler();

            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };

        class KeyBoardDriver : public cpos::hardwarecommunication::InterruptHandler, public Driver {
        public:
            KeyBoardDriver(cpos::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler* handler);
            ~KeyBoardDriver();
            virtual cpos::common::uint32_t HandleInterrupt(cpos::common::uint32_t esp);
            virtual void Activate();
        private:
            cpos::hardwarecommunication::Port8Bit dataport;
            cpos::hardwarecommunication::Port8Bit commandport;

            KeyboardEventHandler* handler;
        };
    }
}


#endif 