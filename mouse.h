#ifndef __MOUSE_H
#define __MOUSE_H

#include "types.h"
#include "interrupts.h"
#include "port.h"

class MouseDriver : public InterruptHandler {
public:
    MouseDriver(InterruptManager* manager);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
private:
    Port8Bit dataport;
    Port8Bit commandport;

    //0 contral bit, 1 x bit, 2 y bit
    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;

    int8_t x, y;
};

#endif 